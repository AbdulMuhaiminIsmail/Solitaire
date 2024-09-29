#include <iostream>
using namespace std;

class Command {
private:
	string cmd;
	string src;
	string dest;
	int count;

public:
	Command(string cmd, string src, string dest, int count = 1) : cmd(cmd), src(src), dest(dest), count(count) {}

    string getCmd() const {
        return cmd;
    }

    string getSrc() const {
        return src;
    }

    string getDest() const {
        return dest;
    }

    int getCount() const {
        return count;
    }
};

