#include "XRandomWidget.h"
#include "XRandom.h"


XRandomWidget::XRandomWidget(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	init();
}

XRandomWidget::~XRandomWidget()
{

}

QList<double> XRandomWidget::getRandomValues() const
{
	return m_randomValues;
}

void XRandomWidget::setMode(int mode)
{
	int multiNumber = 0;
	if (mode & 0x01)
	{	///<MG with single mode
		ui.radio_Fixed->show();
	}
	else
	{
		ui.radio_Fixed->hide();
	}

	if (mode & 0x02)
	{
		ui.radio_Normal->show();
	}
	else
		ui.radio_Normal->hide();

	if (mode & 0x04)
	{
		ui.radio_Linearity->show();
	}
	else
		ui.radio_Linearity->hide();

	if (mode & 0x08)
	{
		ui.radio_Gauss->show();
	}
	else
		ui.radio_Gauss->hide();

	if (mode & 0x10)
	{
		ui.radio_Possion->show();
	}
	else
		ui.radio_Possion->hide();

	if (mode & 0x20)
	{
		ui.radio_Exponential->show();
	}
	else
		ui.radio_Exponential->hide();

	ui.group_randomPara->hide();
	ui.groupBox_ParaSet->hide();

	if (mode & 0x01)
		ui.radio_Fixed->setChecked(true);
	else if (mode & 0x02)
		ui.radio_Normal->setChecked(true);
	else if (mode & 0x04)
		ui.radio_Linearity->setChecked(true);
	else if (mode & 0x08)
		ui.radio_Gauss->setChecked(true);
	else if (mode & 0x10)
		ui.radio_Possion->setChecked(true);
	else if (mode & 0x20)
		ui.radio_Exponential->setChecked(true);
}

void XRandomWidget::setRange(int min, int max)
{
	m_min = min;
	m_max = max;
}

void XRandomWidget::setInitSize(QSize nSize)
{
	m_initSize = nSize;
}

QSize XRandomWidget::sizeHint() const
{
	if (m_initSize == QSize())
	{
		return size();
	}
	else
		return m_initSize;
}

void XRandomWidget::slot_clicked_btn_random()
{
	ui.listWidget->clear();
	//m_randomValues.clear();
	///<MG 单值部分
	if (ui.radio_Fixed->isChecked())
	{
		double singleValue = 0.0;
		bool bConvertSingle = false;
		singleValue = ui.line_Value->text().toDouble(&bConvertSingle);
		if (!bConvertSingle){ return; }
		m_randomValues.push_back(singleValue);
	}
	else
	{
		///<MG 多值部分
		int iRandNum = ui.spin_paraNum->value();

		double fromValue = 0.0, toValue = 0.0;
		bool bConvertFrom = false, bConvertTo = false;
		fromValue = ui.line_paraFrom->text().toDouble(&bConvertFrom);
		toValue = ui.line_paraTo->text().toDouble(&bConvertTo);

		if (bConvertFrom && bConvertTo)
		{	///<MG 按照范围生成随机数
			double from = fromValue <= toValue ? fromValue : toValue;
			double to = fromValue <= toValue ? toValue : fromValue;

			if (from == to)
			{
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(from);
				}
			}
			else if (ui.radio_Normal->isChecked())
			{///<MG 完全随机
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Rand(from, to));
				}
			}
			else if (ui.radio_Linearity->isChecked())
			{///<MG 线性随机
				double range = to - from;
				{
					if (iRandNum == 1)
					{
						for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
						{
							double addValue = from + range / 2;
							m_randomValues.push_back(addValue);
						}

					}
					else
					{
						double add = range / (iRandNum - 1);
						for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
						{
							double addValue = from + iCnt * add;
							m_randomValues.push_back(addValue);
						}
					}
				}
			}
			else if (ui.radio_Gauss->isChecked())
			{///<MG 随机数符合高斯分布 
				double mesh = 0.0, stdc = 1.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					mesh = ui.line_Para1->text().toDouble();
				}
				if (!ui.line_Para2->text().isEmpty())
				{
					stdc = ui.line_Para2->text().toDouble();
				}
				int tryTime = 0;
				while (true)
				{
					double value = XRandom::instance()->GaussRand(mesh, stdc);
					if (value > from && value < to)
					{
						m_randomValues.push_back(value);
					}
					if (m_randomValues.size() >= iRandNum)
					{
						break;
					}
					tryTime++;
					if (tryTime > _MAX_TRY_TIME_)
					{
						break;
					}
				}
			}
			else if (ui.radio_Possion->isChecked())
			{///<MG 随机数符合泊松分布
				double lamdma = 20.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					lamdma = ui.line_Para1->text().toDouble();
				}
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Possion(lamdma));
				}
			}
			else if (ui.radio_Exponential->isChecked())
			{///<
				double lamdma = 20.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					lamdma = ui.line_Para1->text().toDouble();
				}
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Exponential(lamdma));
				}
			}
		}
		else
		{	///<MG 生成纯随机数
			if (ui.radio_Normal->isChecked())
			{
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Rand());
				}
			}
			else if (ui.radio_Linearity->isChecked())
			{
				double value1 = XRandom::instance()->Rand();
				double value2 = XRandom::instance()->Rand();
				double from = value1 > value2 ? value2 : value1;
				double to = value1 > value2 ? value1 : value2;
				double range = to - from;
				double add = range / (iRandNum - 1);
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					double addValue = from + iCnt * add;
					m_randomValues.push_back(addValue);
				}
			}
			else if (ui.radio_Gauss->isChecked())
			{
				double mesh = 0.0, stdc = 1.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					mesh = ui.line_Para1->text().toDouble();
				}
				if (!ui.line_Para2->text().isEmpty())
				{
					stdc = ui.line_Para2->text().toDouble();
				}
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->GaussRand(mesh, stdc));
				}
			}
			else if (ui.radio_Possion->isChecked())
			{
				double lamdma = 20.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					lamdma = ui.line_Para1->text().toDouble();
				}
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Possion(lamdma));
				}
			}
			else if (ui.radio_Exponential->isChecked())
			{
				double lamdma = 20.0;
				if (!ui.line_Para1->text().isEmpty())
				{
					lamdma = ui.line_Para1->text().toDouble();
					if (lamdma < 0)
					{
						lamdma = 20.0;
					}
				}
				for (int iCnt = 0; iCnt < iRandNum; ++iCnt)
				{
					m_randomValues.push_back(XRandom::instance()->Exponential(lamdma));
				}
			}
		}
	}
	for (double value : m_randomValues)
	{
		ui.listWidget->addItem(QString::number(value));
	}
}

