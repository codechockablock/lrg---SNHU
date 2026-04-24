#include "ReportPrinter.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace ReportPrinter {

namespace {

// Try en_US first (most reliable for thousands separators on macOS/Linux),
// fall back to the environment locale, then to C if neither loads.
std::locale pickGroupingLocale() {
    for (const char* name : {"en_US.UTF-8", "en_US", ""}) {
        try { return std::locale(name); } catch (...) {}
    }
    return std::locale::classic();
}

const std::locale& groupingLocale() {
    static const std::locale loc = pickGroupingLocale();
    return loc;
}

// Returns a string like "$1,234.56". Currency symbol is kept as a
// literal "$"; only the number grouping comes from the locale.
std::string money(double value) {
    std::ostringstream oss;
    oss.imbue(groupingLocale());
    oss << std::fixed << std::setprecision(2) << value;
    return "$" + oss.str();
}

std::string groupedInt(long value) {
    std::ostringstream oss;
    oss.imbue(groupingLocale());
    oss << value;
    return oss.str();
}

void printRule(char c, int n) {
    std::cout << std::string(static_cast<std::size_t>(n), c) << '\n';
}

std::string truncate(const std::string& s, std::size_t n) {
    if (s.size() <= n) return s;
    return s.substr(0, n - 1) + "…";
}

} // namespace

void printHeader(std::size_t totalItems) {
    std::cout << "\nPIKE CREEK COMMUNITY HARDWARE - INVENTORY HEALTH REPORT\n";
    printRule('=', 70);
    std::cout << "Report Date:          " << HealthAnalyzer::kReportDate << "\n";
    std::cout << "Total Items Analyzed: " << groupedInt(static_cast<long>(totalItems)) << "\n\n";
}

void printNegativeQoh(const std::vector<Item>& negatives) {
    std::cout << "2. NEGATIVE QUANTITY ON HAND\n";
    printRule('-', 60);
    if (negatives.empty()) {
        std::cout << "No negative QOH found.\n\n";
        return;
    }

    long totalMissing = 0;
    double shrinkCost = 0.0;
    const Item* worst = &negatives.front();
    for (const auto& it : negatives) {
        totalMissing += it.qoh;
        shrinkCost   += std::abs(it.inventoryValueAtCost());
        if (it.qoh < worst->qoh) worst = &it;
    }

    std::cout << "Items with negative QOH:        " << groupedInt(static_cast<long>(negatives.size())) << "\n";
    std::cout << "Most negative:                  " << truncate(worst->description, 50)
              << " (QOH: " << groupedInt(worst->qoh) << ")\n";
    std::cout << "Total missing units:            " << groupedInt(totalMissing) << "\n";
    std::cout << "Estimated unrecorded shrink:    " << money(shrinkCost) << "\n\n";
}

void printDeadStock(const std::vector<Item>& deadStock) {
    std::cout << "3. DEAD STOCK (No sales in 18+ months)\n";
    printRule('-', 60);
    if (deadStock.empty()) {
        std::cout << "No dead stock detected.\n\n";
        return;
    }

    std::cout << "Dead stock items: " << deadStock.size() << "\n\n";
    std::cout << std::left
              << std::setw(12) << "SKU"
              << std::setw(42) << "Description"
              << std::right
              << std::setw(8)  << "QOH"
              << std::setw(10) << "Days"
              << std::setw(14) << "Value"
              << "\n";
    printRule('-', 86);

    double total = 0.0;
    std::size_t shown = 0;
    for (const auto& it : deadStock) {
        if (shown++ >= 15) break;
        long days  = HealthAnalyzer::daysBetween(it.lastSoldDate, HealthAnalyzer::kReportDate);
        double val = it.inventoryValueAtCost();
        total += val;
        std::cout << std::left
                  << std::setw(12) << truncate(it.sku, 11)
                  << std::setw(42) << truncate(it.description, 41)
                  << std::right
                  << std::setw(8)  << it.qoh
                  << std::setw(10) << days
                  << std::setw(14) << money(val)
                  << "\n";
    }
    // If 15-row cap hid some rows, the total above is partial; sum the rest so
    // the printed total matches the full dead-stock dollar figure.
    for (std::size_t i = 15; i < deadStock.size(); ++i) {
        total += deadStock[i].inventoryValueAtCost();
    }

    std::cout << "\nTotal cost tied up in dead stock: " << money(total) << "\n";
    std::cout << "Recommendation: Clearance pricing, donation, or write-off.\n\n";
}

void printReplacementOverstock(const std::vector<Item>& overstock) {
    std::cout << "1. REPLACEMENT SKU OVERSTOCK (from discontinued substitution)\n";
    printRule('-', 60);
    if (overstock.empty()) {
        std::cout << "No severe replacement overstock detected.\n\n";
        return;
    }

    std::cout << std::left
              << std::setw(12) << "SKU"
              << std::setw(46) << "Description"
              << std::right
              << std::setw(10) << "QOH"
              << std::setw(14) << "Value"
              << "\n";
    printRule('-', 82);

    double total = 0.0;
    for (const auto& it : overstock) {
        double val = it.inventoryValueAtCost();
        total += val;
        std::cout << std::left
                  << std::setw(12) << truncate(it.sku, 11)
                  << std::setw(46) << truncate(it.description, 45)
                  << std::right
                  << std::setw(10) << groupedInt(it.qoh)
                  << std::setw(14) << money(val)
                  << "\n";
    }
    std::cout << "\nTotal cost tied up in excess replacements: " << money(total) << "\n\n";
}

void printFinancialSummary(const HealthAnalyzer::FinancialSummary& s) {
    std::cout << "4. ESTIMATED FINANCIAL IMPACT SUMMARY\n";
    printRule('-', 60);
    std::cout << std::left  << std::setw(42) << "Total inventory value at cost:"
              << std::right << money(s.totalValueAtCost) << "\n";
    std::cout << std::left  << std::setw(42) << "Value locked in dead stock:"
              << std::right << money(s.deadStockValue) << "\n";
    std::cout << std::left  << std::setw(42) << "Estimated shrink from negative QOH:"
              << std::right << money(s.negativeQohShrinkCost) << "\n";
    std::cout << std::left  << std::setw(42) << "Potential lost gross profit (overstock):"
              << std::right << money(s.potentialLostMargin) << "\n\n";
    std::cout << "These issues artificially lower COGS and inflate reported profits.\n";
    std::cout << "SKU cleanup, cycle counts, and pricing discipline would improve\n";
    std::cout << "accuracy, cash flow, and long-term viability.\n\n";
}

void printFullReport(const std::vector<Item>& items) {
    printHeader(items.size());
    printReplacementOverstock(HealthAnalyzer::findReplacementOverstock(items));
    printNegativeQoh(HealthAnalyzer::findNegativeQoh(items));
    printDeadStock(HealthAnalyzer::findDeadStock(items));
    printFinancialSummary(HealthAnalyzer::summarize(items));
}

} // namespace ReportPrinter
