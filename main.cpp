#include <iostream>
#include <string>
#include "lib/oauth2.h"
#include "lib/yahooAPI.h"


int main()
{
    User user;
    std::cout << "Logging in..." << std::endl;
    user.login();
    std::cout << "Logged in." << std::endl;
    bool running = true;
    while (running) {
        std::string command;
        std::cout << "Enter command: ";
        std::cin >> command;
        if (command == "exit") {
            running = false;
        } else {
        }
    }
    return 0;
}