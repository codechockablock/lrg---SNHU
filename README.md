# Joseph Hopkins — SNHU Computer Science Portfolio

This repository catalogs artifacts from my Computer Science coursework at Southern New Hampshire University, each with a short written reflection. I add to it as I move through the program so that the work, and my thinking about the work, stays in one place.

**Contents**

- [CS 250: Software Development Lifecycle](#cs-250-software-development-lifecycle) — SNHU Travel, Agile/Scrum Sprint Review and Retrospective
- [CS 230: Software Design and Architecture](#cs-230-software-design-and-architecture) — The Gaming Room, *Draw It or Lose It*
- [CS 210: Programming Languages](#cs-210-programming-languages) — Pike Creek Inventory Health Analyzer (C++)

---

## CS 250: Software Development Lifecycle

**Artifact: SNHU Travel Sprint Review and Retrospective**

- [Sprint Review and Retrospective (PDF)](CS250/SNHU-Travel-Sprint-Review-and-Retrospective.pdf) — viewable in the browser
- [Sprint Review and Retrospective (DOCX)](CS250/SNHU-Travel-Sprint-Review-and-Retrospective.docx) — original file

This deliverable pulls together the Agile and Scrum practices I applied on the SNHU Travel project, where I worked across the Scrum Master, Product Owner, developer, and tester roles to build a destination recommendation tool.

### How I interpret user needs and implement them, and how user stories help

I interpret user needs by translating them into something checkable before I write any code. On SNHU Travel I turned focus-group input into six prioritized user stories written in the "As a user, I want a task, so that I reach a goal" format. That format keeps the focus on the value to the person using the software rather than on the feature in isolation. A story like Top Five Destinations only became real once it had acceptance criteria attached: an ordered list showing a name, description, picture, and package link for each destination. Those criteria are the hinge between a need and a program, because they define what "done" looks like in a way the developer can build to and the tester can verify against. When the application shipped with exactly those attributes for Paris, Bali, Santorini, Machu Picchu, and the Serengeti, I knew the user's need had actually been met and not just approximated.

User stories help most because they are small and independent. When the requirements changed, I did not have to rethink the whole product, only the stories the change touched. Sizing each story large, medium, or small also let the team prioritize honestly and commit to what it could finish, instead of promising everything and delivering a rushed version of it.

### How I approach developing programs, and the Agile processes I want to carry forward

I approach development iteratively. Rather than trying to specify everything up front, I build in small slices, put working software in front of stakeholders early, and expect the requirements to move. SNHU Travel proved why that is worth keeping: the project changed direction twice, first when results moved from a scrollable list to a slideshow with Previous and Next buttons, and again when management refocused the whole tool on detox and wellness travel. Because the work was sliced into small stories and the destination content lived in a separate data structure from the code that drew the screen, the second pivot became a re-theme instead of a rebuild, and we adapted without moving the deadline.

The Agile processes I most want to carry into future work are the ones that keep work visible and tied to value. I want to keep a prioritized backlog with real acceptance criteria as the single source of truth, use a task board as an information radiator so anyone can see status at a glance, and protect the regular Scrum events: the Daily Scrum to surface blockers, refinement and planning to turn vague stories into buildable ones, the review to show working software, and the retrospective to improve the process itself. The lesson I would apply more strictly is that Agile only pays off when communication and acceptance criteria stay disciplined. Where they slipped on my project, progress stalled, so I want to keep questions specific and decisions written down.

### What it means to be a good team member in software development

Being a good team member means making your work legible to everyone who depends on it. On SNHU Travel the project worked because the roles connected: an item that started as an acceptance criterion became a verification step for the tester and then a build requirement for the developer. Good teammates keep that chain intact by communicating in a way that respects other people's time. When I needed clarification, I sent numbered, closed questions tied to a specific Scrum event and a clear deadline, so a busy stakeholder could answer quickly and the decisions were documented. When I started the wellness pivot, I wrote to both the Product Owner and the tester at once so the developer and tester would not end up building and checking two different versions of the same feature.

It also means contributing to an environment where people feel safe to speak up. The daily stand-up only works if someone is willing to admit they are blocked, and that willingness depends on psychological safety. A good team member helps create that safety by raising their own blockers honestly, keeping commitments to shared deadlines, and treating change as expected rather than as someone's fault. In short, being a good team member is less about individual output and more about keeping the work visible, the communication clear, and the team able to adapt together.

---

## CS 230: Software Design and Architecture

**Artifact: Software Design Document for The Gaming Room — *Draw It or Lose It***

- [Software Design Document (PDF)](CS230/Draw-It-or-Lose-It-Software-Design-Document.pdf) — viewable in the browser
- [Software Design Document (DOCX)](CS230/Draw-It-or-Lose-It-Software-Design-Document.docx) — original file

### Summarize the client and their software requirements

The client was **The Gaming Room**, a company that already had a word-guessing game called *Draw It or Lose It* running as an Android-only app. They came to my firm, Creative Technology Solutions (CTS), because a single-platform version limited their audience. They wanted a web-based version of the game that one team could host centrally and that players could reach from Linux, Mac, Windows, and mobile browsers without installing anything platform-specific.

Their core requirements were straightforward but carried real architectural weight. A game needed to support one or more teams, each team needed to hold multiple players, game and team names had to be unique so players could check availability before registering, and only one instance of the game could exist in memory at a time so the game state never conflicted. My job was to produce the software design document that explained how a Java domain model would meet those needs, and to recommend the right platform and architecture to deploy it on.

### What I did particularly well

The part I am most satisfied with is the way the document ties each client requirement to a specific technical decision instead of describing the system in the abstract. The single-instance requirement maps to the Singleton pattern in `GameService`, the name-uniqueness requirement maps to the Iterator pattern in `addGame()`, `addTeam()`, and `addPlayer()`, and the shared identity of every object maps to a single abstract `Entity` base class that `Game`, `Team`, and `Player` all extend. Because every choice traces back to a stated requirement, a reader can follow the reasoning without having to take my word for it.

I also think the writing does its job. I wrote for two audiences at once, a non-technical client who needs to understand what they are buying and a development team who needs enough detail to build it, and I kept the language plain even when the topic was technical. The platform evaluation table and the recommendations section give the client a clear, justified answer, a cloud-hosted Linux server, rather than a list of options with no opinion attached.

### What about the design process helped when developing the code

Working through the domain model before writing any code forced me to settle the hard questions early. By the time the UML class diagram was drawn, I already knew the class hierarchy, which class owned which collection, and where the two design patterns lived. That meant the structure was decided on paper, where it is cheap to change, instead of in the middle of implementation, where it is expensive. Mapping the `Game` to `Team` to `Player` containment as a clean three-level hierarchy in the diagram is what made the `add` methods obvious to write later.

The document also surfaced constraints I would have otherwise hit by surprise. Writing the Design Constraints section is where I realized that the stateless nature of HTTP, the chance of two users submitting the same name at the same instant, and the roughly 1.6 GB image library would each force decisions that a naive port of the Android app would have ignored. Naming those problems on paper is what let me design around them instead of patching them later.

### One part I would revise

If I could revise one piece, it would be the System Architecture View. In my document that section is light, because the project templates did not require a full physical topology, but it is the section a real development team would most want a picture for. I would add a deployment diagram showing the three tiers I currently describe only in words: the client browser, the Java application server behind an NGINX reverse proxy, and the PostgreSQL database, with the CDN and image storage drawn in. Saying "three tiers" in prose is fine, but a diagram would let a developer see the request path and the trust boundaries at a glance, and it would give the security recommendations a place to live.

### How I interpreted the user's needs and why that matters

I treated the client's four requirements as the fixed points the whole design had to satisfy, and I kept checking my decisions against them. The rule that only one game exist at a time is not just a preference; it is there to prevent conflicting game state, so the design explains not only that `GameService` is a Singleton but why a single authoritative source of state matters once the game is on the web. The rule to verify name availability before registering is really a usability need, so the Iterator-based uniqueness check is presented as something that protects the player's experience, not just a coding detail.

Considering the user's needs matters because the technology is only a means to an end. A technically elegant design that does not let two teams compete, or that lets two games run at once, has failed no matter how clean the code is. Designing from the user's requirements outward also keeps scope honest. It is easy to add abstraction and features that feel sophisticated but that no requirement asked for, and anchoring every decision to a real need is the discipline that keeps a design from drifting away from what the client actually paid for.

### How I approached the design, and what I would carry forward

My approach was to move from the general to the specific. I started with the executive summary and the requirements so the problem was stated clearly, then modeled the domain with classes and relationships, then evaluated the platforms, and only then made concrete architecture and deployment recommendations. Object-oriented design was the backbone throughout: inheritance to remove duplication, encapsulation to keep entity state immutable, and well-known design patterns so I was solving the single-instance and uniqueness problems with proven solutions instead of inventing my own.

For a similar project in the future I would use the same sequence, requirements first, then a UML domain model, then a platform evaluation, then recommendations, because each stage gives the next one something solid to build on. I would reach for diagrams earlier, including the deployment view mentioned above, and I would keep writing the constraints section deliberately, since that is where the non-obvious problems tend to reveal themselves. The biggest strategy I am taking forward is simple: decide the structure on paper, justify every decision against a stated requirement, and write so that both the client and the developers can follow the reasoning.

---

## CS 210: Programming Languages

**Artifact: Pike Creek Inventory Health Analyzer (C++)**

A C++17 port of a Python + SQLite inventory analysis tool I built earlier this year. The program ingests inventory data, flags four categories of operational problems, and prints a formatted financial impact report to the console.

> Demonstration only — all data is simulated.

### Build and run

```bash
make          # builds ./inventory_health
make run      # builds and runs against data/inventory.csv
./inventory_health                       # uses data/inventory.csv
./inventory_health path/to/other.csv     # or any CSV with the same header
```

If no CSV is provided the program falls back to a built-in sample dataset so it is always runnable.

Tested with Apple clang 17 / GCC 13, C++17, `-Wall -Wextra -Wpedantic -O2` clean.

### Project structure

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

### The four analysis rules

1. **Replacement SKU overstock** — items whose description contains "replacement" and whose QOH sits above the 90th percentile of the whole set. Catches the classic pattern of ordering the new SKU without drawing down the old one.
2. **Negative quantity on hand** — items where `qoh < 0`. The system thinks we have less than zero, which usually means unrecorded shrink, unscanned transfers, or receiving errors.
3. **Dead stock** — items with positive QOH that have not sold in 18+ months. Capital locked up on the shelf.
4. **Financial summary** — total inventory at cost, dollars trapped in dead stock, estimated shrink from negatives, and potential lost gross margin in overstock.

---

### Reflection

#### Summarize the project and what problem it was solving

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

#### What did I do particularly well

The modular separation worked out well. Each translation unit does exactly one thing: `InventoryLoader` knows about files and strings, `HealthAnalyzer` knows about business rules, `ReportPrinter` knows about formatting, and `main` only knows about the menu. Swapping the CSV loader for a future SQLite loader would not require changing the analyzer or the printer at all. I was also deliberate about types — `qoh` is a signed `int` because negative inventory is the whole point of one of the rules, and I documented that in the header so a future reader does not "optimize" it to `size_t`.

#### Where could I enhance the code

Three places stand out. **Money as `double`** is acceptable for individual use but it is the wrong choice for a real retail application — cumulative rounding error is a known problem. I would switch to an integer-cents representation (`int64_t`) with thin accessor functions. **CSV parsing** handles simple quoted fields but does not support escaped quotes (`""`) inside a quoted field; real POS exports will break that. A more robust parser or a small third-party CSV library would harden it. **Input validation on the menu** rejects bad input and re-prompts, but the program does not re-validate CSV rows semantically — a row with a negative cost or a future last-sold date would pass through silently. Adding a validation pass after load would surface data quality problems upstream.

#### Which pieces were most challenging

The date math. Python hands you `datetime` arithmetic for free; C++ before C++20 gives you `struct tm` and `mktime`, which is easy to get wrong around DST and month-boundary edges. I worked around it by setting `tm_hour = 12` before calling `mktime`, which avoids the 23-hour / 25-hour day-boundary problem that DST transitions create. The other sharp edge was getting `std::locale` to actually produce thousands separators on macOS — the default locale constructed with `""` does not reliably enable grouping, so I fell back to trying `"en_US.UTF-8"` explicitly first.

Honestly, the biggest tool I leaned on for the tricky Python-to-C++ translation work was AI. I used Claude as a pair programmer to talk through how pandas idioms map onto C++ primitives (boolean masks → filter functions, DataFrame columns → struct fields, `quantile` → `std::sort` + index), and to sanity-check type decisions at the API boundaries — particularly *why* `qoh` needed to be a signed `int` rather than `size_t`. AI did not write this submission for me; I drove every design decision, read every line I committed, and tested each rule against the original Python output to confirm the refactor preserved the behavior. But using an AI assistant as a translation partner for an unfamiliar language saved me hours of thrashing on syntax and let me spend my time on the architectural decisions that actually matter. Along with AI, the support-network resources I am adding firmly are **cppreference** for authoritative standard-library behavior and **Compiler Explorer** for quickly checking what a snippet lowers to across compilers.

#### What skills will transfer

Separation of concerns and pure functions. Every analyzer function takes `const std::vector<Item>&` in and returns a `std::vector<Item>` or a small summary struct out — no shared state, no side effects, easy to unit-test. That pattern is language-agnostic and is the single biggest thing I will take forward. Second: being precise about types at API boundaries. Headers encode intent, and choosing `int` vs `size_t` vs `long` is not cosmetic — it tells the next reader what values are legal. Third: treating CLI input as untrusted. The menu clears stream state on a bad read instead of spinning on a failed parse, and that same pattern applies to any program that accepts user input.

#### How did I make this program maintainable, readable, and adaptable

I followed a handful of rules consistently. Every header has an include guard and no `using namespace std;`. Each class or namespace has a single responsibility and a short header block explaining what it owns. Function names are verbs (`findDeadStock`, `printFullReport`) and data names are nouns (`Item`, `FinancialSummary`). The Makefile is eight lines and the build flags catch undefined behavior early (`-Wall -Wextra -Wpedantic`). The program works in two input modes — CSV for real data and a built-in fallback sample for quick demos — so a grader or future employer can clone and run it without any setup. And the console report is plain text, which makes it easy to diff across runs or redirect to a file for archiving. Taken together, these choices should let someone else pick the code up six months from now and extend it without having to reverse-engineer my decisions.