void XRandomWidget::slot_clicked_radio_btn(bool bCheck)
{
	QRadioButton * radio = qobject_cast<QRadioButton*>(sender());
	if (!radio){ return; }

	if (radio == ui.radio_Fixed)
	{
		ui.group_randomPara->hide();
		ui.groupBox_ParaSet->show();
		ui.label_Value->show();
		ui.line_Value->show();

		ui.label_ParaFrom->hide();
		ui.line_paraFrom->hide();
		ui.label_ParaTo->hide();
		ui.line_paraTo->hide();
		ui.label_ParaNumber->hide();
		ui.spin_paraNum->hide();
	}
	else
	{
		ui.groupBox_ParaSet->show();
		ui.label_Value->hide();
		ui.line_Value->hide();
		ui.label_ParaFrom->show();
		ui.line_paraFrom->show();
		ui.label_ParaTo->show();
		ui.line_paraTo->show();
		ui.label_ParaNumber->show();
		ui.spin_paraNum->show();

		if (radio == ui.radio_Normal)
		{
			ui.group_randomPara->hide();
		}
		else if (radio == ui.radio_Linearity)
		{
			ui.group_randomPara->hide();
		}
		else if (radio == ui.radio_Gauss)
		{
			ui.group_randomPara->show();

			ui.label_Para1->show();
			ui.label_Para2->show();

			ui.line_Para1->show();
			ui.line_Para2->show();

			ui.label_Para1->setText(tr("Mean"));
			ui.label_Para2->setText(tr("Stdc"));
		}
		else if (radio == ui.radio_Possion)
		{
			ui.group_randomPara->show();
			ui.label_Para1->show();
			ui.label_Para2->hide();

			ui.line_Para1->show();
			ui.line_Para2->hide();

			///<MG 单位时间或单位面积内随机时间的平均发生率
			ui.label_Para1->setText(tr("Lambda"));
		}
		else if (radio == ui.radio_Exponential)
		{
			ui.group_randomPara->show();
			ui.label_Para1->show();
			ui.label_Para2->hide();

			ui.line_Para1->show();
			ui.line_Para2->hide();

			///<MG 单位时间或单位面积内随机时间的平均发生率
			ui.label_Para1->setText(tr("Lambda"));
			ui.group_randomPara->show();
		}
	}
}

void XRandomWidget::slot_check_value()
{
	QLineEdit * line = dynamic_cast<QLineEdit *>(sender());
	if (line == ui.line_paraFrom)
	{
		double fromValue = line->text().toDouble();
		if (fromValue < m_min)
			line->setStyleSheet("background-color:red");
		else
			line->setStyleSheet("background-color:white");
	}
	else if (line == ui.line_paraTo)
	{
		double toValue = line->text().toDouble();
		if (toValue > m_max)
			line->setStyleSheet("background-color:red");
		else
			line->setStyleSheet("background-color:white");
	}
}

void XRandomWidget::init()
{
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	connect(ui.btn_Random, SIGNAL(clicked()), SLOT(slot_clicked_btn_random()));
	connect(ui.radio_Normal, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));
	connect(ui.radio_Gauss, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));
	connect(ui.radio_Possion, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));
	connect(ui.radio_Exponential, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));
	connect(ui.radio_Linearity, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));
	connect(ui.radio_Fixed, SIGNAL(toggled(bool)), SLOT(slot_clicked_radio_btn(bool)));

	connect(ui.line_paraFrom, SIGNAL(textChanged(const QString &)), SLOT(slot_check_value()));
	connect(ui.line_paraTo, SIGNAL(textChanged(const QString &)), SLOT(slot_check_value()));

	ui.radio_Normal->setChecked(true);
	ui.group_randomPara->hide();
}

void XRandomWidget::accept()
{
	if (m_randomValues.isEmpty())
	{
		slot_clicked_btn_random();
	}
	QDialog::accept();
}
