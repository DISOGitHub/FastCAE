#include "consolecmddockwidget.h"
#include "qmessagebox.h"
consoleCmdDockWidget::consoleCmdDockWidget(QWidget *parent) : QDockWidget(parent)
{
	this->setWindowTitle(tr("Console"));
	cmd_edit = new QTextEdit(this);
	setWidget(cmd_edit);
	cmd_edit->setReadOnly(true);
	wid = 800;
	hei = 200;
}

void consoleCmdDockWidget::slot_update_console_cmd_form(QString cmd_str)
{
	cmd_edit->append(cmd_str);
}

void consoleCmdDockWidget::slot_update_clear_console_command()
{
	cmd_edit->clear();
}

void consoleCmdDockWidget::DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg)
{
	if (onDlg == true)
	{
		if (type != kMsg_Info)
			QMessageBox::warning(0, title, message);
		else
			QMessageBox::information(0, title, message);

	}

	if (type == kMsg_Error)
		cmd_edit->setTextColor(QColor(255, 0, 0));
	else if (type == kMsg_Warning)
		cmd_edit->setTextColor(QColor(0, 0, 255));
	else
		cmd_edit->setTextColor(QColor(0, 0, 0));
	cmd_edit->append(message);
}

void consoleCmdDockWidget::slot_addInfoToConsole(QString infoStr)
{
	cmd_edit->append(infoStr);
}

void consoleCmdDockWidget::reTranslate()
{
	this->setWindowTitle(tr("Console"));
}