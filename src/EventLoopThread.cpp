#include "EventLoop.h"
#include "Log.h"
#include "EventLoopThread.h"

#include <assert.h>

namespace zcoco {
EventLoopThread::EventLoopThread(EventLoopThreadPool* scheduler) 
	:thread_(std::bind(&EventLoopThread::threadFunc, this)),
	scheduler_(scheduler),
	processer_(nullptr),
	mutex_(),
	cond_(mutex_) {

}


EventLoopThread::~EventLoopThread() {
}

EventLoop* EventLoopThread::startProcess() {
	thread_.start();

	MutexGuard guard(mutex_);
	while (processer_ == nullptr) {
		cond_.wait();
	}
	assert(processer_ != nullptr);
	return processer_;
}
void EventLoopThread::join() {
	thread_.join();
}

void EventLoopThread::threadFunc() {
	EventLoop processer(scheduler_);

	{
		MutexGuard guard(mutex_);
		processer_ = &processer;
		cond_.notify();
	}

	processer.run();
}

}

