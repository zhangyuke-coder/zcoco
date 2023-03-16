#pragma once
#include "Mutex.h"
#include "Noncopyable.h"
#include <pthread.h>

namespace zcoco {

class Condition : public Noncopyable {
public:
	explicit Condition(Mutex& mutex);
	~Condition();

	void wait();
	bool wait_seconds(time_t seconds);
	void notify();
	void notifyAll();
	
private:
	Mutex& mutex_;
	pthread_cond_t cond_;
};

}