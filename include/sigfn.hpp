#ifndef SIGFN_HPP
#define SIGFN_HPP

#include "sigfn.h"

#include <functional>
#include <string>
#include <unordered_map>

namespace sigfn
{
    typedef std::function<void(int)> handler_function;

    const std::string INVALID_SIGNUM = "sigfn: invalid signal code";
    const std::string INVALID_HANDLER = "sigfn: invalid signal handler";
        
    class exception final : public std::exception
    {
    private:
        std::string _error_message;
    public:
        exception(int status);
        const char* what() const noexcept override;
    };

    void handle(int signum, const handler_function &handler_function);

    void handle(int signum, handler_function &&handler_function);

    void ignore(int signum);

    void reset(int signum); 
}

#endif