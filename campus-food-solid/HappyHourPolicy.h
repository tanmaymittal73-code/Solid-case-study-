#pragma once
#include "IPricingPolicy.h"

// EXTENSION: a brand-new pricing rule. Added as a NEW file only.
// No existing file (OrderService, Order, other policies) was edited.
class HappyHourPolicy : public IPricingPolicy {
public:
    double finalAmount(double s) const override { return s * 0.80; }   // 20% off
    std::string name() const override { return "Happy Hour 20% off"; }
};
