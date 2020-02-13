#include "curvetitlepropdialog.h"
#include "ui_curvetitlepropdialog.h"
#include "qcolordialog.h"

curveTitlePropDialog::curveTitlePropDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curveTitlePropDialog)
{
    ui->setupUi(this); 
	
}

void curveTitlePropDialog::initForm()
{
	ui->title_name_lineEdit->setStyleSheet("QLineEdit{border-width:1;border-style:outset}");
	ui->title_name_lineEdit->setText(curve_data_ptr->glo_curve_data.title);

	ui->title_fontType_comboBox->setCurrentIndex(curve_data_ptr->glo_curve_data.title_font.font_type);

	QColor tep_color = curve_data_ptr->glo_curve_data.title_font.font_color;
	ui->title_fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(tep_color.red()).arg(tep_color.green()).arg(tep_color.blue()));

	ui->title_fontSize_spinBox->setValue(curve_data_ptr->glo_curve_data.title_font.font_size);
}

curveTitlePropDialog::~curveTitlePropDialog()
{
    delete ui;
}

void curveTitlePropDialog::on_title_name_lineEdit_textChanged(QString text)
{
	curve_data_ptr->glo_curve_data.title = text;
	emit sig_setTitle();
}

void curveTitlePropDialog::on_title_fontSize_spinBox_valueChanged(int index)
{
	curve_data_ptr->glo_curve_data.title_font.font_size = index;
	emit sig_setTitle();
}

void curveTitlePropDialog::on_title_fontType_comboBox_currentIndexChanged(int index)
{
	if (index < 0)
		return;
	curve_data_ptr->glo_curve_data.title_font.font_type = index;
	emit sig_setTitle();
}

void curveTitlePropDialog::on_title_fontColor_pushButton_clicked()
{
	QColor init_color0 = curve_data_ptr->glo_curve_data.title_font.font_color;
	QColor tep_color = QColorDialog::getColor(init_color0, this, tr("color dialog"));
	if (!tep_color.isValid())
		return;
	curve_data_ptr->glo_curve_data.title_font.font_color = tep_color;

	int r = curve_data_ptr->glo_curve_data.title_font.font_color.red();
	int g = curve_data_ptr->glo_curve_data.title_font.font_color.green();
	int b = curve_data_ptr->glo_curve_data.title_font.font_color.blue();
	ui->title_fontColor_pushButton->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	emit sig_setTitle();
}
