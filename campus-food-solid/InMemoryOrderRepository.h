#pragma once
#include "IOrderRepository.h"

// Could be replaced by a FileOrderRepository / DatabaseOrderRepository
// without touching OrderService.
class InMemoryOrderRepository : public IOrderRepository {
public:
    void save(const Order& o) override { orders_.push_back(o); }
    std::optional<Order> findById(int id) const override {
        for (const auto& o : orders_) if (o.id() == id) return o;
        return std::nullopt;
    }
    std::vector<Order> all() const override { return orders_; }
private:
    std::vector<Order> orders_;
};
