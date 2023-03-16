#include "Log.h"
#include "Singleton.h"
#include "Address.h"

#include <iostream>
#include <string>
#include <memory>

#include <thread>

#include <chrono>
using namespace std;
using namespace zcoco;

int main() {
    IpAddress ip("127.0.0.1", 20);
    cout << ip.toString();

}