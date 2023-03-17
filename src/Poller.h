#pragma once 


#include "Noncopyable.h"
#include "Coroutine.h"

#include <map>
#include <memory>
#include <vector>
#include <poll.h>

namespace zcoco {
  
class EventLoop;

class Poller : public Noncopyable {
public:
	typedef std::shared_ptr<Poller> Ptr;
	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	Poller() = default;
	virtual ~Poller() {};
	virtual void updateEvent(int fd, int events, Coroutine::Ptr coroutine) = 0;
	virtual void removeEvent(int fd) = 0;

	virtual void poll(int timeout) = 0;

protected:
	std::string eventToString(int event);

};

class PollPoller : public Poller {
public:
	PollPoller(EventLoop* scheduler);

	void updateEvent(int fd, int events, Coroutine::Ptr coroutine) override;
	void removeEvent(int fd) override;

	void poll(int timeout) override;
	bool isPolling() { return is_polling_; }
	void setPolling(bool polling) { is_polling_ = polling; }
private:
	bool is_polling_;
	std::vector<struct pollfd> pollfds_;
	std::map<int, Coroutine::Ptr> fd_to_coroutine_;
	std::map<int, size_t> fd_to_index_;
	EventLoop* EventLoop_;
};  
}