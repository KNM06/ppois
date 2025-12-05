#ifndef SEASONALDEMANDEXCEPTION_H
#define SEASONALDEMANDEXCEPTION_H

#include <stdexcept>
#include <string>

class SeasonalDemandException : public std::runtime_error {
public:
    explicit SeasonalDemandException(const std::string& message);
    virtual ~SeasonalDemandException();
};

#endif