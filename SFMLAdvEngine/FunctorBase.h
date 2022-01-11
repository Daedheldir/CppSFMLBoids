#pragma once

#include <iostream>

class FunctorBase {
public:
	enum class FunctorTypes {
		Addition,
		Subtraction,
		Division,
		Multiplication,
		BitwiseOR,
		BitwiseAND,
		BitwiseXOR,
		Sine,
		Cosine,
		Tangent,
		LeftShift,
		RightShift,
		NOT
	};
	static constexpr int FunctorTypesCount = 13;
protected:
	float* val;
public:
	FunctorBase(float* val2) :val{ val2 } {};
	virtual ~FunctorBase() {};
	virtual std::string GetName() { return "FB"; };
	virtual float operator()(float val1, float val2) const {
		return getVal();
	}

	virtual float getVal() const {
		return (val == nullptr) ? 0 : *val;
	}
};
