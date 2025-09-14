#include <locale.h>
#include <wchar.h>
#include <ncurses.h>

int main() {
    // Enable UTF-8/wide character support
    setlocale(LC_ALL, "");

    initscr();

    curs_set(0);  // hide the cursor
    const wchar_t* msg = L"Goodbye, cruel world! 🌍";
    addwstr(msg);

    refresh();
    getch();

    endwin();
    return 0;
}

