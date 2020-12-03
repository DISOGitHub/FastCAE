#ifndef DIALOGHULLCUT_H_
#define  DIALOGHULLCUT_H_

#include "HullPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"
namespace Ui
{
	class DialogHullCut;
}
namespace GUI
{
	class MainWindow;
}
namespace MainWidget
{
	class PreWindow;
}
class TopoDS_Shape;
namespace PluginShip
{

	class HULLPLUGINAPI HullCutDialog : public QDialog
	{
		Q_OBJECT
	public:
		HullCutDialog(GUI::MainWindow* m);
		~HullCutDialog();
		
	private:
		void init();
		void getModelShapePara(TopoDS_Shape* aShape);
		void accept() override;
		
	private slots:
		//void on_TypeChanged(int);
		void on_TypeChanged2(int);
	private:
		Ui::DialogHullCut* _ui{};
		QList<TopoDS_Shape*> _shapelist{};
		//bool _click{ false };
		int _geosetid{};
		GUI::MainWindow* _mainwindow;
		MainWidget::PreWindow* _prewindow;



	};
}

#endif