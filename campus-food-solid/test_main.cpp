#include <cmath>
#include <iostream>
#include <memory>
#include "HappyHourPolicy.h"
#include "InMemoryMenu.h"
#include "InMemoryOrderRepository.h"
#include "OrderService.h"
#include "PaymentMethods.h"
#include "PricingPolicies.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::cout << "FAIL line " << __LINE__ << ": " #cond "\n"; ++failures; } } while (0)
static bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

// ---- Test doubles (possible only because of DIP) ----
struct FakeNotifier : INotifier {
    int count = 0; std::string last;
    void notify(const std::string&, const std::string& m) override { ++count; last = m; }
};
struct AlwaysFailPayment : IPaymentMethod {
    PaymentResult pay(double) override { return {false, "", "declined"}; }
    std::string name() const override { return "Fake"; }
};

int main() {
    // LSP: every pricing policy honours 0 <= final <= subtotal
    std::vector<std::unique_ptr<IPricingPolicy>> policies;
    policies.push_back(std::make_unique<NoDiscountPolicy>());
    policies.push_back(std::make_unique<PercentageDiscountPolicy>(10, "S"));
    policies.push_back(std::make_unique<FlatOffPolicy>(500, 50));
    policies.push_back(std::make_unique<HappyHourPolicy>());
    for (auto& p : policies) {
        for (double s : {0.0, 100.0, 600.0}) {
            double f = p->finalAmount(s);
            CHECK(f >= 0 && f <= s + 1e-9);
        }
    }
    CHECK(near(PercentageDiscountPolicy(10, "S").finalAmount(200), 180));
    CHECK(near(FlatOffPolicy(500, 50).finalAmount(600), 550));
    CHECK(near(FlatOffPolicy(500, 50).finalAmount(100), 100));

    // LSP: every payment method honours the contract (no throw, bad amount -> failure)
    CashPayment cash; UpiPayment upi("a@upi"); CampusWalletPayment wal(100);
    for (IPaymentMethod* m : std::vector<IPaymentMethod*>{&cash, &upi, &wal}) {
        CHECK(!m->pay(0).success);
        auto ok = m->pay(10);
        CHECK(ok.success && !ok.reference.empty());
    }
    CHECK(!wal.pay(1000).success);

    // DIP: OrderService with fakes
    InMemoryMenu menu; menu.add({1, "Dosa", 60});
    FakeNotifier note; InMemoryOrderRepository repo;
    NoDiscountPolicy none;
    OrderService svc(menu, none, note, repo);

    auto good = svc.placeOrder("t", {{1, 2}}, cash);
    CHECK(good.success && near(good.total, 120) && repo.all().size() == 1 && note.count == 1);

    AlwaysFailPayment bad;
    auto fail = svc.placeOrder("t", {{1, 1}}, bad);
    CHECK(!fail.success && repo.all().size() == 1);      // nothing saved

    CHECK(!svc.placeOrder("t", {{42, 1}}, cash).success); // unknown item
    CHECK(!svc.placeOrder("t", {{1, 0}}, cash).success);  // bad quantity
    CHECK(!svc.placeOrder("t", {}, cash).success);        // empty

    // OCP extension works through the unchanged service
    HappyHourPolicy hh;
    OrderService svc2(menu, hh, note, repo);
    CHECK(near(svc2.placeOrder("t", {{1, 5}}, cash).total, 240));

    // ISP: refund only on refundable methods
    CHECK(wal.refund("WAL-1", 10) && upi.refund("UPI-1", 10));

    std::cout << (failures ? "TESTS FAILED\n" : "ALL TESTS PASSED\n");
    return failures ? 1 : 0;
}
