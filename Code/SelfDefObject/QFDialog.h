/*************
所有对话框基类
**************/
#ifndef _QFDIALOG_H_
#define _QFDIALOG_H_

#include <QDialog>
#include "SelfDefObjectAPI.h"

namespace GUI
{
	class MainWindow;
}


class SELFDEFINEOBJAPI QFDialog : public QDialog
{
	Q_OBJECT
public:
	QFDialog(GUI::MainWindow* m, bool key = true);
	~QFDialog();

	virtual int exec() override;

signals:
	void endaleGraphWinKey(bool on);

protected:
	GUI::MainWindow* _mainWindow{};
	bool _keySig{ true };
};



#endif