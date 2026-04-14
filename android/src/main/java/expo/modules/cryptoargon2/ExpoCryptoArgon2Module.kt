package expo.modules.cryptoargon2

import expo.modules.kotlin.modules.Module
import expo.modules.kotlin.modules.ModuleDefinition
import expo.modules.kotlin.records.Field
import expo.modules.kotlin.records.Record

class Argon2Params : Record {
    @Field val password: String = ""
    @Field val salt: String = ""
    @Field val iterations: Int = 3
    @Field val memory: Int = 65536
    @Field val parallelism: Int = 1
    @Field val hashLength: Int = 32
    @Field val outputType: String = "binary"  // "binary" | "encoded"
}

class ExpoCryptoArgon2Module : Module() {

    companion object {
        init {
            System.loadLibrary("expocryptoargon2")
        }
    }

    // Implemented in android/src/main/cpp/argon2-jni.cpp
    private external fun nativeArgon2id(
        password: String,
        salt: String,
        iterations: Int,
        memory: Int,
        parallelism: Int,
        hashLength: Int,
        outputType: String
    ): String

    override fun definition() = ModuleDefinition {
        Name("ExpoCryptoArgon2")

        // AsyncFunction runs on expo-modules-core's background executor.
        // Returning a value resolves the JS Promise; throwing rejects it.
        AsyncFunction("argon2id") { params: Argon2Params ->
            nativeArgon2id(
                params.password,
                params.salt,
                params.iterations,
                params.memory,
                params.parallelism,
                params.hashLength,
                params.outputType
            )
        }
    }
}
