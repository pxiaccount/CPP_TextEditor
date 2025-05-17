#include <iostream>
#include <string>
using namespace std;

class TextBuffer {
	private:
		string buffer;
		size_t cursor;
	
	public:
		TextBuffer(): cursor(0) {}

	void insert(const string& text) {
		buffer.insert(cursor, text);
		cursor += text.length();
	}

	void deleteChar(size_t count) {
		if (cursor >= count) {
		buffer.erase(cursor-count, count);
		cursor -= count;
		}
	}

	void moveCursor(size_t position) {
		if (position <= buffer.length()) {
			cursor = position;
		}
	}

	void printBuffer() {
		cout << buffer << endl;
		cout << cursor << endl;
	}
};

int main () {
	TextBuffer x;

	x.insert("Hello");
	x.insert("World");

	x.deleteChar(1);

	x.printBuffer();

	return 0;
}
