#pragma once
#include <memory>

#include "AdditionFunctor.h"
#include "DivisionFunctor.h"
#include "MultiplicationFunctor.h"
#include "SubtractionFunctor.h"
#include "BitwiseANDFunctor.h"
#include "BitwiseORFunctor.h"
#include "BitwiseXORFunctor.h"
#include "SineFunctor.h"
#include "CosineFunctor.h"
#include "TangentFunctor.h"
#include "LeftShiftFunctor.h"
#include "RightShiftFunctor.h"
#include "NOTFunctor.h"

class FunctorFactory
{
private:
	FunctorFactory();

public:
	static  std::shared_ptr<FunctorBase> CreateFunctor(const FunctorBase::FunctorTypes wantedFunctor);
};
