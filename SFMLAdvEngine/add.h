#include <iostream>

using namespace std;

class add {
	int val1;
public:
	add() {};
	add(int val2) :val1{ val2 } {};

	int operator () (int val2) {
		return val1 + val2;
	}
};

