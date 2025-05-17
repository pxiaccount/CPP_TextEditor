#include <ncurses.h>
#include <vector>
#include <string>

#define CTRL_KEY(k) ((k) & 0x1f)

class TextBuffer {
private:
    std::vector<std::string> lines;
    int cursorX, cursorY;

public:
    TextBuffer() : cursorX(0), cursorY(0) {
        lines.push_back("");
    }

    void insert(char ch) {
        if (ch == '\n') {
            std::string newLine = lines[cursorY].substr(cursorX);
            lines[cursorY] = lines[cursorY].substr(0, cursorX);
            lines.insert(lines.begin() + cursorY + 1, newLine);
            cursorY++;
            cursorX = 0;
        } else {
            lines[cursorY].insert(cursorX, 1, ch);
            cursorX++;
        }
    }

    void deleteChar() {
        if (cursorX == 0 && cursorY == 0)
            return;

        if (cursorX > 0) {
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
        } else {
            cursorX = lines[cursorY - 1].size();
            lines[cursorY - 1] += lines[cursorY];
            lines.erase(lines.begin() + cursorY);
            cursorY--;
        }
    }

    void moveCursorLeft() {
        if (cursorX > 0) cursorX--;
        else if (cursorY > 0) {
            cursorY--;
            cursorX = lines[cursorY].size();
        }
    }

    void moveCursorRight() {
        if (cursorX < (int)lines[cursorY].size()) cursorX++;
        else if (cursorY < (int)lines.size() - 1) {
            cursorY++;
            cursorX = 0;
        }
    }

    void moveCursorUp() {
        if (cursorY > 0) {
            cursorY--;
            if (cursorX > (int)lines[cursorY].size())
                cursorX = lines[cursorY].size();
        }
    }

    void moveCursorDown() {
        if (cursorY < (int)lines.size() - 1) {
            cursorY++;
            if (cursorX > (int)lines[cursorY].size())
                cursorX = lines[cursorY].size();
        }
    }

    void render(const std::string& status = "") const {
        clear();
        for (size_t i = 0; i < lines.size(); i++) {
            mvprintw(i, 0, "%s", lines[i].c_str());
        }
        mvprintw((int)lines.size(), 0, "%s", status.c_str());
        move(cursorY, cursorX);
        refresh();
    }
};

int main() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    TextBuffer editor;
    std::string status;

    int ch;
    editor.render(status);

    while ((ch = getch()) != 27) {
        switch (ch) {
            case KEY_LEFT: editor.moveCursorLeft(); break;
            case KEY_RIGHT: editor.moveCursorRight(); break;
            case KEY_UP: editor.moveCursorUp(); break;
            case KEY_DOWN: editor.moveCursorDown(); break;
            case KEY_BACKSPACE:
            case 127: editor.deleteChar(); break;
            case '\n': editor.insert('\n'); break;
            case CTRL_KEY('s'):
                status = "Save failed.";
                break;
            default:
                if (ch >= 32 && ch <= 126)
                    editor.insert((char)ch);
        }
        editor.render(status);
    }

    endwin();
    return 0;
}

