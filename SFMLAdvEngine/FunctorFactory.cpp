#include "FunctorFactory.h"

std::shared_ptr<FunctorBase> FunctorFactory::CreateFunctor(const FunctorBase::FunctorTypes wantedFunctor)
{
	switch (wantedFunctor)
	{
	case FunctorBase::FunctorTypes::Addition:
	{
		return std::make_shared<AdditionFunctor>();
	}
	case FunctorBase::FunctorTypes::Multiplication:
	{
		return std::make_shared<MultiplicationFunctor>();
	}
	case FunctorBase::FunctorTypes::Division:
	{
		return std::make_shared<DivisionFunctor>();
	}
	case FunctorBase::FunctorTypes::Subtraction:
	{
		return std::make_shared<SubtractionFunctor>();
	}
	case FunctorBase::FunctorTypes::BitwiseAND:
	{
		return std::make_shared<BitwiseANDFunctor>();
	}
	case FunctorBase::FunctorTypes::BitwiseOR:
	{
		return std::make_shared<BitwiseORFunctor>();
	}
	case FunctorBase::FunctorTypes::BitwiseXOR:
	{
		return std::make_shared<BitwiseXORFunctor>();
	}
	case FunctorBase::FunctorTypes::Sine:
	{
		return std::make_shared<SineFunctor>();
	}
	case FunctorBase::FunctorTypes::Cosine:
	{
		return std::make_shared<CosineFunctor>();
	}
	case FunctorBase::FunctorTypes::Tangent:
	{
		return std::make_shared<TangentFunctor>();
	}
	case FunctorBase::FunctorTypes::LeftShift:
	{
		return std::make_shared<LeftShiftFunctor>();
	}
	case FunctorBase::FunctorTypes::RightShift:
	{
		return std::make_shared<RightShiftFunctor>();
	}
	case FunctorBase::FunctorTypes::NOT:
	{
		return std::make_shared<NOTFunctor>();
	}
	case FunctorBase::FunctorTypes::PerlinNoise:
	{
		return std::make_shared<PerlinNoiseFunctor>();
	}
	}
}