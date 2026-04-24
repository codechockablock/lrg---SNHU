#include "InventoryLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// Split a CSV line on commas. Handles simple double-quoted fields
// containing commas (e.g. a description like "Screw, 2-1/2\"").
std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    fields.push_back(current);
    return fields;
}

int safeStoi(const std::string& s) {
    try { return std::stoi(s); } catch (...) { return 0; }
}
double safeStod(const std::string& s) {
    try { return std::stod(s); } catch (...) { return 0.0; }
}

} // namespace

namespace InventoryLoader {

std::vector<Item> loadFromCsv(const std::string& path) {
    std::vector<Item> items;
    std::ifstream file(path);
    if (!file.is_open()) return items;

    std::string line;
    bool headerConsumed = false;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;
        if (line.empty()) continue;
        if (!headerConsumed) { headerConsumed = true; continue; }

        auto fields = splitCsvLine(line);
        if (fields.size() < 8) {
            std::cerr << "Warning: skipping malformed row at line "
                      << lineNumber << " (got " << fields.size()
                      << " fields, expected 8)\n";
            continue;
        }

        Item item;
        item.sku              = fields[0];
        item.description      = fields[1];
        item.qoh              = safeStoi(fields[2]);
        item.cost             = safeStod(fields[3]);
        item.retail           = safeStod(fields[4]);
        item.lastSoldDate     = fields[5];
        item.lastReceivedDate = fields[6];
        item.status           = fields[7];
        items.push_back(item);
    }

    return items;
}

std::vector<Item> buildFallbackSample() {
    // Mirrors generate_sample_data() in the Python source so the
    // analysis finds the same categories: discontinued/replacement
    // pairs, big negative lumber QOH, dead stock, and normal items.
    return {
        {"OLD-1001", "Old Deck Screw 2-1/2\" (Discontinued)",       -120,   0.15,   0.39, "2023-08-20", "2024-01-15", "Discontinued"},
        {"NEW-1001", "Spax Deck Screw 2-1/2\" (Replacement)",        1200,   0.22,   0.49, "2025-12-10", "2025-12-01", "Active"},
        {"OLD-2002", "Old Structural Bolt 1/2x6 (Discontinued)",     -80,   2.80,   5.99, "2023-05-10", "2023-11-20", "Discontinued"},
        {"NEW-2002", "Simpson SDWS Framing Screw (Replacement)",      850,   3.50,   7.49, "2025-12-05", "2025-12-03", "Active"},
        {"OLD-3003", "Old Concrete Anchor Kit (Discontinued)",        -45,  28.00,  54.99, "2022-12-01", "2023-07-10", "Discontinued"},
        {"NEW-3003", "Tapcon Pro Kit 1/4x3-1/4\" (Replacement)",      420,  35.00,  64.99, "2025-11-28", "2025-12-02", "Active"},

        {"MW-4001",  "Midwest Stainless Hex Bolts Assortment",         65,  72.00, 129.99, "2025-10-15", "2025-11-10", "Active"},
        {"GRK-5001", "GRK RSS Structural Screws 3-1/8\" (100pk)",     320,  58.50,  94.99, "2025-11-01", "2025-12-04", "Active"},
        {"SPAX-6001","Spax PowerLag 5/16x6\" (50pk)",                 220,  68.00, 109.99, "2025-10-05", "2025-11-25", "Active"},

        {"LUM-7001", "2x4x8 SPF Stud",                             -11033,   4.50,   8.99, "2025-12-01", "2025-11-28", "Active"},
        {"LUM-7002", "4x4x8 Treated Post",                            -850,  12.80,  24.99, "2025-11-10", "2025-11-20", "Active"},
        {"LUM-7003", "1/2\" Plywood Sheathing 4x8",                   -220,  32.00,  59.99, "2025-09-15", "2025-10-01", "Active"},

        {"PAINT-8001","Krylon Fusion Spray Paint Gloss Black",         85,   6.80,  12.99, "2025-12-12", "2025-12-08", "Active"},
        {"ELEC-8002","Leviton Decora Outlet White (10pk)",            140,   8.50,  16.99, "2025-12-10", "2025-12-05", "Active"},

        {"DEAD-10001","Obsolete LED Bulb 40W Equivalent (Old Model)", 150,   4.20,   9.99, "2022-03-15", "2022-06-10", "Active"},
        {"DEAD-10002","Discontinued Brass Ball Valve 3/4\"",           80,  18.50,  34.99, "2021-11-20", "2022-02-05", "Active"},
        {"DEAD-10003","Old Style Door Knob Set (Oil Rubbed Bronze)",   55,  22.00,  44.99, "2022-01-10", "2022-04-18", "Active"},
        {"DEAD-10004","Vintage Halogen Work Light 500W",               30,  25.00,  49.99, "2020-12-05", "2021-03-12", "Active"},
        {"DEAD-10005","Old Packaging Duct Tape (Gray)",               200,   3.80,   7.99, "2022-07-01", "2022-09-15", "Active"},
    };
}

} // namespace InventoryLoader
