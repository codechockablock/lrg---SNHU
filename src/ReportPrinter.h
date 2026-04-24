#ifndef REPORT_PRINTER_H
#define REPORT_PRINTER_H

#include "HealthAnalyzer.h"
#include "Item.h"
#include <vector>

namespace ReportPrinter {

void printHeader(std::size_t totalItems);

void printNegativeQoh(const std::vector<Item>& negatives);
void printDeadStock(const std::vector<Item>& deadStock);
void printReplacementOverstock(const std::vector<Item>& overstock);
void printFinancialSummary(const HealthAnalyzer::FinancialSummary& summary);

// Convenience: run every section against the full item set.
void printFullReport(const std::vector<Item>& items);

} // namespace ReportPrinter

#endif
