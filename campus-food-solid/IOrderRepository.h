#pragma once
#include <optional>
#include <vector>
#include "Order.h"

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual void save(const Order& order) = 0;
    virtual std::optional<Order> findById(int id) const = 0;
    virtual std::vector<Order> all() const = 0;
};
