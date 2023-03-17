#pragma once 
#include "Mutex.h"
#include "Condition.h"
#include "Noncopyable.h"
#include "Thread.h"

#include <memory>

namespace zcoco {
class EventLoopThreadPool;
class EventLoop;

class EventLoopThread :public Noncopyable {
public:
	typedef std::shared_ptr<EventLoopThread> Ptr;

	EventLoopThread(EventLoopThreadPool* scheduler);
	~EventLoopThread();

	EventLoop* startProcess();
	void join();

private:
	void threadFunc();
	Thread thread_;	
	EventLoopThreadPool* scheduler_;
	EventLoop* processer_;
	Mutex mutex_;
	Condition cond_;
};
}