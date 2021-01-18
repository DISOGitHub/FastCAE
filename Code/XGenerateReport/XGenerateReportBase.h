#ifndef XGENERATEREPORTBASE_H
#define XGENERATEREPORTBASE_H

#include <QObject>
#include "XOperateWord.h"
#include "xgeneratereport_global.h"

namespace XReport{

	class XGENERATEREPORT_EXPORT XGenerateReportBase : public QObject
	{
		Q_OBJECT
	public:
		explicit XGenerateReportBase();
		~XGenerateReportBase();
	
		virtual bool generate(QString  info = QString()) = 0;

		virtual bool generatePicture(const QString  file) = 0;
	private:
	};
}


#endif // XGENERATEREPORTBASE_H
