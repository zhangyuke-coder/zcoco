#pragma once
#include "Coroutine.h"
#include "Mutex.h"
#include "Timestamp.h"

#include <atomic>
#include <map>
#include <memory>
#include <sys/timerfd.h>
#include <string.h>
#include <set>
#include <unistd.h>

namespace zcoco {

class EventLoop;
class Timer {
public:
	typedef std::shared_ptr<Timer> Ptr;
	Timer(Timestamp timestamp, EventLoop* processer, Coroutine::Ptr coroutine, uint64_t interval)
   		:timestamp_(timestamp),
		processer_(processer),
		coroutine_(coroutine),
		interval_(interval),
		sequence_(s_sequence_creator_++) {}

	void setTimestamp(Timestamp timestamp) { timestamp_ = timestamp; }
	Timestamp getTimestamp() { return timestamp_; }
	EventLoop* getProcesser() { return processer_; }
	Coroutine::Ptr getCoroutine() { return coroutine_; }
	void setCoroutine(Coroutine::Ptr coroutine) { coroutine_ = coroutine; }
	uint64_t getInterval() { return interval_; };
	int64_t  getSequence() { return sequence_; };
private:
	Timestamp timestamp_;
	EventLoop* processer_;
	Coroutine::Ptr coroutine_;
	uint64_t interval_;
	int64_t sequence_;
	static std::atomic<int64_t> s_sequence_creator_;
};

int createTimerFd();

class TimerManager {
friend class EventLoopThreadPool;
public:
	typedef std::function<void ()> Callback;
	TimerManager() 
		:timer_fd_(createTimerFd()) {}
	~TimerManager() {
		::close(timer_fd_);
	}
	
	int64_t addTimer(Timestamp when, Coroutine::Ptr coroutine, EventLoop* processer, uint64_t interval = 0);
	void cancel(int64_t);
private:
	bool findFirstTimestamp(const Timestamp&, Timestamp&);
	ssize_t readTimerFd();
	void resetTimerFd(Timestamp when);
	void dealWithExpiredTimer();

	int timer_fd_;
	std::multimap<Timestamp, Timer::Ptr> timer_map_;
	Mutex mutex_;

	//for cancel
	std::map<int64_t, Timestamp>  sequence_2_timestamp_;
	std::set<int64_t> 			  cancel_set_;
	
};

}