#ifndef _SETTINGBUSAPI_H_
#define _SETTINGBUSAPI_H_

#include <QString>
#include <QStringList>
#include <QColor>
#include "settingAPI.h"

namespace GUI
{
	class MainWindow;
}

namespace Setting
{
	class MainSetting;
// 	class SolverManager;
// 	class SolveOption;
	class GraphOption;
	class MessageSetting;

	class SETTINGAPI BusAPI
	{
	public:
		//��ȡ����ָ��
		static BusAPI* instance();
		//д��ini
		void writeINI();
		//���ù���·��
		void setWorkingDir();
// 		void setDesignModel(bool m);
// 		bool isDesignModel();
		//��ȡ����·��
		QString getWorkingDir();
//		void setSolver();
		//�������� ��Chinese��  ��English��
		void setLanguage(const QString lang);
		//��ȡ��ǰ����
		QString getLanguage();
// 		SolverManager* getSolverManager();
// 		void setSolverOptions();
// 		SolveOption* getSolveOptions();
		//���û�ͼѡ��
		void setGraphOptions();
		//��ȡ��ͼѡ��
		GraphOption* getGraphOption();
		//��ȡ����ļ�
		QStringList getRencetFiles();
		//�������ļ�
		void appendRecentFile(QString f);
		//�����Ѿ����صĲ��
		void setPlugins(QStringList p);
		//��ȡ�Ѿ����صĲ��
		QStringList getPlugins();
		//�����Ƿ���Ҫ�û���
		void isEnableUserGuidence(bool s);
		bool isEnableUserGuidence();


		QString getLicensePath() const;
		void setLicensePath(const QString & licensePath);

		GUI::MainWindow* getMainWindow();

		void setMainWindow(GUI::MainWindow* mw);
		///<MG
		int getMessageShowMode() const;

		void setMessageShowMode(int showMode);

		bool getMessageShowType() const;

		void setMessageShowType(bool showType);

		bool getMessageShowTime() const;

		void setMessageShowTime(bool showTime);

		int getMessageFontPts() const;

		void setMessageFontPts(int fontPts);

		QColor getMessageBkColor() const;

		void setMessageBkColor(const QColor & bkColor);

	private: 
		BusAPI();
		~BusAPI();
		void readINI();

	private:
		static BusAPI* _ins;
		MainSetting* _mainSetting{};
// 		SolverManager* _solvers{};
// 		SolveOption* _solveOption{};
		GraphOption* _graphOption{};
		MessageSetting * _messageSetting{};

		GUI::MainWindow* _mainWindow{};
//		bool _isDesignModel{ false };

	};
}


#endif