#pragma once
#include <QObject>
#include <QStringList>

class XBeautyUI : public QObject
{
Q_OBJECT

public: 
	static XBeautyUI * instance();

	QString qssFilePath() const;

	void setQssFilePath(const QString & _qssFilePath);

	QString welcomePictureName() const;

	void setWelcomePictureName(const QString & _welcomePictureName);

	QString currentStyle() const;

	void setCurrentStyle(const QString & style);

	QStringList getStyles() const;

	void autoSetStyle() const;

	int welcomePictureRemainTime() const;

	void setWelcomePictureRemainTime(int time = 1500);
private:
	XBeautyUI();
	~XBeautyUI();


private:
	static XBeautyUI * _instance;

	QString m_welcomePictureName;

	QString m_qssFilePath;

	QString m_currentStyle;

	int m_welcomeRemainTime;
};

