# Ncurses-stuff

This project is my playground for learning **ncurses** — specifically the *wide*
version for UTF-8 support. I’m using it to work through *Dan Gookin’s Guide to Ncurses Programming* (ISBN 978-1549682575).  

---

## What this repo does

- Uses **C++20** on macOS (with Homebrew ncurses) and Linux.  
- Every subdirectory in `src/` should contain a standalone program; each builds into a binary of the same name.  
- Binaries are placed in `build/bin/`.  
- **CMakeLists.txt automatically enforces `_XOPEN_SOURCE_EXTENDED`**, so you do not need to define it in each program.  
- Builds link against the **wide version of ncurses** (`ncursesw` / `panelw`) to support UTF-8 characters like emojis.  

## Book & Resources

- *Dan Gookin’s Guide to Ncurses Programming* ISBN 978-1549682575 — main guide for learning ncurses.
- Publisher info: Wambooli Press has a page for *Programmer’s Guide to NCurses*.
- Example C code from the book is available online: [https://c-for-dummies.com/ncurses/](https://c-for-dummies.com/ncurses/)

---

## Setup & Usage

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Then run the programs in `build/bin/`.

## License & Attribution

All code here is my own, built for learning. The structure and examples are inspired
by / adapted from *Dan Gookin’s Guide to Ncurses Programming*.

