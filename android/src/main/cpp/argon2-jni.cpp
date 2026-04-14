#include <jni.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include "argon2.h"

static std::string hexEncode(const uint8_t *data, size_t len) {
    static const char kHex[] = "0123456789abcdef";
    std::string out;
    out.reserve(len * 2);
    for (size_t i = 0; i < len; ++i) {
        out.push_back(kHex[(data[i] >> 4) & 0xF]);
        out.push_back(kHex[data[i] & 0xF]);
    }
    return out;
}

static std::vector<uint8_t> hexDecode(const char *hex, size_t hexLen) {
    std::vector<uint8_t> out(hexLen / 2);
    for (size_t i = 0; i < out.size(); ++i) {
        char hi = hex[i * 2];
        char lo = hex[i * 2 + 1];
        uint8_t h = (hi >= 'a') ? (uint8_t)(hi - 'a' + 10) : (uint8_t)(hi - '0');
        uint8_t l = (lo >= 'a') ? (uint8_t)(lo - 'a' + 10) : (uint8_t)(lo - '0');
        out[i] = (h << 4) | l;
    }
    return out;
}

// JNI function name: Java_<package>_<class>_<method>
extern "C" JNIEXPORT jstring JNICALL
Java_expo_modules_cryptoargon2_ExpoCryptoArgon2Module_nativeArgon2id(
    JNIEnv *env,
    jobject /* thiz */,
    jstring passwordHex,
    jstring saltHex,
    jint    iterations,
    jint    memory,
    jint    parallelism,
    jint    hashLength,
    jstring outputTypeJ
) {
    const char *pwdHexC   = env->GetStringUTFChars(passwordHex, nullptr);
    const char *saltHexC  = env->GetStringUTFChars(saltHex,     nullptr);
    const char *outputTypeC = env->GetStringUTFChars(outputTypeJ, nullptr);

    auto pwd  = hexDecode(pwdHexC,  (size_t)env->GetStringUTFLength(passwordHex));
    auto salt = hexDecode(saltHexC, (size_t)env->GetStringUTFLength(saltHex));
    bool wantEncoded = (std::string(outputTypeC) == "encoded");

    env->ReleaseStringUTFChars(passwordHex, pwdHexC);
    env->ReleaseStringUTFChars(saltHex,     saltHexC);
    env->ReleaseStringUTFChars(outputTypeJ, outputTypeC);

    if (wantEncoded) {
        size_t encodedLen = argon2_encodedlen(
            (uint32_t)iterations, (uint32_t)memory, (uint32_t)parallelism,
            (uint32_t)salt.size(), (uint32_t)hashLength, Argon2_id
        );

        std::string encoded(encodedLen, '\0');

        int rc = argon2id_hash_encoded(
            (uint32_t)iterations, (uint32_t)memory, (uint32_t)parallelism,
            pwd.data(), pwd.size(),
            salt.data(), salt.size(),
            (size_t)hashLength,
            &encoded[0], encodedLen
        );

        std::memset(pwd.data(), 0, pwd.size());

        if (rc != ARGON2_OK) {
            jclass ex = env->FindClass("java/lang/Exception");
            env->ThrowNew(ex, argon2_error_message(rc));
            return nullptr;
        }

        // Strip the trailing null written by argon2 into the string buffer
        encoded.resize(std::strlen(encoded.c_str()));
        return env->NewStringUTF(encoded.c_str());

    } else {
        // binary — return lowercase hex of raw hash bytes
        std::vector<uint8_t> hash((size_t)hashLength);

        int rc = argon2id_hash_raw(
            (uint32_t)iterations, (uint32_t)memory, (uint32_t)parallelism,
            pwd.data(), pwd.size(),
            salt.data(), salt.size(),
            hash.data(), hash.size()
        );

        std::memset(pwd.data(), 0, pwd.size());

        if (rc != ARGON2_OK) {
            std::memset(hash.data(), 0, hash.size());
            jclass ex = env->FindClass("java/lang/Exception");
            env->ThrowNew(ex, argon2_error_message(rc));
            return nullptr;
        }

        std::string result = hexEncode(hash.data(), hash.size());
        std::memset(hash.data(), 0, hash.size());
        return env->NewStringUTF(result.c_str());
    }
}
