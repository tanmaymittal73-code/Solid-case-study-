#pragma once
#include <algorithm>
#include "IPricingPolicy.h"

class NoDiscountPolicy : public IPricingPolicy {
public:
    double finalAmount(double s) const override { return s; }
    std::string name() const override { return "No discount"; }
};

class PercentageDiscountPolicy : public IPricingPolicy {
public:
    PercentageDiscountPolicy(double percent, std::string label)
        : percent_(std::clamp(percent, 0.0, 100.0)), label_(std::move(label)) {}
    double finalAmount(double s) const override { return s * (1.0 - percent_ / 100.0); }
    std::string name() const override { return label_; }
private:
    double percent_;
    std::string label_;
};

class FlatOffPolicy : public IPricingPolicy {
public:
    FlatOffPolicy(double minSpend, double off) : minSpend_(minSpend), off_(off) {}
    double finalAmount(double s) const override {
        return (s >= minSpend_) ? std::max(0.0, s - off_) : s;
    }
    std::string name() const override { return "Flat off on big orders"; }
private:
    double minSpend_, off_;
};
