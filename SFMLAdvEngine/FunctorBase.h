#pragma once

#include <iostream>

class FunctorBase {
protected:
	int val;
public:
	FunctorBase() : val{ 0 } {};
	FunctorBase(int val2) :val{ val2 } {};

	int operator () (int val2) {
		return val;
	}
};

