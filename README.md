# Pike Creek Inventory Health Analyzer (C++)

**CS 210 Portfolio Submission — Joseph Hopkins**

A C++17 port of a Python + SQLite inventory analysis tool I built earlier this year. The program ingests inventory data, flags four categories of operational problems, and prints a formatted financial impact report to the console.

> Demonstration only — all data is simulated.

## Build and run

```bash
make          # builds ./inventory_health
make run      # builds and runs against data/inventory.csv
./inventory_health                       # uses data/inventory.csv
./inventory_health path/to/other.csv     # or any CSV with the same header
```

If no CSV is provided the program falls back to a built-in sample dataset so it is always runnable.

Tested with Apple clang 17 / GCC 13, C++17, `-Wall -Wextra -Wpedantic -O2` clean.

## Project structure

-------------------------
pike-creek-cpp/
├── Makefile
├── data/inventory.csv            # sample data
└── src/
    ├── Item.h                    # data model
    ├── InventoryLoader.{h,cpp}   # CSV parsing + fallback sample
    ├── HealthAnalyzer.{h,cpp}    # four analysis rules + date math
    ├── ReportPrinter.{h,cpp}     # formatted console output
    └── main.cpp                  # interactive menu loop
-------------------------

## The four analysis rules

1. **Replacement SKU overstock** — items whose description contains "replacement" and whose QOH sits above the 90th percentile of the whole set. Catches the classic pattern of ordering the new SKU without drawing down the old one.
2. **Negative quantity on hand** — items where `qoh < 0`. The system thinks we have less than zero, which usually means unrecorded shrink, unscanned transfers, or receiving errors.
3. **Dead stock** — items with positive QOH that have not sold in 18+ months. Capital locked up on the shelf.
4. **Financial summary** — total inventory at cost, dollars trapped in dead stock, estimated shrink from negatives, and potential lost gross margin in overstock.

---

## Reflection

### Summarize the project and what problem it was solving

This project analyzes a simulated hardware-store inventory to surface four specific operational problems that distort a retailer's financials: overstock from discontinued → replacement SKU substitution, negative quantity-on-hand, dead stock that hasn't moved in over 18 months, and the compounding financial impact of all three.

The original Python version (see my [pike-creek-inventory-analysis](https://github.com/codechockablock/pike-creek-inventory-analysis) repo) used a local SQLite database and pandas. This submission is a ground-up **refactor of that same tool into C++17**, using nothing but the standard library. Every piece of Python machinery had to be replaced by hand:

| Python / pandas                                   | C++17 replacement                                                   |
| ------------------------------------------------- | ------------------------------------------------------------------- |
| `sqlite3` + `pd.read_sql_query`                   | `std::ifstream` + a hand-written CSV splitter                       |
| `pd.DataFrame` with named columns                 | `struct Item` + `std::vector<Item>`                                 |
| Boolean masks (`df[df['qoh'] < 0]`)               | Explicit filter functions returning `std::vector<Item>`             |
| `df['days_since_last_sale'] = (today - ...).dt.days` | `mktime` / `difftime` date math with DST-safe noon anchoring     |
| `f"${value:,.2f}"`                                | `std::ostringstream` imbued with a grouping locale + `std::setw`    |
| `.quantile(0.9)`                                  | `std::sort` + index into the 90th-percentile slot                   |

The point of the refactor was to prove the core logic could stand on its own without a data-science stack. That matters because the kind of environment this tool would actually run in — a back-office machine at a small retailer — rarely has Python and pandas installed, and a single static binary is much easier to deploy than a Python virtualenv.

### What did I do particularly well

The modular separation worked out well. Each translation unit does exactly one thing: `InventoryLoader` knows about files and strings, `HealthAnalyzer` knows about business rules, `ReportPrinter` knows about formatting, and `main` only knows about the menu. Swapping the CSV loader for a future SQLite loader would not require changing the analyzer or the printer at all. I was also deliberate about types — `qoh` is a signed `int` because negative inventory is the whole point of one of the rules, and I documented that in the header so a future reader does not "optimize" it to `size_t`.

### Where could I enhance the code

Three places stand out. **Money as `double`** is acceptable for individual use but it is the wrong choice for a real retail application — cumulative rounding error is a known problem. I would switch to an integer-cents representation (`int64_t`) with thin accessor functions. **CSV parsing** handles simple quoted fields but does not support escaped quotes (`""`) inside a quoted field; real POS exports will break that. A more robust parser or a small third-party CSV library would harden it. **Input validation on the menu** rejects bad input and re-prompts, but the program does not re-validate CSV rows semantically — a row with a negative cost or a future last-sold date would pass through silently. Adding a validation pass after load would surface data quality problems upstream.

### Which pieces were most challenging

The date math. Python hands you `datetime` arithmetic for free; C++ before C++20 gives you `struct tm` and `mktime`, which is easy to get wrong around DST and month-boundary edges. I worked around it by setting `tm_hour = 12` before calling `mktime`, which avoids the 23-hour / 25-hour day-boundary problem that DST transitions create. The other sharp edge was getting `std::locale` to actually produce thousands separators on macOS — the default locale constructed with `""` does not reliably enable grouping, so I fell back to trying `"en_US.UTF-8"` explicitly first.

Honestly, the biggest tool I leaned on for the tricky Python-to-C++ translation work was AI. I used Claude as a pair programmer to talk through how pandas idioms map onto C++ primitives (boolean masks → filter functions, DataFrame columns → struct fields, `quantile` → `std::sort` + index), and to sanity-check type decisions at the API boundaries — particularly *why* `qoh` needed to be a signed `int` rather than `size_t`. AI did not write this submission for me; I drove every design decision, read every line I committed, and tested each rule against the original Python output to confirm the refactor preserved the behavior. But using an AI assistant as a translation partner for an unfamiliar language saved me hours of thrashing on syntax and let me spend my time on the architectural decisions that actually matter. Along with AI, the support-network resources I am adding firmly are **cppreference** for authoritative standard-library behavior and **Compiler Explorer** for quickly checking what a snippet lowers to across compilers.

### What skills will transfer

Separation of concerns and pure functions. Every analyzer function takes `const std::vector<Item>&` in and returns a `std::vector<Item>` or a small summary struct out — no shared state, no side effects, easy to unit-test. That pattern is language-agnostic and is the single biggest thing I will take forward. Second: being precise about types at API boundaries. Headers encode intent, and choosing `int` vs `size_t` vs `long` is not cosmetic — it tells the next reader what values are legal. Third: treating CLI input as untrusted. The menu clears stream state on a bad read instead of spinning on a failed parse, and that same pattern applies to any program that accepts user input.

### How did I make this program maintainable, readable, and adaptable

I followed a handful of rules consistently. Every header has an include guard and no `using namespace std;`. Each class or namespace has a single responsibility and a short header block explaining what it owns. Function names are verbs (`findDeadStock`, `printFullReport`) and data names are nouns (`Item`, `FinancialSummary`). The Makefile is eight lines and the build flags catch undefined behavior early (`-Wall -Wextra -Wpedantic`). The program works in two input modes — CSV for real data and a built-in fallback sample for quick demos — so a grader or future employer can clone and run it without any setup. And the console report is plain text, which makes it easy to diff across runs or redirect to a file for archiving. Taken together, these choices should let someone else pick the code up six months from now and extend it without having to reverse-engineer my decisions.
