#ifndef HEALTH_ANALYZER_H
#define HEALTH_ANALYZER_H

#include "Item.h"
#include <string>
#include <vector>

namespace HealthAnalyzer {

// Fixed "today" reference matches the Python version so numbers are
// reproducible across runs and match the original report.
constexpr const char* kReportDate = "2025-12-17";

// Number of whole days between two YYYY-MM-DD dates.
// Returns -1 if either string is not parseable.
long daysBetween(const std::string& fromYmd, const std::string& toYmd);

// QOH < 0 — inventory the system thinks we have less than zero of.
std::vector<Item> findNegativeQoh(const std::vector<Item>& items);

// QOH > 0 and no sale in monthsThreshold months (default 18).
std::vector<Item> findDeadStock(const std::vector<Item>& items,
                                int monthsThreshold = 18);

// "Replacement" items (description contains "replacement") carrying
// more stock than the top-decile cutoff of the whole set.
std::vector<Item> findReplacementOverstock(const std::vector<Item>& items);

struct FinancialSummary {
    double totalValueAtCost      = 0.0;
    double deadStockValue        = 0.0;
    double negativeQohShrinkCost = 0.0; // absolute value
    double potentialLostMargin   = 0.0;
    std::size_t totalItems       = 0;
};
FinancialSummary summarize(const std::vector<Item>& items);

} // namespace HealthAnalyzer

#endif
