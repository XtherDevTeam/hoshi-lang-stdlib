#include "net.h"
#include <cstdio>

LIBNET_EXPORT const char *libnet_resolve(const char *domain_name) {
    // Set up hints for the getaddrinfo function
    addrinfo hints{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // We want IPv4 addresses
    hints.ai_socktype = SOCK_STREAM; // We want TCP sockets

    // Call getaddrinfo to resolve the domain name
    addrinfo *addr_list;
    int error = getaddrinfo(domain_name, "80", &hints, &addr_list);
    if (error != 0) {
        return nullptr;
    }

    // Convert the resolved IP address to a string
    char *ip_str = (char *)malloc(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &((sockaddr_in *)addr_list->ai_addr)->sin_addr, ip_str, INET_ADDRSTRLEN);

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(addr_list);

    // Return the resolved IP address
    return ip_str;
}

LIBNET_EXPORT int libnet_socket(int64_t *result) {
    if (int rc = ::socket(AF_INET, SOCK_STREAM, 0); rc < 0) {
        return rc;
    } else {
        *result = rc;
        return 0;
    }
}

LIBNET_EXPORT int libnet_socket_connect(int fd, const char *ipAddr, uint16_t port) {
    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (int rc = inet_aton(ipAddr, &serv_addr.sin_addr); rc != 1) {
        return LIBNET_INET_ATON_FAIL;
    }
    if (int rc = ::connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); rc < 0) {
        return LIBNET_CONN_FAIL;
    }
    return 0;
}

LIBNET_EXPORT int libnet_socket_bind(int fd, const char *ipAddr, uint16_t port) {
    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (int rc = inet_aton(ipAddr, &serv_addr.sin_addr); rc != 1) {
        return LIBNET_INET_ATON_FAIL;
    }
    if (int rc = ::bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); rc < 0) {
        return rc;
    }
    return 0;
}

LIBNET_EXPORT int libnet_socket_accept(int fd, sockaddr_in *clientAddr, socklen_t *clientAddrLen) {
    if (auto nfd = accept(fd, (struct sockaddr *)clientAddr, clientAddrLen); nfd > 0) {
        return nfd;
    } else {
        return LIBNET_ACCEPT_FAIL;
    }
}

LIBNET_EXPORT int libnet_socket_send(int fd, char *buf, u_int64_t len, int sendFlag) {
    if (send(fd, buf, len, sendFlag) < 0) {
        return LIBNET_SEND_FAIL;
    }
    return LIBNET_SUCCESS;
}

LIBNET_EXPORT int libnet_socket_recv(int fd, char *buf, u_int64_t len, int recvFlag) {
    if (auto res = recv(fd, buf, len, recvFlag); res < 0) {
        return LIBNET_RECV_FAIL;
    } else if (res == 0) {
        return 0;
    } else {
        return res;
    }
}

LIBNET_EXPORT void libnet_socket_close(int fd) {
    close_socket(fd);
}

LIBNET_EXPORT sslInfo *libnet_ssl_connect(int fd, const char *domain) {
    auto r = (sslInfo *)malloc(sizeof(sslInfo));
    if ((r->ctx = SSL_CTX_new(TLS_client_method())) == nullptr) {
        free(r);
        return nullptr;
    }
    if (SSL_CTX_set_default_verify_paths(r->ctx) != 1) {
        free(r);
        return nullptr;
    }

    r->ssl = SSL_new(r->ctx);
    SSL_set_fd(r->ssl, fd);

    // only when domain is explicit set, set SNI extension
    if (domain) {
        if (SSL_set_tlsext_host_name(r->ssl, domain) != 1) {
            free(r);
            return nullptr;
        }
        SSL_set1_host(r->ssl, domain);
    }

    if (auto retCode = SSL_connect(r->ssl); retCode != 1) {
        // print error message
        // char error_buffer[1024];
        // ERR_error_string_n(ERR_get_error(), error_buffer, sizeof(error_buffer));
        // puts(error_buffer);
        free(r);
        return nullptr;
    }
    r->fd = fd;
    return r;
}

LIBNET_EXPORT int libnet_ssl_send(sslInfo *fd, char *data, u_int64_t len) {
    if (SSL_write(fd->ssl, data, len) <= 0) {
        return LIBNET_SSL_SEND_FAIL;
    }
    return LIBNET_SUCCESS;
}

LIBNET_EXPORT int libnet_ssl_recv(sslInfo *fd, char *dest, u_int64_t len) {
    if (auto res = SSL_read(fd->ssl, dest, len); res < 0) {
        return LIBNET_SSL_RECV_FAIL;
    } else if (res == 0) {
        return 0;
    } else {
        return res;
    }
}

LIBNET_EXPORT void libnet_ssl_close(sslInfo *fd) {
    SSL_CTX_free(fd->ctx);
    SSL_shutdown(fd->ssl);
    SSL_free(fd->ssl);
    close_socket(fd->fd);
}

LIBNET_EXPORT int libnet_socket_listen(int fd, int backlog) {
    if (int rc = ::listen(fd, backlog); rc < 0) {
        return rc;
    } else {
        return 0;
    }
}

LIBNET_EXPORT void libnet_free(void *ptr) {
    return free(ptr);
}

LIBNET_EXPORT int libnet_ssl_get_error() {
    return ERR_get_error();
}

LIBNET_EXPORT char *libnet_ssl_error_message(int rc) {
    char *error_buffer = (char *)malloc(1024);
    ERR_error_string_n(rc, error_buffer, 1024);
    return error_buffer;
}

LIBNET_EXPORT void libnet_ssl_clear_error() {
    ERR_clear_error();
}