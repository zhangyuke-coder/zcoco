#include "EventLoop.h"
#include "Log.h"
#include "Hook.h"
#include "TimerManager.h"
#include "EventLoopThreadPool.h"

#include <assert.h>
#include <signal.h>

class IgnoreSigpipe {
public:
	IgnoreSigpipe() {
		::signal(SIGPIPE, SIG_IGN);
	}
};

IgnoreSigpipe signalObj;

namespace zcoco {

EventLoopThreadPool::EventLoopThreadPool(size_t thread_number)
	:thread_num_(thread_number),
	main_processer_(this),
	timer_manager_(new TimerManager()),
	thread_(std::bind(&EventLoopThreadPool::start, this)),
	cond_(mutex_),
	quit_cond_(mutex_),
	join_thread_(std::bind(&EventLoopThreadPool::joinThread, this)) {
	assert(thread_number > 0);
	assert(EventLoop::GetProcesserOfThisThread() == nullptr);

	//main_processer
	work_processers_.push_back(&main_processer_);
}

EventLoopThreadPool::~EventLoopThreadPool() {
	stop();
}

void EventLoopThreadPool::start() {
	if (running_) {
		return;
	}
	//work_thread
	for (size_t i = 0; i < thread_num_ - 1; ++i) {
		threads_.push_back(std::make_shared<EventLoopThread>(this));
	}

	//work_processer
	for (const EventLoopThread::Ptr& thread : threads_) {
		work_processers_.push_back(thread->startProcess());
	}

	//timer_thread
	timer_thread_ = std::make_shared<EventLoopThread>(this);
	//timer_processer
	timer_processer_ = timer_thread_->startProcess();
	timer_processer_->addTask([&]() {
						while (true) {
							timer_manager_->dealWithExpiredTimer();
						}
					}, "timer");
	{
		MutexGuard lock(mutex_);
		running_ = true;
	}
	cond_.notify();
	main_processer_.run();
}

void EventLoopThreadPool::startAsync() {
	if (running_) {
		return;
	}
	thread_.start();
	{
		MutexGuard lock(mutex_);
		while (!running_) {
			cond_.wait();
		}
	}
}

void EventLoopThreadPool::wait() {
	quit_cond_.wait();
}

void EventLoopThreadPool::stop() {
	if (!running_) 
		return;
	running_ = false;
	
	//main_processer
	main_processer_.stop();

	//work_processer
	for (auto processer : work_processers_) {
		processer->stop();
	}

	//timer_processer
	timer_processer_->stop();

	//如果stop在scheduler线程中调用,在新建的线程中join
	if (zcoco::isHookEnabled()) {
		join_thread_.start();
	} else {
		joinThread();
	}
}

void EventLoopThreadPool::joinThread() {
	if (thread_.isStarted()) {
		thread_.join();
	}

	for (auto thread : threads_) {
		thread->join();
	}
	timer_thread_->join();
	quit_cond_.notify();
}

void EventLoopThreadPool::addTask(Coroutine::Func task, std::string name) {
	EventLoop* picked = pickOneProcesser();	//thread-save

	assert(picked != nullptr);
	picked->addTask(task, name);	//thread-save
}

EventLoop* EventLoopThreadPool::pickOneProcesser() {
	MutexGuard lock(mutex_);
	static size_t index = 0;

	assert(index < work_processers_.size());
	EventLoop* picked = work_processers_[index++];
	index = index % work_processers_.size();

	return picked;
}

int64_t EventLoopThreadPool::runAt(Timestamp when, Coroutine::Ptr coroutine) {
	EventLoop* processer = EventLoop::GetProcesserOfThisThread();
	if (processer == nullptr) {
		processer = pickOneProcesser();
	}
	return timer_manager_->addTimer(when, coroutine, processer); //threa-save
}

int64_t EventLoopThreadPool::runAfter(uint64_t micro_delay, Coroutine::Ptr coroutine) {
	Timestamp when = Timestamp::now() + micro_delay;
	return runAt(when, coroutine);
}

int64_t EventLoopThreadPool::runEvery(uint64_t micro_interval, Coroutine::Ptr coroutine) {
	EventLoop* processer = EventLoop::GetProcesserOfThisThread();
	if (processer == nullptr) {
		processer = pickOneProcesser();
	}
	Timestamp when = Timestamp::now() + micro_interval;
	return timer_manager_->addTimer(when, coroutine, processer, micro_interval);
}


void EventLoopThreadPool::cancel(int64_t timer_id) {
	timer_manager_->cancel(timer_id);
}
}