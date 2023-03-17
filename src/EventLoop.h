#pragma once
#include "Coroutine.h"
#include "Mutex.h"
#include "Noncopyable.h"
#include "Poller.h"
#include <list>

namespace zcoco {
class EventLoopThreadPool;

class EventLoop : public Noncopyable {
public:
    typedef std::shared_ptr<EventLoop> Ptr;

	EventLoop(EventLoopThreadPool* scheduler);
	virtual ~EventLoop() {}

	virtual void run();
	void stop();
	bool stoped() { return stop_; }
	size_t getLoad() { return load_; }
	EventLoopThreadPool* getScheduler() { return scheduler_; }
	void addTask(Coroutine::Ptr coroutine);
	void addTask(Coroutine::Func func, std::string name = "");
	void updateEvent(int fd, int events, Coroutine::Ptr coroutine = nullptr);
	void removeEvent(int fd);

	static EventLoop*& GetProcesserOfThisThread();

private:
	void wakeupPollCoroutine();
	ssize_t comsumeWakeEvent();

	bool stop_ = false;
	size_t load_ = 0;
	Mutex mutex_;
	EventLoopThreadPool* scheduler_;
	PollPoller poller_;
	int event_fd_;
	std::list<Coroutine::Ptr> coroutines_;
};
}