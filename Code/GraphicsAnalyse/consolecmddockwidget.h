#ifndef CONSOLECMDDOCKWIDGET_H
#define CONSOLECMDDOCKWIDGET_H

#include <QDockWidget>
#include "global.h"
#include "qtextedit.h"
//#include "sizeDockWidget.h"
class consoleCmdDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit consoleCmdDockWidget(QWidget *parent = 0);
	virtual void reTranslate();
	void setDockSize(int w, int h)
	{
		wid = w;
		hei = h;
		//sizeHint();
		cmd_edit->resize(w, h);
	}
	QSize sizeHint() const
	{
		return QSize(wid, hei);
	}
	QTextEdit *cmd_edit;
public slots:
	void slot_update_clear_console_command();
	void slot_update_console_cmd_form(QString cmd_str);
	void slot_addInfoToConsole(QString infoStr);
	void DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg);
signals:
private:
	int wid, hei;
};

#endif // CONSOLECMDDOCKWIDGET_H
