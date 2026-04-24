// Pike Creek Community Hardware - Inventory Health Analyzer (C++ port)
// CS 210 portfolio submission.
//
// Refactored from the original Python + SQLite implementation. CSV
// input replaces the database so the program builds and runs with
// only the C++17 standard library.

#include "HealthAnalyzer.h"
#include "InventoryLoader.h"
#include "Item.h"
#include "ReportPrinter.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

void printMenu() {
    std::cout << "\n====================================\n";
    std::cout <<   "  Inventory Health Analyzer - Menu\n";
    std::cout <<   "====================================\n";
    std::cout << "  1) Full health report\n";
    std::cout << "  2) Negative QOH only\n";
    std::cout << "  3) Dead stock only\n";
    std::cout << "  4) Replacement overstock only\n";
    std::cout << "  5) Financial summary only\n";
    std::cout << "  6) Exit\n";
    std::cout << "Choice: ";
}

int readMenuChoice() {
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::vector<Item> loadItems(const std::string& csvPath) {
    auto items = InventoryLoader::loadFromCsv(csvPath);
    if (items.empty()) {
        std::cout << "Could not read '" << csvPath
                  << "'. Using built-in sample data.\n";
        items = InventoryLoader::buildFallbackSample();
    } else {
        std::cout << "Loaded " << items.size() << " items from " << csvPath << ".\n";
    }
    return items;
}

} // namespace

int main(int argc, char* argv[]) {
    const std::string csvPath = (argc > 1) ? argv[1] : "data/inventory.csv";
    const std::vector<Item> items = loadItems(csvPath);

    while (true) {
        printMenu();
        const int choice = readMenuChoice();

        switch (choice) {
            case 1:
                ReportPrinter::printFullReport(items);
                break;
            case 2:
                ReportPrinter::printNegativeQoh(HealthAnalyzer::findNegativeQoh(items));
                break;
            case 3:
                ReportPrinter::printDeadStock(HealthAnalyzer::findDeadStock(items));
                break;
            case 4:
                ReportPrinter::printReplacementOverstock(HealthAnalyzer::findReplacementOverstock(items));
                break;
            case 5:
                ReportPrinter::printFinancialSummary(HealthAnalyzer::summarize(items));
                break;
            case 6:
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please enter 1-6.\n";
                break;
        }
    }
}
