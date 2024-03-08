/*
** Copyright 2024 Maxtek Consulting
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

#include "sigfn.h"

#ifdef _WIN32
typedef void (*__sighandler_t)(int);
#endif

const int SIGFN_SIGNUMS[] = {
    SIGABRT,
    SIGFPE,
    SIGILL,
    SIGINT,
    SIGSEGV,
    SIGTERM,
};

#define SIGFN_SIGNUM_COUNT sizeof(SIGFN_SIGNUMS) / sizeof(SIGFN_SIGNUMS[0])

struct sigfn_mapping
{
    int signum;
    sigfn_handler_func handler_func;
    void *userdata;
};

static struct sigfn_mapping sifgn_mappings[SIGFN_SIGNUM_COUNT];

static void sigfn_handler(int signum);

static int sigfn_signal(int signum, __sighandler_t handler);

int sigfn_handle(int signum, sigfn_handler_func handler, void *userdata)
{
    int result;
    int index;
    struct sigfn_mapping *mapping;

    result = (handler != NULL) ? SIGFN_SUCCESS : SIGFN_INVALID_HANDLER;

    if (result == SIGFN_SUCCESS)
    {
        result = sigfn_signal(signum, sigfn_handler);
    }

    if (result == SIGFN_SUCCESS)
    {
        for (index = 0; index < SIGFN_SIGNUM_COUNT; index++)
        {
            if (SIGFN_SIGNUMS[index] == signum)
            {
                mapping = &sifgn_mappings[index];
                mapping->signum = signum;
                mapping->handler_func = handler;
                mapping->userdata = userdata;
            }
        }
    }

    return result;
}

int sigfn_ignore(int signum)
{
    return sigfn_signal(signum, SIG_IGN);
}

int sigfn_reset(int signum)
{
    return sigfn_signal(signum, SIG_DFL);
}

void sigfn_handler(int signum)
{
    struct sigfn_mapping *mapping;
    int mapping_index;

    mapping = NULL;

    for (mapping_index = 0; mapping_index < SIGFN_SIGNUM_COUNT; mapping_index++)
    {
        if (mapping == NULL && sifgn_mappings[mapping_index].signum == signum)
        {
            mapping = &sifgn_mappings[mapping_index];
        }
    }

    // skip NULL check because it is unreachable
    mapping->handler_func(mapping->signum, mapping->userdata);
}

int sigfn_signal(int signum, __sighandler_t handler)
{
    int result;

    result = SIGFN_SUCCESS;

    switch (signum)
    {
    case SIGABRT:
    case SIGFPE:
    case SIGILL:
    case SIGINT:
    case SIGSEGV:
    case SIGTERM:
        (void)signal(signum, handler);
        break;
    default:
        result = SIGFN_INVALID_SIGNUM;
        break;
    }
    return result;
}