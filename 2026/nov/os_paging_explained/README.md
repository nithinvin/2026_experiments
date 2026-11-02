# Paging in Operating Systems, in Plain English

**Paging** is how an operating system splits a program's memory into small, equal-sized pieces and puts those pieces wherever RAM has room, while the program still believes its memory is one neat, continuous block.

## The one analogy to remember

Imagine moving your belongings into a storage building:

- You pack everything into **identical boxes**.
- The building has **identical lockers**, each holding exactly one box.
- Your boxes can go into **any free lockers**, in any order.
- A **notebook** records which box is in which locker.

| Storage building | Operating system term |
|---|---|
| Box | Page (a piece of a program's memory) |
| Locker | Frame (a slot in RAM) |
| Notebook | Page table |
| "Box 2, item 50" | Virtual address (what the program uses) |
| "Locker 7, item 50" | Physical address (the real RAM location) |
| Sticky note with recently used lockers | TLB (a fast cache of translations) |
| Off-site warehouse | Disk (swap space) |

```mermaid
flowchart LR
    P["Program asks for<br/>box 2, item 50"] --> N["Notebook says<br/>box 2 is in locker 7"]
    N --> R["Go to<br/>locker 7, item 50"]
```

## Reading order

| # | File | Question it answers |
|---|---|---|
| 1 | [Why paging exists](01_why_paging_exists.md) | What problem does paging solve? |
| 2 | [How paging works](02_how_paging_works.md) | How does an address get translated? |
| 3 | [The TLB](03_tlb_speeding_it_up.md) | Why isn't all this lookup work painfully slow? |
| 4 | [Page faults and replacement](04_page_faults_and_replacement.md) | What if the data isn't in RAM, or RAM is full? |
| 5 | [Multi-level page tables](05_multilevel_page_tables.md) | How do we stop page tables from taking too much memory? |
| 6 | [Cheat sheet and practice](06_cheat_sheet_and_practice.md) | Can I work the problems myself? |
