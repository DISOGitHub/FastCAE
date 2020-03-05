#include "QFDialog.h"
#include "mainWindow/mainWindow.h"


QFDialog::QFDialog(GUI::MainWindow* m, bool k) :_mainWindow(m), _keySig(k), QDialog(m)
{
	if (_keySig)
		connect(this, SIGNAL(endaleGraphWinKey(bool)), _mainWindow, SIGNAL(enableGraphWindowKeyBoard(bool)));

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
	if (_keySig)
		emit endaleGraphWinKey(false);
}
QFDialog::~QFDialog()
{
	if (_keySig)
		emit endaleGraphWinKey(true);
}

int QFDialog::exec()
{
	if (_keySig)
		emit endaleGraphWinKey(false);
	return QDialog::exec();
}
