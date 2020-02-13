#ifndef SCALARBAREDITDOCKWIDGET_H
#define SCALARBAREDITDOCKWIDGET_H
#include <QDockWidget>
#include "global.h"
#include "scalarbareditform.h"
class scalarBarEditDockWidget : public QDockWidget
{
    Q_OBJECT
public:
	explicit scalarBarEditDockWidget(QWidget *parent = 0);
	ScalarBarEditForm *get_scalarBarEditForm() { return scalarBarEdit_Form; };
	virtual void reTranslate();
signals:
private:
	ScalarBarEditForm *scalarBarEdit_Form;
};

#endif // SCALARBAREDITDOCKWIDGET_H
