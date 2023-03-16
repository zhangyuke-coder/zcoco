#pragma once 

#include "Noncopyable.h"
#include "Condition.h"

namespace zcoco {
class CountDownLatch : public Noncopyable {
public:
	CountDownLatch(int count);

	void wait();
	void countDown();
private:
	int count_;
	Mutex mutex_;
	Condition cond_;
};

}