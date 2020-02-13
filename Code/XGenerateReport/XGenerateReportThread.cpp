#include "XGenerateReportThread.h"
#include <QDir>
namespace XReport{

	XGenerateReportThread::XGenerateReportThread(XGenerateReportBase * generator /*= 0*/)
		:m_generator(generator)
	{

	}

	XGenerateReportThread::~XGenerateReportThread()
	{

	}

	void XGenerateReportThread::generatePictures(const QStringList & pictures)
	{
		m_pictures = pictures;
		if (!m_timer)
		{
			m_timer = new QTimer();
			connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_generate_picture()));
		}
	}

	void XGenerateReportThread::run()
	{
		if (m_generator)
		{
			QString info;
			bool bResult = m_generator->generate(info);
			emit sig_generate_finished(bResult, info);
		}
	}

	void XGenerateReportThread::slot_generate_picture()
	{
		if (m_pictures.isEmpty())
		{
			m_timer->stop();
			return;
		}
		QString file = m_pictures.at(0);
		bool bResult = m_generator->generatePicture(file);
		m_pictures.removeOne(file);
	}

}
