#include <iostream>
#include "HappyHourPolicy.h"          // <- the new requirement
#include "InMemoryMenu.h"
#include "InMemoryOrderRepository.h"
#include "Notifiers.h"
#include "OrderService.h"             // unchanged
#include "PaymentMethods.h"

int main() {
    InMemoryMenu menu;
    menu.add({1, "Masala Dosa", 60});
    ConsoleNotifier notifier;
    InMemoryOrderRepository repo;

    HappyHourPolicy happy;                               // only this line is new
    OrderService service(menu, happy, notifier, repo);   // same service, new behaviour

    CashPayment cash;
    auto r = service.placeOrder("asha", {{1, 5}}, cash);
    std::cout << r.message << "\n";
}
