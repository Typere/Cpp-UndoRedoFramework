/*
 * CompositeCommandTest.cpp
 *
 *  Created on: 31.10.2016
 *      Author: Gerd
 */

#include "../include/CompositeCommandTest.h"

#include <include/CompositeCommand.h>

void CompositeCommandTest::DoItCommand() {
	Plus::throwException() = false;
	CompositeCommand ccmd1(UndoRedoManagerImpl());

	ccmd.doIt(minus);
	ccmd.doIt(plus);
	urMngr.doIt(std::move(ccmd));

	int expected { plusValue - minusValue };
	ASSERT_EQUAL(expected, calculator.getResult());

//	urMngr.doIt(plus);
//
//	expected += plusValue;
//	ASSERT_EQUAL(expected, calculator.getResult());
}
void CompositeCommandTest::DoItCommandWithException() {
	Plus::throwException() = true;

	int expected { 0 };
	int result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	try{
		ccmd.doIt(minus);
		ccmd.doIt(minus);
		ccmd.doIt(plus);
	}catch(std::exception& e){}

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	urMngr.doIt(ccmd);

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
	urMngr.undo();

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
}
void CompositeCommandTest::testUndoRedoDoIt() {
	int expected { 0 };
	int result = calculator.getResult();

	ccmd.doIt(minus);
	ccmd.doIt(plus);
	urMngr.doIt(std::move(ccmd));

	expected = plusValue - minusValue;
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	urMngr.undo();

	expected = 0;
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
	urMngr.redo();

	expected = plusValue - minusValue;
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
}
void CompositeCommandTest::UndoWithException() {
	ccmd.doIt(minus);
	ccmd.doIt(plus); // throws in undo
	ccmd.doIt(minus);
	urMngr.doIt(std::move(ccmd));

	int expected { plusValue - (minusValue + minusValue) };
	int result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	Plus::throwException() = true;
	try{
		urMngr.undo();
	}catch(std::exception& e){}

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	Plus::throwException() = false;

	ASSERT_EQUAL(true, urMngr.isUndoable());
	urMngr.undo();

	expected = 0;
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	//check a second Exception
	Plus::throwException() = true;

	try{
		urMngr.redo();
	}catch(std::exception& e){}

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
}
void CompositeCommandTest::RedoWithException() {
	int expected = 0;
	ccmd.doIt(minus);
	ccmd.doIt(plus);
	ccmd.doIt(minus);
	urMngr.doIt(ccmd);

	expected = plusValue - (minusValue+minusValue);
	int result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	urMngr.undo();

	expected = 0;
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	Plus::throwException() = true;

	try{
		urMngr.redo();
	}catch(std::exception& e){}

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	Plus::throwException() = false;

	urMngr.redo();

	expected = plusValue - (minusValue+minusValue);
	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);

	Plus::throwException() = true;

	try{
		urMngr.undo();
	}catch(std::exception& e){}

	result = calculator.getResult();
	ASSERT_EQUAL(expected, result);
}
void CompositeCommandTest::DoItExceptionNeutral() {
	Plus::throwException() = true;
	ASSERT_THROWS(ccmd.doIt(plus), std::logic_error);
}
void CompositeCommandTest::UndoExceptionNeutral() {
	ccmd.doIt(plus);
	urMngr.doIt(std::move(ccmd));
	Plus::throwException() = true;

	ASSERT_THROWS(urMngr.undo(), std::logic_error);
}
void CompositeCommandTest::RedoExceptionNeutral() {
	ccmd.doIt(plus);
	urMngr.doIt(std::move(ccmd));
	urMngr.undo();
	Plus::throwException() = true;

	ASSERT_THROWS(urMngr.redo(), std::logic_error);
}

cute::suite CompositeCommandTest::make_suite(){
	cute::suite s { };
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, DoItCommand));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, DoItCommandWithException));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, testUndoRedoDoIt));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, UndoWithException));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, DoItExceptionNeutral));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, UndoExceptionNeutral));
	s.push_back(CUTE_SMEMFUN(CompositeCommandTest, RedoExceptionNeutral));
	return s;
}

