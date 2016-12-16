#pragma once

UENUM(BlueprintType)
enum class ECompareOperator : uint8
{
	Equal			UMETA(DisplayName="Equal To"),
	NotEqual		UMETA(DisplayName="Not Equal To"),
	Less			UMETA(DisplayName="Less Than"),
	LessOrEqual		UMETA(DisplayName="Less Than Or Equal To"),
	Greater			UMETA(DisplayName="Greater Than"),
	GreaterOrEqual	UMETA(DisplayName="Greater Than Or Equal To"),
};


UENUM()
enum class ELifeCycle : uint8
{
	Time = 0,
	TimeAutoDelete,
	Infinity,
};

