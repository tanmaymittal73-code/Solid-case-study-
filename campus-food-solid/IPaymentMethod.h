#pragma once
#include <string>

struct PaymentResult {
    bool success;
    std::string reference;
    std::string message;
};

// LSP contract for EVERY payment method:
//  - never throws; failure is reported via PaymentResult{success=false}
//  - accepts any amount, rejects amount <= 0 gracefully
//  - on success, reference is non-empty
class IPaymentMethod {
public:
    virtual ~IPaymentMethod() = default;
    virtual PaymentResult pay(double amount) = 0;
    virtual std::string name() const = 0;
};

// ISP: refund is a separate capability. Cash cannot refund, so Cash does not
// implement it (no empty/throwing refund() method forced onto it).
class IRefundable {
public:
    virtual ~IRefundable() = default;
    virtual bool refund(const std::string& reference, double amount) = 0;
};
