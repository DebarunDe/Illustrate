#include <iostream>
#include "lib/yahooAPI.h"

int main()
{
    printValue("Hello World");
    std::cout << AUTH_CODE << std::endl;
    std::cout << CLIENT_ID << std::endl;
    std::cout << CLIENT_SECRET << std::endl;
    std::cout << REDIRECT_URI << std::endl;
    getAccessToken(CLIENT_ID, CLIENT_SECRET, REDIRECT_URI,AUTH_CODE);

    return 0;
}