#pragma once
#include "IPaymentMethod.h"

inline std::string makeReference(const std::string& prefix) {
    static int counter = 1000;
    return prefix + "-" + std::to_string(++counter);
}

class CashPayment : public IPaymentMethod {
public:
    PaymentResult pay(double amount) override {
        if (amount <= 0) return {false, "", "Invalid amount"};
        return {true, makeReference("CASH"), "Paid at counter"};
    }
    std::string name() const override { return "Cash"; }
};

class UpiPayment : public IPaymentMethod, public IRefundable {
public:
    explicit UpiPayment(std::string upiId) : upiId_(std::move(upiId)) {}
    PaymentResult pay(double amount) override {
        if (amount <= 0) return {false, "", "Invalid amount"};
        if (upiId_.find('@') == std::string::npos) return {false, "", "Invalid UPI id"};
        return {true, makeReference("UPI"), "Paid via " + upiId_};
    }
    bool refund(const std::string& reference, double amount) override {
        return !reference.empty() && amount > 0;
    }
    std::string name() const override { return "UPI"; }
private:
    std::string upiId_;
};

class CampusWalletPayment : public IPaymentMethod, public IRefundable {
public:
    explicit CampusWalletPayment(double balance) : balance_(balance) {}
    PaymentResult pay(double amount) override {
        if (amount <= 0) return {false, "", "Invalid amount"};
        if (amount > balance_) return {false, "", "Insufficient wallet balance"};
        balance_ -= amount;
        return {true, makeReference("WAL"), "Paid from campus wallet"};
    }
    bool refund(const std::string& reference, double amount) override {
        if (reference.empty() || amount <= 0) return false;
        balance_ += amount;
        return true;
    }
    std::string name() const override { return "Campus Wallet"; }
    double balance() const { return balance_; }
private:
    double balance_;
};
