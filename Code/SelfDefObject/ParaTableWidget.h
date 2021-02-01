#ifndef _PARATABLEWIDGET_H_
#define _PARATABLEWIDGET_H_

#include <QTableWidget>
#include "SelfDefObjectAPI.h"

namespace GUI
{
	class MainWindow;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaTableWidget : public QTableWidget
	{
		Q_OBJECT
	public:
		ParaTableWidget(GUI::MainWindow* m);
		~ParaTableWidget() = default;

	signals:
		void lockGraphFocus(bool);

	protected slots:
		void horizontalMenu(QPoint p);
		void verticalMenu(QPoint p);
		void insertRowAbove();
		void insertRowBelow();
		void insertColumnLeft();
		void insertColumnRight();
		void deleteRow();
		void deleteColumn();
		void renameColumn();

	protected:
		void focusInEvent(QFocusEvent *event) override;

	protected:
		int _currentColumn{ -1 };
		int _currentRow{ -1 };
		GUI::MainWindow* _mainWindow{};

	};


}



#endif