#ifndef PLUGINBASE_H
#define PLUGINBASE_H

#include <QObject>
#include "PluginManagerAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace GUI
{
	class MainWindow;
}

class QTranslator;
class QSettings;
class QDomDocument;
class QDomElement;

namespace Plugins
{
	enum PluginType
	{
		None,
		IOExtend,
		UserTool,
		ModelExtend,
		Mesher,
	};

	class PLUGINMANAGERAPI PluginBase : public QObject
	{
		Q_OBJECT
	public:
		PluginBase();
		~PluginBase();
		//���ز��
		virtual bool install() = 0;
		//ж�ز��
		virtual bool uninstall() = 0;
		//����
		virtual void reTranslate(QString lang);
		//ִ�в��
		virtual void exec(int commandType = 0);
		//��������
		void setType(PluginType t);
		//��ȡ����
		PluginType getType();
		//д�������ļ�
		virtual void writeToProjectFile(QDomDocument* doc, QDomElement* parent);
		//�ӹ����ļ�����
		virtual void readFromProjectFile(QDomElement* parentEle);
		//�Ƿ�����Ϣ��Ҫ���浽�����ļ�
		virtual bool hasInfoToSave();

		void setFileName(QString f);
		QString getFileName();
		QString getDescribe();
		PluginType getPluginType();

#ifdef Q_OS_WIN
		void setWinModule(HMODULE m);
		HMODULE getWinModule();
#endif

#ifdef Q_OS_LINUX
		void setLinuxModule(void* m);
		void* getLinuxModule();

#endif

	protected:
		virtual void readINI(QSettings* settings);
		virtual void writeINI(QSettings* settings);

	protected:
		QString _libFileName{};
#ifdef Q_OS_WIN
		HMODULE _winModule{};
#endif
#ifdef Q_OS_LINUX
		void* _linuxModule{};
#endif
		QString _describe{};
		QTranslator* _translator{};

		PluginType _pluginType{ None };
	};
}



#endif