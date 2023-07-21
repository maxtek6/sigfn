#ifndef SIGFN_H
#define SIGFN_H

#define SIGFN_SUCCESS 0
#define SIGFN_INVALID_SIGNUM 1
#define SIGFN_INVALID_HANDLER 2

typedef void (*sigfn_handler_func)(int signum, void *userdata);

int sigfn_handle(int signum, sigfn_handler_func handler, void *userdata);

int sigfn_ignore(int signum);

int sigfn_reset(int signum);

#endif