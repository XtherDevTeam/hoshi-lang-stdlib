cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)

project(hoshi-lang-stdlib)

file(GLOB ZIP_SRC lib/zip/*.c)
file(GLOB SQLITE_SRC lib/sqlite/*.c)
file(GLOB LIBNET_SRC lib/net/*.cpp)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Building for Windows, disable static linking ${OPENSSL_ROOT_DIR}")
else()
    message(STATUS "Building for non-Windows, enable static linking")
    set(OPENSSL_USE_STATIC_LIBS ON)
endif()

find_package(OpenSSL REQUIRED)

add_library(std STATIC ${ZIP_SRC} ${SQLITE_SRC} ${LIBNET_SRC} ${LIBSIG_SRC})

target_link_libraries(std OpenSSL::SSL OpenSSL::Crypto)

include_directories(hoshi-lang/)

message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(MODIFIED_OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES})
    list(REMOVE_ITEM MODIFIED_OPENSSL_LIBRARIES dl)
    list(APPEND MODIFIED_OPENSSL_LIBRARIES ${OPENSSL_INCLUDE_DIR}/../libcrypto-3-x64.dll)
    list(APPEND MODIFIED_OPENSSL_LIBRARIES ${OPENSSL_INCLUDE_DIR}/../libssl-3-x64.dll)
    message(STATUS "Patch for Windows: ${MODIFIED_OPENSSL_LIBRARIES}")
    file(COPY ${MODIFIED_OPENSSL_LIBRARIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FOLLOW_SYMLINK_CHAIN)

    set(CMAKE_CXX_FLAGS "-static")
    target_link_libraries(std wsock32 ws2_32)
else()
    set(MODIFIED_OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES})
    list(REMOVE_ITEM MODIFIED_OPENSSL_LIBRARIES dl)
    message(STATUS "OpenSSL libraries: ${MODIFIED_OPENSSL_LIBRARIES}")
    file(COPY ${MODIFIED_OPENSSL_LIBRARIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FOLLOW_SYMLINK_CHAIN)
    set(CMAKE_CXX_FLAGS "-static -ldl")
endif()