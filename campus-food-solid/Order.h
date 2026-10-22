#pragma once
#include <string>
#include <vector>
#include "MenuItem.h"

struct OrderLine {
    MenuItem item;
    int qty;
};

// SRP: Order only stores order data and its raw subtotal.
// It does NOT know about discounts, payment, printing or storage.
class Order {
public:
    Order(int id, std::string customer) : id_(id), customer_(std::move(customer)) {}

    void addLine(const MenuItem& item, int qty) { lines_.push_back({item, qty}); }

    int id() const { return id_; }
    const std::string& customer() const { return customer_; }
    const std::vector<OrderLine>& lines() const { return lines_; }

    double subtotal() const {
        double s = 0;
        for (const auto& l : lines_) s += l.item.price * l.qty;
        return s;
    }

private:
    int id_;
    std::string customer_;
    std::vector<OrderLine> lines_;
};
