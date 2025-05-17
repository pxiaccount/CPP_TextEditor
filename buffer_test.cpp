#include <ncurses.h>
#include <iostream>
#include <string>
using namespace std;

class TextBuffer {
	private:
		string buffer;
		size_t cursor;
	
	public:
		TextBuffer(): cursor(0) {}

	void insert(char ch) {
		buffer.insert(cursor, 1, ch);
		cursor++;
	}

	void deleteChar() {
		if (cursor > 0) {
		buffer.erase(cursor-1, 1);
		cursor -= 1;
		}
	}
	
	void MoveCursorLeft() {
		if (cursor > 0) cursor--;
	}

	void MoveCursorRight() {
		if (cursor < buffer.length()) cursor++;
	}

	void render() {
		clear();
		for (size_t i = 0; i < buffer.size(); i++) {
			mvaddch(0, i, buffer[i]);
		}
		move(0, cursor);
		refresh();
	}
};

int main () {
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(1);

	TextBuffer x;
	int ch;

	while((ch = getch()) != 27) {
		switch(ch) {
			case KEY_LEFT:
				x.MoveCursorLeft();
				break;
			case KEY_RIGHT:
				x.MoveCursorRight();
				break;
			case KEY_BACKSPACE:
				x.deleteChar();
				break;
			default:
				if (ch >= 32 && ch <= 126) x.insert((char)ch);
		}
		x.render();
	}

	endwin();
	return 0;

}
