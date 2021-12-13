#include <iostream>

using namespace std;

class div {
	int val1;
public:
	div() {};
	div(int val2) :val1{ val2 } {};

	int operator () (int val2) {
		return val1 / val2;
	}
};


