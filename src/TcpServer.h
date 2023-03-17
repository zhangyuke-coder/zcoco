#pragma once 
#include "Address.h"
#include "Noncopyable.h"
#include "Socket.h"
#include "TcpConnection.h"
#include <functional>
#include <vector>

namespace zcoco {

class EventLoopThreadPool;

class TcpServer : public Noncopyable {
public:
	typedef std::function<void (TcpConnection::Ptr)> ConnectionHanlder;
	TcpServer(const IpAddress& listen_addr, EventLoopThreadPool* scheduler);
	~TcpServer() {}
	
	void start();
	void setConnectionHandler(ConnectionHanlder&& handler);

private:
	void startAccept();

	IpAddress listen_addr_;
	Socket::Ptr listen_socket_;
	EventLoopThreadPool* scheduler_;
	ConnectionHanlder connection_handler_;
};

void defualtHandler(TcpConnection::Ptr connection);
}