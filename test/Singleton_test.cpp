#include "Singleton.h"
#include <iostream>
using namespace std;
using namespace zcoco;
class A {

public:
    void print() {
        cout << "A" << endl;
    }
};
int main() {
    auto a = Singleton<A>::getInstance();
    a->print();
}