#include "confirmation.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include "hardware.h"
#include "singletion.h"
#include "generateLicense.h"
#include "settings/busAPI.h"
#include "ui_confirmation.h"

Confirmation::Confirmation(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Confirmation)
{
	ui->setupUi(this);
	initilize();
}

Confirmation::~Confirmation()
{
	delete ui;
}

bool Confirmation::licenseCheck()
{
	return true;
	QString licensePath = Setting::BusAPI::instance()->getLicensePath();
	if (licensePath.isEmpty())
	{
		if (QDialog::Accepted == exec())
		{
			return true;
		}
		return false;
	}
	QFile file(licensePath);
	if (file.open(QIODevice::ReadOnly))
	{
		QString information = file.readAll();
		file.close();
		if (check(information))
		{
			return true;
		}
		return false;
	}
	return false;

//	return true;
}

void Confirmation::initilize()
{
	connect(ui->btn_buy, SIGNAL(clicked()), SLOT(slot_clicked_buy_button()));
	connect(ui->btn_confirm, SIGNAL(clicked()), SLOT(slot_clicked_confirm_button()));
	connect(ui->btn_path, SIGNAL(clicked()), SLOT(slot_clicked_path_button()));
	connect(ui->radio_file, SIGNAL(clicked()), SLOT(slot_clicked_radio_button()));
	connect(ui->radio_text, SIGNAL(clicked()), SLOT(slot_clicked_radio_button()));

	ui->radio_text->setChecked(true);
	ui->text_license->setEnabled(true);
	ui->btn_path->setEnabled(false);
}

bool Confirmation::check(QString license)
{
	if (license.size() < 1024)
	{
		return false;
	}

	if (license.size() > 1024)
	{
		QString external = license.right(license.size() - 1024);
		for (QChar charter : external)
		{
			int iValue = charter.toLatin1();
			if (iValue > 32 && iValue < 126)
			{
				return false;
			}
		}
	}

	license = license.left(1024);

	Hardware * hardware = XSingletion<Hardware>::getInstance();
	QString cpuId = hardware->getCentralProcessingUnitId();
	QStringList diskSerialId = hardware->getDiskSerialId();
	QString macAddress = hardware->getMacAddress();

	//QString cpuId = "BFEBFBFF000506E3";
	//QStringList diskSerialId = QStringList{"WD-WCC6Y1KD2KNT"};
	//QString macAddress = "64-00-6A-3C-00-8F";


	QString diskSerialIdPart = license.left(204);
	QString cpuIdPart = license.mid(204, 204);
	QString macAddressPart = license.mid(408, 204);
	QString begTimePart = license.mid(612, 206);
	QString endTimePart = license.right(206);

	generateLicense * _generate = XSingletion<generateLicense>::getInstance();
	/*disk serial ids information total length 204 */
	QStringList decodeDiskSerialId = _generate->decode(diskSerialIdPart, '?', 2, 0, 1).split('|', QString::SkipEmptyParts);
	if (decodeDiskSerialId != diskSerialId)
	{
		return false;
	}

	/*central processor unit information total length 204 */
	QString decodeCpuId = _generate->decode(cpuIdPart, '!', 3, 0, 0);
	if (decodeCpuId != cpuId)
	{
		return false;
	}

	/*mac address information total length 204 */
	QString decodeMacAddress = _generate->decode(macAddressPart, '<', 4, 0, 1);
	if (decodeMacAddress != macAddress)
	{
		return false;
	}
	
	/*begin time of the license total length 206*/
	QString decodeBegTimeStep1 = _generate->decode(begTimePart, ';', 3, 1, 0);
	QDate from = _generate->decode(decodeBegTimeStep1, 9, 6);

	/*end time of the license total length 206*/
	QString decodeEndTimeStep1 = _generate->decode(endTimePart, '}', 3, 1, 1);
	QDate to = _generate->decode(decodeEndTimeStep1, 3, 7);

	if (from > to)
	{
		return false;
	}
	
	if (from == to)
	{
		///<MG forever 
		return true;
	}
	if (to < QDate::currentDate())
	{
		return false;
	}
}

void Confirmation::slot_clicked_buy_button()
{
	QDesktopServices::openUrl(QUrl(QLatin1String("http://www.qianfankeji.cn/about.php?cid=3")));
}

void Confirmation::slot_clicked_confirm_button()
{
	QString license;
	if (ui->radio_file->isChecked())
	{
		QFile file(ui->line_path->text());
		if (!file.exists())
		{
			QMessageBox::warning(this, "Warning", "This is not a validate file path!");
			return;
		}
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(this, "Warning", "Could not open this file !");
			return;
		}
		license = file.readAll();
		file.close();
	}
	else if (ui->radio_text->isChecked())
	{
		license = ui->text_license->toPlainText();
	}
	if (check(license))
	{
		///<MG user input the license text. we store it as a invalidate license file and tell the setting module
		if (ui->radio_text->isChecked())
		{
			QString filePath = qApp->applicationDirPath() + "/qianfan.lincese";
			QFile file(filePath);
			if (file.open(QIODevice::WriteOnly))
			{
				file.write(license.toStdString().c_str());
				file.close();
			}
			Setting::BusAPI::instance()->setLicensePath(filePath);
			Setting::BusAPI::instance()->writeINI();
		}
		///<MG user use the license file. save the file path to the setting module
		else if (ui->radio_file->isChecked())
		{
			Setting::BusAPI::instance()->setLicensePath(ui->line_path->text());
			Setting::BusAPI::instance()->writeINI();
		}
		QDialog::accept();
	}
}

void Confirmation::slot_clicked_path_button()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select License File"), QString(), "*.license");
	ui->line_path->setText(filePath);
}

void Confirmation::slot_clicked_radio_button()
{
	ui->btn_path->setEnabled(ui->radio_file->isChecked());
	ui->text_license->setEnabled(ui->radio_text->isChecked());
}
