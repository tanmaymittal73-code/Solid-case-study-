#pragma once
#include <optional>
#include <vector>
#include "MenuItem.h"

// ISP: two small, role-specific interfaces instead of one fat "IMenu".
// Students only need to READ the menu; admins need to WRITE it.
class IMenuReader {
public:
    virtual ~IMenuReader() = default;
    virtual std::vector<MenuItem> items() const = 0;
    virtual std::optional<MenuItem> find(int id) const = 0;
};

class IMenuWriter {
public:
    virtual ~IMenuWriter() = default;
    virtual void add(const MenuItem& item) = 0;
    virtual bool remove(int id) = 0;
};
