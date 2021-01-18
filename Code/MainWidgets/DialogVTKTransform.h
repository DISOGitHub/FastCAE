#ifndef _DIALOGVTKTRANSLATION_H_
#define _DIALOGVTKTRANSLATION_H_

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"

class QListWidgetItem;

namespace Ui
{
	class DialogVTKTransform;
	class DialogSelectComponents;
}

namespace MeshData
{
	class MeshSet;
}

namespace MainWidget
{
	class DialogSelectComponents;
	class MAINWIDGETSAPI DialogVTKTransform : public QFDialog
	{
		Q_OBJECT
	public:
		DialogVTKTransform(GUI::MainWindow* parent);
		~DialogVTKTransform();

	private:
		void removeCurrentItem(QListWidgetItem* curItem);

	private slots:
		void enabledCustomRotateAxisSlot();
		void disabledCustomRotateAxisSlot();
		virtual void accept();
		void selectedComponentsSlot(QList<MeshData::MeshSet*> components);
		void customContextMenuRequestedSlot(const QPoint& point);

	private:
		Ui::DialogVTKTransform* _ui;
		GUI::MainWindow* _mw;
		DialogSelectComponents* _selectdlg;
		QList<MeshData::MeshSet*> _components;
	};

	class MAINWIDGETSAPI DialogSelectComponents : public QFDialog
	{
		Q_OBJECT
	public:
		DialogSelectComponents(GUI::MainWindow* parent);
		~DialogSelectComponents();
		void clearSelectItems();

	signals:
		void selectedComponentsSig(QList<MeshData::MeshSet*> components);

	private slots:
		virtual void accept();

	private:
		Ui::DialogSelectComponents* _ui;
	};
}
#endif