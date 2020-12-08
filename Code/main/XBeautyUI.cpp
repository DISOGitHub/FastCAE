#include "XBeautyUI.h"
#include <QStyleFactory>
#include <QApplication>

XBeautyUI * XBeautyUI::_instance = NULL;

XBeautyUI * XBeautyUI::instance()
{
	if (!_instance)
	{
		_instance = new XBeautyUI();
	}
	return _instance;
}

QString XBeautyUI::qssFilePath() const
{
	return m_qssFilePath;
}

void XBeautyUI::setQssFilePath(const QString & _qssFilePath)
{
	m_qssFilePath = _qssFilePath;
}

QString XBeautyUI::welcomePictureName() const
{
	return m_welcomePictureName;
}

void XBeautyUI::setWelcomePictureName(const QString & _welcomePictureName)
{
	m_welcomePictureName = _welcomePictureName;
}

QString XBeautyUI::currentStyle() const
{
	return m_currentStyle;
}

void XBeautyUI::setCurrentStyle(const QString & style)
{
	m_currentStyle = style;
	qApp->setStyle(style);
}

QStringList XBeautyUI::getStyles() const
{
	return QStyleFactory::keys();
}

void XBeautyUI::autoSetStyle() const
{
	QStringList styles = getStyles();
	if (styles.contains("Fusion"))
	{
		qApp->setStyle("Fusion");
		return;
	}
	else if (styles.contains("WindowsVista"))
	{
		qApp->setStyle("WindowsVista");
		return;
	}
	else if (styles.contains("WindowsXP"))
	{
		qApp->setStyle("WindowsXP");
		return;
	}
	else if (styles.contains("Windows"))
	{
		qApp->setStyle("Windows");
		return;
	}
}

int XBeautyUI::welcomePictureRemainTime() const
{
	return m_welcomeRemainTime;
}

void XBeautyUI::setWelcomePictureRemainTime(int time /*= 1500*/)
{
	m_welcomeRemainTime = time;
}

XBeautyUI::XBeautyUI()
{
}


XBeautyUI::~XBeautyUI()
{
}
