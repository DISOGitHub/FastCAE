#ifndef XGENERATEREPORT_H
#define XGENERATEREPORT_H

#include "xgeneratereport_global.h"
#include <QThread>
#include <QTimer>
#include "XGenerateReportBase.h"
namespace XReport
{
	class XGENERATEREPORT_EXPORT XGenerateReportThread : public QThread
	{
		Q_OBJECT
	public:
		explicit XGenerateReportThread(XGenerateReportBase * generator = 0);
		~XGenerateReportThread();

	protected:
		void generatePictures(const QStringList & pictures);

		void run() override;

		void setGenerator(XGenerateReportBase * generator);

	Q_SIGNALS:
		void sig_generate_finished(bool,QString);

		private slots:
		void slot_generate_picture();
	private:
		XGenerateReportBase * m_generator{};
		QStringList m_pictures;
		QTimer * m_timer{};
	};
}


#endif // XGENERATEREPORT_H
