#pragma once
#include "Noncopyable.h"
#include <pthread.h>
#include <iostream>
namespace zcoco {

template <typename T>
class Singleton : public Noncopyable {
public:
	static T* getInstance() {
		pthread_once(&once_control, once_init);
		return value_;
	}
	static void destroy() {
		delete value_;
	}

private:
    static void once_init() {
        value_ = new T();
        ::atexit(destroy);
    }
	Singleton();
	~Singleton();

	static T* value_;
	static pthread_once_t once_control;
};

template <typename T>
pthread_once_t Singleton<T>::once_control = PTHREAD_ONCE_INIT;

template <typename T>
T* Singleton<T>::value_;
}