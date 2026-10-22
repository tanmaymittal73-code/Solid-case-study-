#pragma once
#include <string>

// OCP: new pricing rules are NEW classes implementing this interface.
// Contract (needed for LSP): 0 <= finalAmount(s) <= s for any s >= 0.
class IPricingPolicy {
public:
    virtual ~IPricingPolicy() = default;
    virtual double finalAmount(double subtotal) const = 0;
    virtual std::string name() const = 0;
};
