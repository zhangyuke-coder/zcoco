#include "EventLoop.h"
#include "Hook.h"
#include "Log.h"

#include <errno.h>
#include <sys/eventfd.h>
#include <string.h>
#include <unistd.h>

namespace zcoco {

const int kPollTimeMs = 1000;

static int createEventFd() {
	int event_fd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
	if (event_fd == -1) {
		LOG_FATAL << "create eventfd failed, errno=" << errno << " error str:" << strerror(errno);
	}
	return event_fd;
}

EventLoop::EventLoop(EventLoopThreadPool* scheduler)
	:mutex_(), 
	scheduler_(scheduler),
	poller_(this),
	event_fd_(createEventFd()) {

	//当有新事件到来时唤醒poll协程
	addTask([&](){
				while (!stop_) {
					if (comsumeWakeEvent() < 0) {
						LOG_ERROR << "read eventfd:" << strerror(errno);
						break;
					}
				}
			}, "Wake");
}


void EventLoop::run() {
	if (GetProcesserOfThisThread() != nullptr) {
		LOG_FATAL << "run two EventLoop in one thread";
	} else {
		GetProcesserOfThisThread() = this;
	}
	zcoco::setHookEnabled(true);
	Coroutine::Ptr cur;

	//没有可以执行协程时调用poll协程
	Coroutine::Ptr poll_coroutine = std::make_shared<Coroutine>(std::bind(&Poller::poll, &poller_, kPollTimeMs), "Poll");

	while (!stop_) {
		{
			MutexGuard guard(mutex_);
			//没有协程时执行poll协程
			if (coroutines_.empty()) {
				cur = poll_coroutine;
				poller_.setPolling(true);
			} else {
				for (auto it = coroutines_.begin();
						it != coroutines_.end();
							++it) {
					cur = *it;
					coroutines_.erase(it);
					break;
				}
			}
		}
		cur->swapIn();
		if (cur->getState() == CoroutineState::TERMINATED) {
			load_--;
		}
	}
}

void EventLoop::addTask(Coroutine::Ptr coroutine) {
	MutexGuard guard(mutex_);
	coroutines_.push_back(coroutine);
	load_++;

	if (poller_.isPolling()) {
		wakeupPollCoroutine();
	}
}

void EventLoop::addTask(Coroutine::Func func, std::string name) {
	addTask(std::make_shared<Coroutine>(std::move(func), name));
}

void EventLoop::updateEvent(int fd, int events, Coroutine::Ptr coroutine) {
	poller_.updateEvent(fd, events, coroutine);
}
	
void EventLoop::removeEvent(int fd) {
	poller_.removeEvent(fd);
}

void EventLoop::stop() {
	stop_ = true;
	if (poller_.isPolling()) {
		wakeupPollCoroutine();
	}
}

void EventLoop::wakeupPollCoroutine() {
	uint64_t buffer = 1;
	ssize_t n = ::write(event_fd_, &buffer, sizeof buffer);
	if (n != sizeof buffer) {
		LOG_ERROR << "wakeupPollCoroutine() size of the supplied buffer is not 8 bytes";
	}
}

ssize_t EventLoop::comsumeWakeEvent() {
	uint64_t buffer = 1;
	ssize_t n = ::read(event_fd_, &buffer, sizeof buffer);
	if (n != sizeof buffer) {
		LOG_ERROR << "comsumeWakeEvent() size of the data is not 8 bytes";
	}
	return n;
}

EventLoop*& EventLoop::GetProcesserOfThisThread() {
	static __thread EventLoop* t_EventLoop = nullptr;
	return t_EventLoop;
}

}