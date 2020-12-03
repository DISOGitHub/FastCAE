#ifndef _GMESHMANAGER_H_
#define _GMESHMANAGER_H_

#include <QObject>
#include <QHash>
#include "GmshModuleAPI.h"

class QWidget;

namespace GUI
{
	class MainWindow;
}


namespace Gmsh
{
	class GmshThread;

	class GMSHAPI GmshThreadManager : public QObject
	{
		Q_OBJECT
	public:
		GmshThreadManager(GUI::MainWindow* mw);
		~GmshThreadManager() = default;

		void insertThread(QWidget* w, GmshThread* t);
		void stopAll();
		bool isRuning();


	signals:
		void threadStarted(QWidget*);

	private slots:
		void stopThread(QWidget* w);
		void threadFinished(GmshThread* t);

	private:
		QHash<QWidget*, GmshThread*> _threadHash{};
	};
}




#endif