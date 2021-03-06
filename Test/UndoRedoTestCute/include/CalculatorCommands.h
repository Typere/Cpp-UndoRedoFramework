#ifndef CALCULATOR_COMMANDS
#define CALCULATOR_COMMANDS

#include <include/Command.h>
#include "Calculator.h"

#include <iostream>
#include <stdexcept>


class Plus : public Command
{
public:
	Plus(Calculator& calculator, int summand) :
		calculator(calculator), summand(summand){
		throwException() = false;
		throwAtTimes() = 0;
	}

//	Plus(const Plus& source) :
//		calculator(source.calculator), summand(source.summand) {}

	virtual void doIt() override
	{
		if(throwException()) {
			if(throwAtTimes() <= 0)
				throw std::logic_error("Plus.doIt() throws");
			else
				--throwAtTimes();
		}

//		std::cout << "Plus.doIt()" << std::endl;
		calculator.plus(summand);
	}
	virtual void undo() override
	{
		if(throwException()) {
			if(throwAtTimes() <= 0)
				throw std::logic_error("Plus.doIt() throws");
			else
				--throwAtTimes();
		}
		calculator.minus(summand);
	}

	virtual std::unique_ptr<Command> clone() const & override
	{
		using namespace std;
		return std::unique_ptr<Command>( new Plus(*this));
	}

	virtual std::unique_ptr<Command> clone() && override
	{
		using namespace std;
//		cout << "PlusCommand::clone() &&" << endl;
		return std::unique_ptr<Command>( new Plus( std::move(*this) ));
	}
	static bool& throwException(){
		static bool b = false;
		return b;
	}
	static int& throwAtTimes(){
		static int times = 0;
		return times;
	}
private:
	Calculator& calculator;
	int summand;
};

class Minus : public Command
{
public:
	Minus(Calculator& calculator, int subtrahent):calculator(calculator), subtrahent(subtrahent){}
	virtual void doIt() override
	{
		calculator.minus(subtrahent);
	}
	virtual void undo() override
	{
		calculator.plus(subtrahent);
	}

	virtual std::unique_ptr<Command> clone() const& override
	{
		using namespace std;
//		cout << "MinusCommand::clone() const&" << endl;
		return std::unique_ptr<Command>( new Minus( std::move(*this) ));
	}
	virtual std::unique_ptr<Command> clone() && override
	{
		using namespace std;
//		cout << "MinusCommand::clone() &&" << endl;
		return std::unique_ptr<Command>( new Minus( std::move(*this) ));
	}
private:
	Calculator& calculator;
	int subtrahent;
};

#endif //CALCULATOR_COMMANDS
