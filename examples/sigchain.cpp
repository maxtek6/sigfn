#include <sigfn.hpp>

#include <iostream>

#include <vector>

static void parse_signums(int argc, const char **argv, std::vector<int> &signums);

int main(int argc, const char **argv)
{
    std::vector<int> signums;

    parse_signums(argc, argv, signums);

    for (int signum : signums)
    {
        sigfn::handle(
            signum,
            [](int signum)
            {
                std::cout << "received signum: " << signum << std::endl;
            });
        std::cout << "sending signal: " << signum << std::endl;
        raise(signum);
        sigfn::reset(signum);
    }

    return 0;
}

void parse_signums(int argc, const char **argv, std::vector<int> &signums)
{
    const std::unordered_map<std::string, int> signum_values = {
        {"SIGABRT", SIGABRT},
        {"SIGFPE", SIGFPE},
        {"SIGILL", SIGILL},
        {"SIGINT", SIGINT},
        {"SIGSEGV", SIGSEGV},
        {"SIGTERM", SIGTERM},
    };
    int signum;
    std::string signum_string;
    std::unordered_map<std::string, int>::const_iterator signum_values_iterator;

    if (argc > 1)
    {
        signums.reserve(argc - 1);
        for (int arg_index = 1; arg_index < argc; arg_index++)
        {
            signum_string = *(argv + arg_index);
            signum_values_iterator = signum_values.find(signum_string);
            if (signum_values_iterator != signum_values.cend())
            {
                signum = signum_values_iterator->second;
                std::cerr << "adding " << signum_string << "(" << signum << ") to sigchain" << std::endl;
                signums.push_back(signum);
            }
            else
            {
                std::cerr << "skipping invalid signal \"" << signum_string << "\"" << std::endl;
            }
        }
    }
}