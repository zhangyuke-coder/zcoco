#include "Thread.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace zcoco;
void test1() {
    for(int i = 0; i < 10; i++) {
        cout << i << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}
int main() {
    Thread t1(test1, "test1");
    t1.start();
    
    t1.join();
}