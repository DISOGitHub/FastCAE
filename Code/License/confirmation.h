#ifndef CONFIRMATION_H
#define CONFIRMATION_H

#include <QDialog>


#if defined(LICENSE_LIB)
#define LICENSE_LIB Q_DECL_EXPORT
#else
#define  LICENSE_LIB Q_DECL_IMPORT
#endif

namespace Ui
{
	class Confirmation;
}

class LICENSE_LIB Confirmation : public QDialog
{
	Q_OBJECT

public:
	Confirmation(QWidget *parent = 0);
	~Confirmation();

	bool licenseCheck();

private:
	Ui::Confirmation * ui;

private:
	void initilize();

	///<MG check weather this is a validate license context
	bool check(QString license);

	private slots:
	///<MG slot while user clicked buy button
	void slot_clicked_buy_button();

	///<MG slot while user clicked confirm button
	void slot_clicked_confirm_button();

	///<MG slot while user clicked path button
	void slot_clicked_path_button();

	///<MG slot while user clicked radio button
	void slot_clicked_radio_button();
};

#endif // CONFIRMATION_H
