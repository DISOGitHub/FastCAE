#ifndef CUSTOMIZERHELPER_H_
#define CUSTOMIZERHELPER_H_

#include "mainWindowAPI.h"
#include <QObject>
#include <QList>

class QMenu;
class QToolBar;

namespace Ui
{
	class MainWindow;
}

namespace GUI
{
	class MainWindow;

	class MAINWINDOWAPI CustomizerHelper :  public QObject
	{
		Q_OBJECT
	public:
		CustomizerHelper(MainWindow* m, Ui::MainWindow* ui);
		~CustomizerHelper() = default;

		void startCustomizer();
		void finishCustomizer();

	public slots:
		/*根据配置文件开放接口 */
		void registerInterface();

	private:
		QString readConfigOptions();
		void updateBasicInfo();

		void enableGeoImport(bool on);
		void enableGeoExport(bool on);
		void enableGeoSketch(bool on);
		void enableGeoFeatureModeling(bool on);
		void enableGeoFeatureOperate(bool on);
		void enableGeoSelectAndView(bool on);

		void enableMeshImport(bool on);
		void enableMeshExport(bool on);
		void enableSolidMesh(bool on);
		void enableSurfaceMesh(bool on);
		void enableMeshCheck(bool on);
		void enableMeshComponent(bool on);
		void enableMeshSelectAndView(bool on);

	private:
		GUI::MainWindow* _mainWindow{};
		Ui::MainWindow* _ui{};

		QList<QMenu*> _menuList{};
		QList<QToolBar*> _toolBarList{};

	};
}



#endif