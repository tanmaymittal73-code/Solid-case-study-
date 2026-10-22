#pragma once
#include <iomanip>
#include <sstream>
#include "Order.h"

// SRP: the only reason this class changes is a change in receipt FORMAT.
class ReceiptPrinter {
public:
    static std::string render(const Order& o, double total,
                              const std::string& pricingName,
                              const std::string& paymentName,
                              const std::string& paymentRef) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2);
        out << "------ RECEIPT #" << o.id() << " (" << o.customer() << ") ------\n";
        for (const auto& l : o.lines())
            out << "  " << l.item.name << " x" << l.qty << "  Rs " << l.item.price * l.qty << "\n";
        out << "  Subtotal : Rs " << o.subtotal() << "\n";
        out << "  Pricing  : " << pricingName << "\n";
        out << "  TOTAL    : Rs " << total << "\n";
        out << "  Paid by  : " << paymentName << " (" << paymentRef << ")\n";
        out << "---------------------------------------";
        return out.str();
    }
};
