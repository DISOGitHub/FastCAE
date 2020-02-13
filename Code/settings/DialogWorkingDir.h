/**************************
设置工作目录对话框
libaojun
**************************/
#ifndef _GIALOGWORKINGDIR_H_
#define _GIALOGWORKINGDIR_H_

//#include "SelfDefObject/QFDialog.h"
#include <QDialog>

namespace Ui
{
	class DialogWorkingDir;
}
namespace GUI
{
	class MainWindow;
}

namespace Setting
{
class MainSetting;

class WorkingDirDialog : public QDialog
{
	Q_OBJECT
public:
	WorkingDirDialog(GUI::MainWindow* mw, MainSetting* setting);
	~WorkingDirDialog();
private:
	void init();
	void accept() override;
	void reject() override;
	void closeEvent(QCloseEvent *e);

private slots:
	void on_pushButton_clicked();

private:
	MainSetting *_setting{};
	Ui::DialogWorkingDir* _ui{};
};


}



#endif
