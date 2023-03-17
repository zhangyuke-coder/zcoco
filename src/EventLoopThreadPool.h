#pragma once 
#include "Coroutine.h"
#include "Mutex.h"
#include "Noncopyable.h"
#include "EventLoopThread.h"
#include "EventLoop.h"
#include "Singleton.h"
#include "Timestamp.h"

#include <vector>
namespace zcoco {

class TimerManager;

class EventLoopThreadPool : public Noncopyable {
friend class Singleton<EventLoopThreadPool>;
public:
	typedef std::shared_ptr<EventLoopThreadPool> Ptr;

	EventLoopThreadPool(size_t thread_number = 1);
	~EventLoopThreadPool();

	void start();
	void startAsync();
	void wait();
	void stop();
	void addTask(Coroutine::Func task, std::string name = "");
	int64_t runAt(Timestamp when, Coroutine::Ptr coroutine);
	int64_t runAfter(uint64_t micro_delay, Coroutine::Ptr coroutine);
	int64_t runEvery(uint64_t micro_interval, Coroutine::Ptr coroutine);
	void cancel(int64_t);

protected:
	EventLoop* pickOneProcesser();
private:
	void joinThread();
private:
	bool running_ = false;
	size_t thread_num_;
	EventLoop main_processer_;
	std::vector<EventLoop*> work_processers_;
	std::vector<EventLoopThread::Ptr> threads_;

	//单独一个线程处理定时任务
	EventLoop* timer_processer_;
	EventLoopThread::Ptr timer_thread_;
	std::unique_ptr<TimerManager> timer_manager_;

	Thread thread_;

	Mutex mutex_;
	Condition cond_;
	Condition quit_cond_;

	//for stop
	Thread join_thread_;
};
}