cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(hoshi-lang-stdlib)

file(GLOB ZIP_SRC lib/zip/*.c)
file(GLOB SQLITE_SRC lib/sqlite/*.c)
file(GLOB LIBNET_SRC lib/net/*.cpp)


find_package(OpenSSL REQUIRED)

add_library(std STATIC ${ZIP_SRC} ${SQLITE_SRC} ${LIBNET_SRC})

target_link_libraries(std OpenSSL::SSL OpenSSL::Crypto)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    file(GLOB FUCK_OPENSSL "${OPENSSL_INCLUDE_DIR}/../*dll*")
    message(Patch for Windows: ${OPENSSL_INCLUDE_DIR})
    file(COPY ${FUCK_OPENSSL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FOLLOW_SYMLINK_CHAIN)

    set(CMAKE_CXX_FLAGS "-static")
    target_link_libraries(std wsock32 ws2_32)
endif()