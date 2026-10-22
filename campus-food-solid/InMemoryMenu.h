#pragma once
#include <map>
#include "IMenu.h"

// One concrete class may implement both roles; clients only see the role they need.
class InMemoryMenu : public IMenuReader, public IMenuWriter {
public:
    std::vector<MenuItem> items() const override {
        std::vector<MenuItem> out;
        for (const auto& kv : data_) out.push_back(kv.second);
        return out;
    }
    std::optional<MenuItem> find(int id) const override {
        auto it = data_.find(id);
        if (it == data_.end()) return std::nullopt;
        return it->second;
    }
    void add(const MenuItem& item) override { data_[item.id] = item; }
    bool remove(int id) override { return data_.erase(id) > 0; }

private:
    std::map<int, MenuItem> data_;
};
