#ifndef ITEM_H
#define ITEM_H

#include <string>

// Single inventory row. qoh is signed because negative quantities
// are the whole point of the negative-QOH analysis rule.
struct Item {
    std::string sku;
    std::string description;
    int         qoh            = 0;
    double      cost           = 0.0;
    double      retail         = 0.0;
    std::string lastSoldDate;     // "YYYY-MM-DD" or "" if unknown
    std::string lastReceivedDate; // "YYYY-MM-DD" or ""
    std::string status;           // "Active" | "Discontinued"

    double inventoryValueAtCost() const { return static_cast<double>(qoh) * cost; }
    double potentialLostMargin() const {
        return qoh > 0 ? qoh * (retail - cost) : 0.0;
    }
};

#endif
