#include "Thread.h"
#include "Condition.h"
#include "Mutex.h"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
using namespace std;
using namespace zcoco;
Mutex mtx;
Condition cv(mtx);

string flag("A");


void PrintA() {
    while(true) {
        while(flag == "B" || flag == "C") {
            cv.wait();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "A" << std::endl;
        flag = "B";
        cv.notifyAll();
    }
}

void PrintB() {
    while(true) {
        while(flag == "A" || flag == "C") {
            cv.wait();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "B" << std::endl;
        flag = "C";
        cv.notifyAll();
    }
}

void PrintC() {
    while(true) {
        while(flag == "B" || flag == "A") {
            cv.wait();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "C" << std::endl;
        flag = "A";
        cv.notifyAll();
    }
}

int main() {
    Thread t1(PrintA, "PrintA");
    Thread t2(PrintB, "PrintB");
    Thread t3(PrintC, "PrintC");
    t1.start();
    t2.start();
    t3.start();
    t1.join();
    t2.join();
    t3.join();
    std::cin.get();
}