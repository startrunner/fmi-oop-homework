#pragma once
#include <vector>

class NumberTransformation
{
public:
	NumberTransformation() {}

	virtual int transform(int number)const = 0;
	virtual NumberTransformation* clone()const = 0;
	virtual ~NumberTransformation() {}
};

class IdentityTransformation : public NumberTransformation
{
public:
	virtual int transform(int number) const override { return number; }

	virtual NumberTransformation* clone()const override
	{
		return new IdentityTransformation(*this);
	}
};


//Any binary operation
class OperationTransformation : public NumberTransformation
{
	int operand;

public:
	int transform(int number)const override
	{
		return operate(number, operand);
	}

protected:
	OperationTransformation(int operand) :operand(operand) {}
	OperationTransformation(const OperationTransformation &other) :operand(other.operand) {}

	OperationTransformation& operator=(const OperationTransformation &other)
	{
		if (this == &other)return *this;

		this->operand = other.operand;
		return *this;
	}

	virtual int operate(int x, int y) const = 0;
};

class SumTransformation : public OperationTransformation
{
public:
	SumTransformation() :SumTransformation(0) {}
	SumTransformation(int addition) :OperationTransformation(addition) {}
	SumTransformation(const SumTransformation &other) :OperationTransformation(other) {}

	SumTransformation& operator =(const SumTransformation &other)
	{
		OperationTransformation::operator=(other);
		return *this;
	}

	virtual NumberTransformation* clone()const override
	{
		return new SumTransformation(*this);
	}

protected:
	int operate(int x, int y)const override { return x + y; }

public:
	virtual ~SumTransformation() {}
};

class ProductTransformation : public OperationTransformation
{
public:
	ProductTransformation() :ProductTransformation(1) {}
	ProductTransformation(int multiplier) :OperationTransformation(multiplier) {}
	ProductTransformation(const ProductTransformation &other) :OperationTransformation(other) {}

	ProductTransformation& operator = (const SumTransformation &other)
	{
		OperationTransformation::operator=(other);
		return *this;
	}

	virtual ProductTransformation* clone()const override
	{
		return new ProductTransformation(*this);
	}

protected:
	int operate(int x, int y)const override { return x * y; }

public:
	virtual ~ProductTransformation() {}
};

class Calculator
{
	std::vector<NumberTransformation*> transformations;

public:
	Calculator() {}
	Calculator(const Calculator &other)
	{
		for (NumberTransformation *transformation : other.transformations)
		{
			transformations.push_back(transformation->clone());
		}
	}

	Calculator& operator =(const Calculator &other)
	{
		if (this == &other)return *this;

		for (NumberTransformation *transformation : transformations)delete transformation;
		transformations.clear();

		transformations.reserve(other.transformations.size());
		for (NumberTransformation *transformation : other.transformations)
		{
			transformations.push_back(transformation->clone());
		}

		return *this;
	}

	void add(const NumberTransformation *transformation)
	{
		transformations.push_back(transformation->clone());
	}

	int calculate(int number)const
	{
		for (NumberTransformation *transformation : transformations)
		{
			number = transformation->transform(number);
		}
		return number;
	}

	virtual ~Calculator()
	{
		for (NumberTransformation *transformation : transformations)delete transformation;
	}
};