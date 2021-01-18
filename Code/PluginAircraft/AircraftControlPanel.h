#ifndef AIRCRAFTCONTROLPANEL_H_
#define AIRCRAFTCONTROLPANEL_H_

#include "AircraftPluginAPI.h"
#include "moduleBase/dockWidgetBase.h"
#include <QTreeWidget>
#include <QDialog>

class QTreeWidgetItem;


namespace DataProperty
{
	class DataBase;
}

namespace Aircraft
{
	
	class MaterialManager;
	
	class  AircraftControlPanel :  public QTreeWidget
	{
		Q_OBJECT
	public:
		AircraftControlPanel(GUI::MainWindow* m);
		~AircraftControlPanel();
		
	signals:
		/*更新属性框 */
		void updateProperty(DataProperty::DataBase* data);

		void updateGeant4Property(int type);

		void updateGeant4Property(int type, QTreeWidgetItem * item);

		void clean();

	private:
		void init();
		void initProjectTree();
		//右键菜单
		void contextMenuEvent(QContextMenuEvent *event) override;
		void showDialog(QDialog* d);

	private slots:
		//鼠标左键单击事件处理
		void singleClicked(QTreeWidgetItem*item, int i);
		void SolveCalculate();

	private:
		GUI::MainWindow* _mainWindow{};

		QTreeWidgetItem* _root{};

		QTreeWidgetItem * _AircraftPara{};
		QTreeWidgetItem * _AerodynamicPara{};
		QTreeWidgetItem * _EnginePara{};
		
		
	};
}



#endif