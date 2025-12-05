#ifndef CUSTOMERBLACKLISTEDEXCEPTION_H
#define CUSTOMERBLACKLISTEDEXCEPTION_H

#include <stdexcept>
#include <string>

class CustomerBlacklistedException : public std::runtime_error {
public:
    explicit CustomerBlacklistedException(const std::string& message);
    virtual ~CustomerBlacklistedException() = default;
};

#endif