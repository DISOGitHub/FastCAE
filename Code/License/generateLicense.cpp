#include "generateLicense.h"
#include <time.h>

generateLicense::generateLicense()
{
	srand((unsigned)time(NULL));
}


generateLicense::~generateLicense()
{
}

QString generateLicense::generate(QString info, int totalLength, char endCharter, int interval /*= 1*/, int smallPos /*= 0*/, int bigPos /*= 0*/)
{
	int iNeedLength = info.size() * (interval + 1);
	if (iNeedLength > totalLength){ return QString(""); }

	QString result;
	for (int iCnt = 0; iCnt < info.size(); ++iCnt)
	{
		if (smallPos == 0)
		{///<MG information charter random charter
			result.append(info.at(iCnt));
			for (int iSub = 0; iSub < interval; ++iSub)
			{
				result.append(randomChar(false, endCharter));
			}
		}
		else
		{
			for (int iSub = 0; iSub < interval; ++iSub)
			{
				result.append(randomChar(false, endCharter));
			}
			result.append(info.at(iCnt));
		}
	}
	result.append(endCharter);
	int vaildateLength = result.size();
	for (int iCnt = 0; iCnt < totalLength - vaildateLength; ++iCnt)
	{
		result.append(randomChar(true, endCharter));
	}

	if (bigPos)
	{
		QString temp;
		for (int iCnt = 0; iCnt < result.size(); ++iCnt)
		{
			temp.append(result.at(result.size() - iCnt - 1));
		}
		result = temp;
	}
	return result;
}

QString generateLicense::generate(const QDate & date, const QTime & time, int skip /*= 0*/, int interval /*= 0*/) const
{
	if (interval * 9 + skip > 95 )
	{
		return QString("");
	}
	QString realDate = date.toString("yyyyMMdd");
	
	QString result;
	for (int iCnt = 0; iCnt < realDate.size(); ++iCnt)
	{
		int iValue = realDate.at(iCnt).digitValue();
		result.append(QChar(iValue * interval + skip + 32));
	}
	return result;
}

QString generateLicense::decode(QString info, char endCharter, int interval /*= 1*/, int smallPos /*= 0*/, int bigPos /*= 0*/)
{
	if (bigPos)
	{
		QString temp;
		for (int iCnt = 0; iCnt < info.size(); ++iCnt)
		{
			temp.append(info.at(info.size() - iCnt - 1));
		}
		info = temp;
	}

	QString result;
	int blockSize = interval + 1;

	QStringList vaildateInfo;
	for (int iCnt = 0; iCnt < info.size(); ++iCnt)
	{
		if (blockSize + iCnt *blockSize > info.size())
		{
			return QString("");
		}
		QString temp = info.mid(iCnt * blockSize, blockSize);

		//if ((smallPos == 0 && temp.at(0).toLatin1() == endCharter) ||
		//	(smallPos != 0 && temp.at(blockSize - 1).toLatin1() == endCharter));
		//{
		//	break;
		//}

		if (temp.contains(endCharter))
		{
			break;
		}
		vaildateInfo.push_back(temp);
	}

	if (smallPos)
	{
		for (QString oneChar : vaildateInfo)
		{
			result.append(oneChar.at(blockSize - 1));
		}
	}
	else
	{
		for (QString oneChar :vaildateInfo)
		{
			result.append(oneChar.at(0));
		}
	}
	return result;
}

QDate generateLicense::decode(QString info, int skip /*= 0*/, int interval /*= 1*/)
{
	QDate date(1937,1,1);
	if (info.size() < QString("yyyyMMdd").size())
	{
		return date;
	}
	QString result;
	for (int iCnt = 0; iCnt < info.size();++iCnt)
	{
		QChar temp = info.at(iCnt);
		int value = (int)temp.toLatin1();
		QChar real = ((value - 32 - skip) / interval) + 0x30;
		result.append(real);
	}
	return QDate::fromString(result, "yyyyMMdd");
}

char generateLicense::randomChar(bool over, char endCharter) const
{
	char cRand;
	if (over)
	{
		cRand = rand() % 95 + 32;
	}
	else
	{
		while ((cRand = rand() % 95 + 32) == endCharter)
		{
			continue;
		}
	}
	return cRand;
}
