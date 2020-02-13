#include "MessageSetting.h"
#include <QSettings>

namespace Setting {
	void MessageSetting::writeINI(QSettings* setting)
	{
		QString showModeStr;
		if (m_showMode & 0x01)
		{ ///<MG show Normal
			showModeStr.append("Normal:Yes|");
		}
		else
			showModeStr.append("Normal:No|");

		if (m_showMode & 0x02)
		{ ///<MG show Warning
			showModeStr.append("Warning:Yes|");
		}
		else
			showModeStr.append("Warning:No|");

		if (m_showMode & 0x04)
		{ ///<MG show Error
			showModeStr.append("Error:Yes");
		}
		else
			showModeStr.append("Error:No");

		
		setting->setValue("/Message/BkColor", QString("red:%1|green:%2|blue:%3").arg(m_bkColor.red()).arg(m_bkColor.green()).arg(m_bkColor.blue()));
		setting->setValue("/Message/ShowMode", showModeStr);
		setting->setValue("/Message/ShowType", m_showType ? "Yes" : "No");
		setting->setValue("/Message/ShowTime", m_showTime ? "Yes" : "No");
		setting->setValue("/Message/FontPts", QString::number(m_fontPts));

	}

	void MessageSetting::readINI(QSettings* setting)
	{
		m_showMode = 0;
		QStringList showModeStrList = setting->value("/Message/ShowMode").toString().split("|", QString::SkipEmptyParts);
		if (showModeStrList.size() == 3)
		{
			if (showModeStrList[0].toLower().contains("normal"))
			{
				if (showModeStrList[0].toLower().contains("yes"))
				{
					m_showMode = m_showMode | 0x01;
				}
				else
					m_showMode = m_showMode | 0x00;
			}
			if (showModeStrList[1].toLower().contains("warning"))
			{
				if (showModeStrList[1].toLower().contains("yes"))
				{
					m_showMode = m_showMode | 0x02;
				}
				else
					m_showMode = m_showMode | 0x00;
			}
			if (showModeStrList[2].toLower().contains("error"))
			{
				if (showModeStrList[2].toLower().contains("yes"))
				{
					m_showMode = m_showMode | 0x04;
				}
				else
					m_showMode = m_showMode | 0x00;
			}
		}

		m_showType = setting->value("/Message/ShowType").toString().toLower() == "yes";
		m_showTime = setting->value("/Message/ShowTime").toString().toLower() == "yes";
		m_fontPts = setting->value("/Message/FontPts").toInt();

		QStringList bkColorList = setting->value("/Message/BkColor").toString().toLower().split("|", QString::SkipEmptyParts);
		if (bkColorList.size() == 3)
		{
			for (int iCnt = 0; iCnt < 3; ++iCnt)
			{
				QStringList strValue = bkColorList.at(iCnt).split(":", QString::SkipEmptyParts);
				if (strValue.size() == 2)
				{
					int colorValue = strValue.at(1).toInt();
					if (iCnt == 0)
						m_bkColor.setRed(colorValue);
					else if (iCnt == 1)
						m_bkColor.setGreen(colorValue);
					else
						m_bkColor.setBlue(colorValue);
				}
				else
				{
					m_bkColor = QColor(0, 0, 0);
					break;
				}
			}
		}
		else
			m_bkColor = QColor(0, 0, 0);
	}

	int MessageSetting::getShowMode() const
	{
		return m_showMode;
	}

	void MessageSetting::setShowMode(int showMode)
	{
		m_showMode = showMode;
	}

	bool MessageSetting::getShowType() const
	{
		return m_showType;
	}

	void MessageSetting::setShowType(bool showType)
	{
		m_showType = showType;
	}

	bool MessageSetting::getShowTime() const
	{
		return m_showTime;
	}

	void MessageSetting::setShowTime(bool showTime)
	{
		m_showTime = showTime;
	}

	int MessageSetting::getFontPts() const
	{
		return m_fontPts;
	}

	void MessageSetting::setFontPts(int fontPts)
	{
		m_fontPts = fontPts;
	}

	QColor MessageSetting::getBkColor() const
	{
		return m_bkColor;
	}

	void MessageSetting::setBkColor(const QColor & bkColor)
	{
		m_bkColor = bkColor;
	}

}
