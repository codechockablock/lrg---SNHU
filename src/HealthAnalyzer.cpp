#include "HealthAnalyzer.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

namespace HealthAnalyzer {

namespace {

bool parseYmd(const std::string& ymd, std::tm& out) {
    if (ymd.size() < 10) return false;
    std::memset(&out, 0, sizeof(out));
    try {
        int y = std::stoi(ymd.substr(0, 4));
        int m = std::stoi(ymd.substr(5, 2));
        int d = std::stoi(ymd.substr(8, 2));
        out.tm_year = y - 1900;
        out.tm_mon  = m - 1;
        out.tm_mday = d;
        out.tm_hour = 12; // avoid DST edge cases on day boundary
        return true;
    } catch (...) {
        return false;
    }
}

std::string toLower(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    for (char c : s) result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    return result;
}

} // namespace

long daysBetween(const std::string& fromYmd, const std::string& toYmd) {
    std::tm a{}, b{};
    if (!parseYmd(fromYmd, a) || !parseYmd(toYmd, b)) return -1;
    std::time_t ta = std::mktime(&a);
    std::time_t tb = std::mktime(&b);
    if (ta == -1 || tb == -1) return -1;
    double seconds = std::difftime(tb, ta);
    return static_cast<long>(seconds / 86400.0);
}

std::vector<Item> findNegativeQoh(const std::vector<Item>& items) {
    std::vector<Item> result;
    for (const auto& it : items) {
        if (it.qoh < 0) result.push_back(it);
    }
    return result;
}

std::vector<Item> findDeadStock(const std::vector<Item>& items, int monthsThreshold) {
    // Treat a month as 30.44 days for threshold purposes — same
    // approximation the Python version uses with its 547-day cutoff.
    const long thresholdDays = static_cast<long>(monthsThreshold * 30.44);

    std::vector<Item> result;
    for (const auto& it : items) {
        if (it.qoh <= 0) continue;
        long days = daysBetween(it.lastSoldDate, kReportDate);
        if (days < 0) continue;
        if (days > thresholdDays) result.push_back(it);
    }
    return result;
}

std::vector<Item> findReplacementOverstock(const std::vector<Item>& items) {
    if (items.empty()) return {};

    // 90th-percentile QOH cutoff across all items.
    std::vector<int> qohs;
    qohs.reserve(items.size());
    for (const auto& it : items) qohs.push_back(it.qoh);
    std::sort(qohs.begin(), qohs.end());
    const std::size_t idx = static_cast<std::size_t>(0.9 * qohs.size());
    const int cutoff = qohs[std::min(idx, qohs.size() - 1)];

    std::vector<Item> result;
    for (const auto& it : items) {
        const std::string desc = toLower(it.description);
        const bool looksReplacement =
            desc.find("replacement") != std::string::npos ||
            desc.find("(repl")       != std::string::npos;
        if (looksReplacement && it.qoh > cutoff) {
            result.push_back(it);
        }
    }
    return result;
}

FinancialSummary summarize(const std::vector<Item>& items) {
    FinancialSummary s;
    s.totalItems = items.size();

    const auto dead = findDeadStock(items);
    const auto neg  = findNegativeQoh(items);

    for (const auto& it : items) {
        s.totalValueAtCost    += it.inventoryValueAtCost();
        s.potentialLostMargin += it.potentialLostMargin();
    }
    for (const auto& it : dead) s.deadStockValue        += it.inventoryValueAtCost();
    for (const auto& it : neg)  s.negativeQohShrinkCost += std::fabs(it.inventoryValueAtCost());
    return s;
}

} // namespace HealthAnalyzer
