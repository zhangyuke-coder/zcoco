#pragma once

#include "TcpConnection.h"
namespace zcoco {

class TcpClient {
public:
	TcpClient(const IpAddress& server_addr);
	virtual ~TcpClient() {}

	TcpConnection::Ptr connect();

private:
	static const int kMaxRetryDelayMs = 15 * 1000;
	static const int kInitRetryDelayMs = 500;

	IpAddress server_addr_;
	int retry_delay_ms_ = kInitRetryDelayMs;
};
  
}