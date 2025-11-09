//
// Created by XIaokang00010 on 2025/11/02.
//

/**
 * libnet: A Cross-Platform C++ Socket Binding Library for FFI
 * Distributed along with hoshi-lang standard library under the MIT license.
 * Copyright (c) 2025 Jerry Chou a.k.a XtherDevTeam
 */

#ifndef HOSHILANGSTD_LIBNET
#define HOSHILANGSTD_LIBNET

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

struct __WSAINIT {
    __WSAINIT() {
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(2, 2), &wsa_data);
    }

    ~__WSAINIT() {
        WSACleanup();
    }
} ___WSAINIT;

#define inet_aton(a, b) inet_pton(AF_INET, (a), (b))
#define close_socket closesocket
#else

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define close_socket close

#endif

#define LIBNET_EXPORT extern "C"

#define LIBNET_SUCCESS 0
#define LIBNET_INET_ATON_FAIL -1
#define LIBNET_CONN_FAIL -2
#define LIBNET_LISTEN_FAIL -3
#define LIBNET_ACCEPT_FAIL -4
#define LIBNET_SEND_FAIL -5
#define LIBNET_RECV_FAIL -6
#define LIBNET_SSL_CONNECT_FAIL -7
#define LIBNET_SSL_SEND_FAIL -8
#define LIBNET_SSL_RECV_FAIL -9


#define LIBNET_DEFAULT_BACKLOG 5

struct sslInfo {
    SSL *ssl{};
    SSL_CTX *ctx{};
    int fd{};
};

// Parse the given domain name and return the resolved IP address as a string
// pointer shall be finalized by the caller
LIBNET_EXPORT const char *libnet_resolve(const char *domain_name);

LIBNET_EXPORT int libnet_socket(int64_t *result);

LIBNET_EXPORT int libnet_socket_connect(int fd, const char *ipAddr, uint16_t port);

LIBNET_EXPORT int libnet_socket_bind(int fd, const char *ipAddr, uint16_t port);

LIBNET_EXPORT int libnet_socket_listen(int fd, int backlog = LIBNET_DEFAULT_BACKLOG);

LIBNET_EXPORT int libnet_socket_accept(int fd, sockaddr_in *clientAddr, socklen_t *clientAddrLen);

LIBNET_EXPORT int libnet_socket_send(int fd, char *buf, u_int64_t len, int sendFlag = 0);

LIBNET_EXPORT int libnet_socket_recv(int fd, char *buf, u_int64_t len, int recvFlag = 0);

LIBNET_EXPORT void libnet_socket_close(int fd);

LIBNET_EXPORT sslInfo *libnet_ssl_connect(int fd);

LIBNET_EXPORT int libnet_ssl_send(sslInfo *fd, char *data, u_int64_t len);

LIBNET_EXPORT int libnet_ssl_recv(sslInfo *fd, char *dest, u_int64_t len);

LIBNET_EXPORT void libnet_ssl_close(sslInfo *fd);

LIBNET_EXPORT void libnet_free(void *ptr);

#endif // HOSHILANGSTD_LIBNET
