#ifndef CUSTOMIZERHELPER_H_
#define CUSTOMIZERHELPER_H_

#include "mainWindowAPI.h"
#include <QObject>
#include <QList>

class QMenu;
class QToolBar;
class SARibbonPannel;
class SARibbonCategory;

namespace Ui
{
	class MainWindowRibbon;
}

namespace GUI
{
	class MainWindow;

	class MAINWINDOWAPI CustomizerHelper :  public QObject
	{
		Q_OBJECT
	public:
		CustomizerHelper(MainWindow* m, Ui::MainWindowRibbon* ui);
		~CustomizerHelper() = default;

		void startCustomizer();
		void finishCustomizer();

	public slots:
		/*根据配置文件开放接口 */
		void registerInterface();

	private:
		bool isUseRibbon() const;
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
		Ui::MainWindowRibbon* _ui{};

		QList<QMenu*> _menuList{};
		QList<QToolBar*> _toolBarList{};
	};
}



#endif