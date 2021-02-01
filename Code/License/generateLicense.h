#pragma once
#include <QDate>
#include <QObject>
#include <QStringList>

class generateLicense : public QObject
{
	Q_OBJECT
public:
	generateLicense();
	~generateLicense();

	///<MG generate information to string
	QString generate(QString info, int totalLength, char endCharter, int interval = 1, int smallPos = 0, int bigPos = 0);

	///<MG decode string to information
	QString decode(QString info, char endCharter, int interval = 1, int smallPos = 0, int bigPos = 0);

	///<MH generate time
	QString generate(const QDate & date, const QTime & time, int skip = 0, int interval = 1) const;

	///<MG decode string to QDate
	QDate decode(QString info, int skip = 0, int interval = 1);
private:
	///<MG random a char 
	char randomChar(bool over, char endCharter) const;
};

