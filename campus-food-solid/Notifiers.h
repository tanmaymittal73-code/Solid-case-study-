#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "INotifier.h"

class ConsoleNotifier : public INotifier {
public:
    void notify(const std::string& to, const std::string& msg) override {
        std::cout << "[CONSOLE -> " << to << "] " << msg << "\n";
    }
};

class EmailNotifier : public INotifier {   // simulated
public:
    void notify(const std::string& to, const std::string& msg) override {
        std::cout << "[EMAIL   -> " << to << "@campus.edu] " << msg << "\n";
    }
};

// Composite: sends through many notifiers. OrderService still sees ONE INotifier.
class MultiNotifier : public INotifier {
public:
    void add(std::shared_ptr<INotifier> n) { list_.push_back(std::move(n)); }
    void notify(const std::string& to, const std::string& msg) override {
        for (auto& n : list_) n->notify(to, msg);
    }
private:
    std::vector<std::shared_ptr<INotifier>> list_;
};
