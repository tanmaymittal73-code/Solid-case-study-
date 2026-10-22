#include <iostream>
#include <memory>
#include "AdminService.h"
#include "InMemoryMenu.h"
#include "InMemoryOrderRepository.h"
#include "Notifiers.h"
#include "OrderService.h"
#include "PaymentMethods.h"
#include "PricingPolicies.h"

// Student view: receives only the READ interface.
void showMenu(const IMenuReader& menu) {
    std::cout << "=== Campus Menu ===\n";
    for (const auto& m : menu.items())
        std::cout << "  " << m.id << ". " << m.name << "  Rs " << m.price << "\n";
}

// Only refundable payment methods can be passed here (compile-time guarantee).
void refund(IRefundable& method, const std::string& ref, double amt) {
    std::cout << (method.refund(ref, amt) ? "Refund OK\n" : "Refund rejected\n");
}

int main() {
    // ---- Composition root: the ONLY place concrete classes are chosen ----
    InMemoryMenu menu;
    AdminService admin(menu);                         // gets IMenuWriter
    admin.addDish(1, "Masala Dosa", 60);
    admin.addDish(2, "Veg Biryani", 120);
    admin.addDish(3, "Filter Coffee", 25);
    admin.addDish(4, "Paneer Wrap", 90);

    auto notifier = std::make_shared<MultiNotifier>();
    notifier->add(std::make_shared<ConsoleNotifier>());
    notifier->add(std::make_shared<EmailNotifier>());

    InMemoryOrderRepository repo;
    PercentageDiscountPolicy studentPricing(10, "Student 10% off");
    OrderService service(menu, studentPricing, *notifier, repo);   // DIP: injection

    showMenu(menu);

    std::cout << "\n--- 1. UPI payment ---\n";
    UpiPayment upi("asha@upi");
    auto r1 = service.placeOrder("asha", {{1, 2}, {3, 2}}, upi);
    std::cout << r1.message << "\n";

    std::cout << "\n--- 2. Wallet payment (enough balance) ---\n";
    CampusWalletPayment wallet(200);
    auto r2 = service.placeOrder("ravi", {{2, 1}}, wallet);
    std::cout << r2.message << "\nWallet left: Rs " << wallet.balance() << "\n";

    std::cout << "\n--- 3. Wallet payment (insufficient) -> graceful failure ---\n";
    auto r3 = service.placeOrder("ravi", {{2, 3}}, wallet);
    std::cout << r3.message << "\n";

    std::cout << "\n--- 4. Cash payment ---\n";
    CashPayment cash;
    auto r4 = service.placeOrder("meena", {{4, 1}}, cash);
    std::cout << r4.message << "\n";

    std::cout << "\n--- 5. Invalid item ---\n";
    service.placeOrder("meena", {{99, 1}}, cash);

    std::cout << "\n--- 6. Refund (only refundable methods compile) ---\n";
    refund(wallet, "WAL-1002", 108);
    // refund(cash, "x", 1);   // <- will NOT compile: CashPayment is not IRefundable
    std::cout << "Wallet after refund: Rs " << wallet.balance() << "\n";

    std::cout << "\nTotal orders stored: " << repo.all().size() << "\n";
    return 0;
}
