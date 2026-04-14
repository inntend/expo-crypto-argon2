cmake_minimum_required(VERSION 3.22.1)
project(ExpoCryptoArgon2)

# Path to the phc-winner-argon2 submodule (relative to this CMakeLists.txt)
set(C_ARGON2_DIR "${CMAKE_SOURCE_DIR}/../c-argon2")

add_library(
    expocryptoargon2
    SHARED
    src/main/cpp/argon2-jni.cpp
    ${C_ARGON2_DIR}/src/argon2.c
    ${C_ARGON2_DIR}/src/core.c
    ${C_ARGON2_DIR}/src/encoding.c
    ${C_ARGON2_DIR}/src/ref.c
    ${C_ARGON2_DIR}/src/thread.c
    ${C_ARGON2_DIR}/src/blake2/blake2b.c
    # Intentionally excluded: ${C_ARGON2_DIR}/src/run.c (defines main())
)

target_include_directories(expocryptoargon2 PRIVATE
    ${C_ARGON2_DIR}/include
)

find_library(log-lib log)
target_link_libraries(expocryptoargon2 ${log-lib})
