#pragma once
#include "IMenu.h"

// Admin role: depends only on IMenuWriter (ISP). It cannot even see read-side APIs.
class AdminService {
public:
    explicit AdminService(IMenuWriter& menu) : menu_(menu) {}
    void addDish(int id, const std::string& name, double price) { menu_.add({id, name, price}); }
    bool removeDish(int id) { return menu_.remove(id); }
private:
    IMenuWriter& menu_;
};
