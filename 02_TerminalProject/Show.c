#include <stdlib.h>
#include <ncurses.h>

#define DX 2
#define BUF_SIZE 1024
#define LOG_BUF_SIZE 4
#define WINDOW_LINES (LINES - 2 * DX)
#define WINDOW_COLS (COLS - 2 * DX)

int CODE_LINES = 0, MAX_LINE_LENGTH = 0;
char *buf[BUF_SIZE];  // Consider file has no more than BUF_SIZE lines
int length[BUF_SIZE];

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void wshow(WINDOW *win, int start_row, int start_col)
{
    werase(win);

    wmove(win, 1, 0);
    for (int row = start_row; row < min(start_row + WINDOW_LINES - 2, CODE_LINES); ++row) {
        char *line = buf[row];
        wprintw(win, "  %*d: %.*s\n", LOG_BUF_SIZE, row + 1,
                WINDOW_COLS - LOG_BUF_SIZE - 6, line + min(length[row], start_col));
    }

    box(win, 0, 0);
    wrefresh(win);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s FILENAME.\nShows file FILENAME.\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];

    FILE *fp = fopen(filename,"r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file %s.", filename);
        return 1;
    }

    for (int i = 0; i < BUF_SIZE; ++i) {
        buf[i] = NULL;
        size_t size;
        if ((length[i] = (int) getline(buf + i, &size, fp)) < 0) {
            break;
        }
        if (length[i] > 0 && buf[i][length[i] - 1] == '\n') {
            buf[i][length[i] - 1] = '\0';
            --length[i];
        }
        ++CODE_LINES;
        MAX_LINE_LENGTH = max(MAX_LINE_LENGTH, length[i]);
    }
    fclose(fp);

    initscr();
    noecho();
    cbreak();
    set_escdelay(0);

    printw("File: %s", filename);
    refresh();

    WINDOW *win = newwin(WINDOW_LINES, WINDOW_COLS, DX, DX);
    keypad(win, TRUE);
    scrollok(win, TRUE);

    int c, cur_row = 0, cur_col = 0;
    wshow(win, cur_row, cur_col);

    while ((c = wgetch(win)) != 27) {
        switch (c) {
            case KEY_UP:
                --cur_row;
                break;
            case ' ':
            case KEY_DOWN:
                ++cur_row;
                break;
            case KEY_PPAGE:
                cur_row -= WINDOW_LINES - 2;
                break;
            case KEY_NPAGE:
                cur_row += WINDOW_LINES - 2;
                break;
            case KEY_LEFT:
                --cur_col;
                break;
            case KEY_RIGHT:
                ++cur_col;
                break;
            default:
                break;
        }

        cur_row = max(0, min(cur_row, CODE_LINES - 1));
        cur_col = max(0, min(cur_col, MAX_LINE_LENGTH - 1));

        wshow(win, cur_row, cur_col);
    }

    endwin();
    for (int i = 0; i < CODE_LINES; ++i) {
        free(buf[i]);
    }
}
