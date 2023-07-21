#include <sigfn.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>

#define PASS 0
#define FAIL 1

#define INVALID_SIGNUM -1
#define INVALID_HANDLER NULL

struct unit_test
{
    const char *name;
    int (*func)();
};

static int test_main(const char *test_name);

static void echo_signum(int signum, void *userdata);

static int test_handle();

static int test_ignore();

static int test_reset();

int main(int argc, const char **argv)
{
    int result;
    if (argc > 1)
    {
        result = test_main(*(argv + 1));
    }
    return result;
}

void echo_signum(int signum, void *userdata)
{
    *(int *)userdata = signum;
}

int test_main(const char *test_name)
{
    const struct unit_test unit_tests[] = {
        {"HANDLE", test_handle},
        {"IGNORE", test_ignore},
        {"RESET", test_reset},
    };
    int result;
    int offset;
    int index;

    result = -1;
    offset = -1;

    for (index = 0; index < sizeof(unit_tests) / sizeof(*unit_tests); index++)
    {
        if (strcmp(test_name, (unit_tests + index)->name) == 0)
        {
            offset = index;
        }
    }

    if (offset != -1)
    {
        result = (unit_tests + offset)->func();
    }

    return result;
}

int test_handle()
{
    int result;
    int expected;
    int actual;
    int raised;

    result = PASS;

    expected = SIGFN_INVALID_SIGNUM;
    actual = sigfn_handle(INVALID_SIGNUM, echo_signum, NULL);
    result = (expected == actual) ? PASS : FAIL;

    if (result == PASS)
    {
        expected = SIGFN_INVALID_HANDLER;
        actual = sigfn_handle(SIGINT, INVALID_HANDLER, NULL);
        result = (expected == actual) ? PASS : FAIL;
    }

    if (result == PASS)
    {
        expected = SIGFN_SUCCESS;
        actual = sigfn_handle(SIGINT, echo_signum, &raised);
        result = (expected == actual) ? PASS : FAIL;
    }

    if (result == PASS)
    {
        expected = SIGINT;
        raise(SIGINT);
        actual = raised;
        result = (expected == actual) ? PASS : FAIL;
    }

    return result;
}

int test_ignore()
{
    int result;
    int expected;
    int actual;

    expected = SIGFN_INVALID_SIGNUM;
    actual = sigfn_ignore(INVALID_SIGNUM);
    result = (expected == actual) ? PASS : FAIL;

    if (result == PASS)
    {
        expected = SIGFN_SUCCESS;
        actual = sigfn_ignore(SIGINT);
        result = (expected == actual) ? PASS : FAIL;
    }

    return result;
}

int test_reset()
{
    
    return 0;
}
