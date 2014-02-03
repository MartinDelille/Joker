#ifndef PHSTRIPDOCTEST_H
#define PHSTRIPDOCTEST_H

#include <QtTest>

#include "PhStrip/PhStripDoc.h"

class PhStripDocTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Open DetX tests
	void openDetXHeaderTest();
	void openDetXPeopleTest();
	void openDetXLoopTest();
	void openDetXCutTest();
	void openDetXTextTest();
	void openDetXOffTest();

	// Get people
	void getPeopleByNameTest();

	// Get prev/next element tests
	void getPreviousElementFrameTest();
	void getNextElementFrameTest();
	void getNextTextTest();
	void getNextTextTestByPeople();
	void getNextTextTestByPeopleList();

private:
	QString f2s(PhFrame frame);
	PhFrame s2f(QString string);

	PhStripDoc _doc;
};

#endif // PHSTRIPDOCTEST_H
