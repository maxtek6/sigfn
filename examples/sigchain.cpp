#include <sigfn.hpp>

#include <iostream>

#include <vector>

static int parse_signum(const std::string &signum_string);

int main(int argc, const char **argv)
{
    std::vector<std::string> signum_strings;
    int signum;
    if (argc > 0)
    {
        while (signum_strings.size() < (argc - 1))
        {
            signum_strings.push_back(*(argv + signum_strings.size() + 1));
        }
        try
        {
            for (const std::string &signum_string : signum_strings)
            {
                signum = parse_signum(signum_string);
                sigfn::handle(
                    signum,
                    [](int signum)
                    {
                        std::cout << "received signum: " << signum << std::endl;
                    });
                std::cout << "sending signal: " << signum_string << std::endl;
                raise(signum);
                sigfn::reset(signum);
            }
        }
        catch (const std::exception &exception)
        {
            std::cerr << exception.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Usage: sigchain SIGNUM [SIGNUM]..." << std::endl;
    }
    return 0;
}

int parse_signum(const std::string &signum_string)
{
    const std::unordered_map<std::string, int> signum_values = {
        {"SIGABRT", SIGABRT},
        {"SIGFPE", SIGFPE},
        {"SIGILL", SIGILL},
        {"SIGINT", SIGINT},
        {"SIGSEGV", SIGSEGV},
        {"SIGTERM", SIGTERM},
    };
    int result(-1);

    auto signum_value = signum_values.find(signum_string);
    if (signum_value != signum_values.end())
    {
        result = signum_value->second;
    }

    return result;
}