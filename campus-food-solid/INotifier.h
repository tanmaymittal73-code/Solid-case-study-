#pragma once
#include <string>

class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void notify(const std::string& recipient, const std::string& message) = 0;
};
