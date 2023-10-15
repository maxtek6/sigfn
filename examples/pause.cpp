#include <sigfn.hpp>
#include <iostream>
#include <thread>

int main(int argc, const char **argv)
{
    bool loop;
    // set callback for SIGINT
    sigfn::handle(SIGINT, [&](int signum) {
        std::cout << std::endl << "Received signal: " << signum << std::endl;
        loop = false;
    });
    
    // notify user that the program is "paused"
    std::cout << "Paused. Press Ctrl+C to exit." << std::endl;
    
    // sleep loop until we receive the signal
    loop = true;
    while(loop)
    {
        // sleep for 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}