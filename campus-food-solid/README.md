# Campus Food Ordering System (C++17, SOLID)

## Problem
A campus canteen needs an ordering system whose requirements keep growing: new discount
rules, new payment methods, new notification channels, and different user roles
(student vs. admin). The design must allow these changes with minimal edits to existing code.

## Features
- Admin adds/removes dishes (write-only view of the menu)
- Student views the menu and places orders (read-only view of the menu)
- Pluggable pricing policies: none, percentage (student 10%), flat-off on big orders
- Pluggable payments: Cash, UPI, Campus Wallet (UPI and Wallet support refunds)
- Pluggable notifications: Console, simulated Email, and a Multi (composite) notifier
- Pluggable storage via `IOrderRepository` (in-memory implementation provided)
- Receipt generation

## Project layout
```
include/      interfaces + implementations (header-only)
src/main.cpp  demo / composition root
tests/        assert-based tests with fake objects
extensions/   HappyHourPolicy.h  (the demonstrated extension)
examples/     extension_demo.cpp
```

## Build and run
Requires g++ with C++17.
```
make            # builds campus_food, extension_demo, run_tests
./campus_food   # main demo
./run_tests     # tests
./extension_demo
```
Or with CMake: `mkdir build && cd build && cmake .. && make && ctest`

## SOLID at a glance
| Principle | Where |
|---|---|
| SRP | `Order`, `ReceiptPrinter`, `OrderService`, `InMemoryOrderRepository` each have one job |
| OCP | add behaviour via new classes of `IPricingPolicy`, `IPaymentMethod`, `INotifier`, `IOrderRepository` |
| LSP | all payment methods and pricing policies obey the contracts in their interface headers (checked in tests) |
| ISP | `IMenuReader`/`IMenuWriter` split; `IRefundable` separate from `IPaymentMethod` |
| DIP | `OrderService` depends only on interfaces, injected in `main.cpp` |

## Demonstrated extension
New requirement: "Happy Hour 20% off". Added only `extensions/HappyHourPolicy.h`
(+ the demo in `examples/`). `OrderService` and all existing classes are unchanged.

## Sample output (excerpt)
```
------ RECEIPT #1 (asha) ------
  Masala Dosa x2  Rs 120.00
  Filter Coffee x2  Rs 50.00
  Subtotal : Rs 170.00
  Pricing  : Student 10% off
  TOTAL    : Rs 153.00
  Paid by  : UPI (UPI-1001)
```

## Contributors
| Name | Reg. No. | Contribution |
|---|---|---|
| (Student 1) | (Reg no.) | (e.g. pricing, payments, tests) |
| (Student 2) | (Reg no.) | (e.g. menu, notifications, repository, README) |
