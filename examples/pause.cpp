/*
** Copyright 2024 John R. Patek Sr.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

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