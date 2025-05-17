#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

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

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    for (size_t i = 0; i < lines.size() && i < (size_t)(rows - 2); ++i) {
        mvprintw(i, 0, "%s", lines[i].c_str());
    }

    mvhline(rows - 2, 0, '-', cols);
    mvprintw(rows - 2, 2, "%s", status.c_str());

    mvhline(rows - 1, 0, ' ', cols);
    mvprintw(rows - 1, 0, "F2: Save | Ctrl C: Exit");

    move(cursorY, cursorX);
    refresh();
}

    bool loadFromFile(const string& filename) {
        ifstream inFile(filename);
    	if (!inFile) return false;

    	lines.clear();
        cursorX = 0;
        cursorY = 0;

	string line;
	while (getline(inFile, line)) {
        	lines.push_back(line);
    	}
	if (lines.empty()) lines.push_back("");
	return true;
    }

    bool saveToFile(const string& filename) const {
    	ofstream outFile(filename);
    	if (!outFile) return false;

    	for (const auto& line : lines) {
        	outFile << line << '\n';
    	}
    	return true;
    }
};

int main() {
    string filename;
    cout << "Enter file name to open or create: ";
    getline(cin, filename);

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    TextBuffer editor;
    string status;

    if (editor.loadFromFile(filename))
        status = "Loaded " + filename;
    else {
        status = "New file: " + filename;
    }

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
            case KEY_F(2):
                if (editor.saveToFile(filename))
                    status = "Saved to " + filename;
                else
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

