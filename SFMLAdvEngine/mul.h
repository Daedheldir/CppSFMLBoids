#include <iostream>

using namespace std;

class mul {
	int val1;
public:
	mul() {};
	mul(int val2) :val1{ val2 } {};

	int operator () (int val2) {
		return val1 * val2;
	}
};


