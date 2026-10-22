#pragma once
#include <utility>
#include <vector>
#include "IMenu.h"
#include "IPricingPolicy.h"
#include "IPaymentMethod.h"
#include "INotifier.h"
#include "IOrderRepository.h"
#include "ReceiptPrinter.h"

struct OrderOutcome {
    bool success;
    int orderId;
    double total;
    std::string message;   // receipt on success, reason on failure
};

// DIP: this high-level class depends ONLY on abstractions (interfaces).
// Concrete classes are chosen and injected from outside (main / tests).
// OCP: new pricing / payment / notifier / storage never require editing this file.
// SRP: it only orchestrates the "place order" workflow.
class OrderService {
public:
    OrderService(const IMenuReader& menu, const IPricingPolicy& pricing,
                 INotifier& notifier, IOrderRepository& repo)
        : menu_(menu), pricing_(pricing), notifier_(notifier), repo_(repo) {}

    // requests = list of (menuItemId, quantity)
    OrderOutcome placeOrder(const std::string& customer,
                            const std::vector<std::pair<int, int>>& requests,
                            IPaymentMethod& payment) {
        Order order(nextId_, customer);
        for (const auto& [id, qty] : requests) {
            auto item = menu_.find(id);
            if (!item)  return fail(customer, "Item " + std::to_string(id) + " not on menu");
            if (qty <= 0) return fail(customer, "Invalid quantity for item " + std::to_string(id));
            order.addLine(*item, qty);
        }
        if (order.lines().empty()) return fail(customer, "Empty order");

        double total = pricing_.finalAmount(order.subtotal());
        PaymentResult pr = payment.pay(total);
        if (!pr.success) return fail(customer, "Payment failed: " + pr.message);

        repo_.save(order);
        std::string receipt = ReceiptPrinter::render(order, total, pricing_.name(),
                                                     payment.name(), pr.reference);
        notifier_.notify(customer, "Order #" + std::to_string(order.id()) + " confirmed.");
        return {true, nextId_++, total, receipt};
    }

private:
    OrderOutcome fail(const std::string& customer, const std::string& why) {
        notifier_.notify(customer, "Order failed: " + why);
        return {false, -1, 0.0, why};
    }

    const IMenuReader& menu_;          // read-only view of the menu (ISP)
    const IPricingPolicy& pricing_;
    INotifier& notifier_;
    IOrderRepository& repo_;
    int nextId_ = 1;
};
