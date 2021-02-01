#include "ValidatorName.h"
#include <QRegExp>



namespace FastCAEDesigner
{
	ValidatorName::ValidatorName(QObject *parent) : QObject(parent)
	{

	}

	bool ValidatorName::FileNameIsAllow(QString fileName)
	{
		if (fileName.isEmpty())
			return false;

		bool isOk = true;
		QString pattern = "[\\\\/:*?\"<>., ;'{|`~!@#$%&*()=+\\^}\\[\\]]";

		QRegExp rx(pattern);
		int match = fileName.indexOf(rx);

		if (match >= 0)
			isOk = false;

		return isOk;
	}
}
 