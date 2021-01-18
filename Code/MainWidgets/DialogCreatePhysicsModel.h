#ifndef CREATEPHYSICSMODEL_H
#define CREATEPHYSICSMODEL_H

#include <QMap>
#include "DataProperty/modelTreeItemType.h"
#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class DialogCreateModel;
}
namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PhysicsWidget;


// 	typedef struct tag_AddItem
// 	{
// 		QString itemName;
// 		QString itemDescInfo;
// 		int itemType; ///<MG 0 LineEdit 1 TextEdit
// 
// 		tag_AddItem(const QString & name, const QString & desc, int type)
// 		{
// 			itemName = name;
// 			itemDescInfo = desc;
// 			itemType = type;
// 		}
// 	}AddItem;
// 
// 	typedef QVector<AddItem> VecAddItem;

	class CreatePhysicsModel : public QFDialog
	{
		Q_OBJECT

	public:
		CreatePhysicsModel(PhysicsWidget* physicsW, GUI::MainWindow* m);
//		CreatePhysicsModel(const VecAddItem & items, PhysicsWidget* physicsW, GUI::MainWindow* m);
		~CreatePhysicsModel();
//		QString getControlText(const QString & name);
//		QString getSelectedTreeTypeName() const;
		ProjectTreeType getSelectedTreeType() const;

	private:
		void init(/*onst VecAddItem & items = VecAddItem()*/);
		void accept() override;

	private:
		Ui::DialogCreateModel* _ui{};
		PhysicsWidget* _physicsWidget{};
// 		QMap<QString, QWidget*> _subWidgets;
// 		QMap<QString, QString> _subWidgetsText;
	};
}

#endif
