#ifndef VALIDATORNAME_H
#define VALIDATORNAME_H

#include <QObject>

namespace FastCAEDesigner
{
	class ValidatorName : public QObject
	{
		Q_OBJECT
	public:
		ValidatorName(QObject *parent = 0);

	signals:

		public slots :
	public:
		static bool FileNameIsAllow(QString fileName);

	};
}
#endif // VALIDATORNAME_H
