#ifndef INVENTORY_LOADER_H
#define INVENTORY_LOADER_H

#include "Item.h"
#include <string>
#include <vector>

namespace InventoryLoader {

// Load items from a CSV file. Expected header:
// sku,description,qoh,cost,retail,last_sold_date,last_received_date,status
// Returns empty vector if the file cannot be opened.
std::vector<Item> loadFromCsv(const std::string& path);

// Hard-coded fallback data mirroring the Python generate_sample_data().
// Used when the CSV is missing so the program is always runnable.
std::vector<Item> buildFallbackSample();

} // namespace InventoryLoader

#endif
