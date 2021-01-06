#pragma once
#include "QColor"
class QSettings;

namespace Setting
{
	class MessageSetting
	{
	public:
		MessageSetting() = default;
		~MessageSetting() = default;

		void writeINI(QSettings* setting);
		void readINI(QSettings* setting);

		int getShowMode() const;

		void setShowMode(int showMode);

		bool getShowType() const;

		void setShowType(bool showType);

		bool getShowTime() const;

		void setShowTime(bool showTime);

		int getFontPts() const;

		void setFontPts(int fontPts);

		QColor getBkColor() const;

		void setBkColor(const QColor & bkColor);

	private:
		///<MG setting if show normal . warning . error message
		int m_showMode;

		///<MG if show the message type
		bool m_showType;

		///<MG if show the message time
		bool m_showTime;

		///<MG font size in pts
		int m_fontPts;

		///<MG background color of the message window
		QColor m_bkColor;
	};
}


