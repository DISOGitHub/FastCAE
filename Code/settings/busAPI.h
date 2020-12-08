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
		//获取单例指针
		static BusAPI* instance();
		//写出ini
		void writeINI();
		//设置工作路径
		void setWorkingDir();
// 		void setDesignModel(bool m);
// 		bool isDesignModel();
		//获取工作路径
		QString getWorkingDir();
//		void setSolver();
		//设置语言 “Chinese”  “English”
		void setLanguage(const QString lang);
		//获取当前语言
		QString getLanguage();
// 		SolverManager* getSolverManager();
// 		void setSolverOptions();
// 		SolveOption* getSolveOptions();
		//设置绘图选项
		void setGraphOptions();
		//获取绘图选项
		GraphOption* getGraphOption();
		//获取最近文件
		QStringList getRencetFiles();
		//添加最近文件
		void appendRecentFile(QString f);
		//设置已经加载的插件
		void setPlugins(QStringList p);
		//获取已经加载的插件
		QStringList getPlugins();
		//设置是否需要用户向导
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