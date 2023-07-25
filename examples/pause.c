#include <sigfn.h>

#include <stdio.h>

// to implement sleep_for()
#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

// signal handler
static void handle_signal(int signum, void *userdata);

// helper function to abstract platform dependent sleep functions
static void sleep_for(int ms);

int main()
{
    // used to store signum
    int flag;

    // initialize to what should be an impossible value for SIGINT
    flag = -1;

    // set callback for SIGINT
    sigfn_handle(SIGINT, handle_signal, &flag);
    
    // notify user that the program is "paused"
    puts("Paused. Press Ctrl+C to exit.");

    // sleep loop until we receive the signal
    while (flag < 0)
    {
        // this should stop the CPU from being overworked
        sleep_for(100);
    }

    // notify user that flag was set and exit cleanly
    printf("\nReceived signal: %d\n", flag);

    return 0;
}

void handle_signal(int signum, void *userdata)
{
    // set int that was passed by address
    *(int*)userdata = signum;
}

void sleep_for(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    const struct timespec duration = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1000000,
    };
    nanosleep(&duration, NULL);
#endif
}