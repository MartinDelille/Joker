#ifndef PHTIMECODEEDITTEST_H
#define PHTIMECODEEDITTEST_H

#include <QObject>

class TimeCodeEditTest : public QObject
{
	Q_OBJECT
public:
	explicit TimeCodeEditTest();

private slots:
	void timeTest();
	void setTimeTest();

	void keyboardInputTest();
	void keyboardBadInputTest();

	void mouseInputTest();
};

#endif // PHTIMECODEEDITTEST_H
