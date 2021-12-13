#include <iostream>
#include "FunctorBase.h"
using namespace std;

class MultiplicationFunctor : public FunctorBase {
public:
	MultiplicationFunctor() {};
	MultiplicationFunctor(int val2) : FunctorBase{ val2 } {};

	int operator () (float val1, float val2) {
		return val1 * val2;
	}
};


