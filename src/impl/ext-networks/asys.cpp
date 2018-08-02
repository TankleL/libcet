#include "../api_dev_mod.h"
#include "../../inc/logger.h"
#include "../../inc/ext-networks/asys.h"

using namespace Cet::Ext_Net;

#if defined( WIN32 )

void Receptionist::ignite()
{
	try
	{
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);

		struct addrinfo *result = nullptr , *ptr = nullptr, hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		std::ostringstream oss;
		oss << m_port;

		int res = 0;

		// Resolve the local address and port to be used by the server
		res = getaddrinfo(NULL, oss.str().c_str(), &hints, &result);
		if (res != 0) {
			LOG_K("getaddrinfo failed: %d\n", res);
			WSACleanup();
			throw std::exception("getaddrinfo failed");
		}

		// create listen socket
		cet_socket_t listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listen_sock == INVALID_SOCKET) {
			LOG_K("error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			throw std::exception("error at socket()");
		}

		// bind
		res = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
		if (res == SOCKET_ERROR) {
			LOG_K("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(listen_sock);
			WSACleanup();
			throw std::exception("error at bind()");
		}

		freeaddrinfo(result);

		if (listen(listen_sock, m_backlog) == SOCKET_ERROR) {
			LOG_K("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(listen_sock);
			WSACleanup();
			throw std::exception("error at listen()");
		}

		LOG_K("listening");

		LOG_K("end");

		WSACleanup();
	}
	catch (std::exception ex)
	{
		WSACleanup();
		LOG_E(ex.what());
	}
}
#endif