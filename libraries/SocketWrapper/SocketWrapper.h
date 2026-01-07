#pragma once

#include "zephyr/sys/printk.h"
#if defined(CONFIG_NET_SOCKETS_SOCKOPT_TLS)
#include <zephyr/net/tls_credentials.h>
#define CA_CERTIFICATE_TAG 1
#endif

#include <zephyr/net/socket.h>
#include <memory>
#include <string.h>
#include <Arduino.h>
#include <api/String.h>
#include <api/IPAddress.h>

class ZephyrSocketWrapper {
protected:
	std::shared_ptr<int> sock_fd;
	bool is_ssl = false;
	int ssl_sock_temp_char = -1;

	// custom deleter for shared_ptr to close automatically the socket
	static void socket_deleter(int *fd) {
		if (fd && *fd != -1) {
			::close(*fd);
		}
		delete fd;
	}

public:
	ZephyrSocketWrapper() = default;

	ZephyrSocketWrapper(int fd);
	~ZephyrSocketWrapper() = default; // socket close managed by shared_ptr

	bool connect(const char *host, uint16_t port);
	bool connect(IPAddress host, uint16_t port);

#if defined(CONFIG_NET_SOCKETS_SOCKOPT_TLS)
	bool connectSSL(const char *host, uint16_t port, const char *ca_certificate_pem = nullptr);
#endif

	int available();
	int recv(uint8_t *buffer, size_t size, int flags = MSG_DONTWAIT);
	int send(const uint8_t *buffer, size_t size);
	void close() ;
	bool bind(uint16_t port);
	bool listen(int backlog = 5) ;
	int accept();

	String remoteIP();
	friend class ZephyrClient;
};
