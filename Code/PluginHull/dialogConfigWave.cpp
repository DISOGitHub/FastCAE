#include "dialogConfigWave.h"
#include "ui_dialogConfigWave.h"


namespace PluginShip
{
	ConfigWaveDialog::ConfigWaveDialog(GUI::MainWindow *m)
	{
		_ui = new Ui::ConfigWaveDialog;
		_ui->setupUi(this);

		InitCheckBox();

		init();

		_ui->lblwaveangle->setPixmap(QPixmap(":/qrc/waveangle2.png"));
		_ui->lblwaveangle->setAutoFillBackground(true);

		connect(_ui->AddBtn_RAO, SIGNAL(clicked()), this, SLOT(AddBtn_SingleRAO()));
		connect(_ui->ModifyBtn_RAO, SIGNAL(clicked()), this, SLOT(ModifyBtn_RAO()));
		connect(_ui->DeleteBtn_RAO, SIGNAL(clicked()), this, SLOT(DeleteBtn_RAO()));
		connect(_ui->DeleteAllBtn_RAO, SIGNAL(clicked()), this, SLOT(DeleteAllBtn_RAO()));

		connect(_ui->table_Freq, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTableItemClicked(QModelIndex)));
		connect(_ui->table_Amplitude, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTableItemClicked(QModelIndex)));
		connect(_ui->table_WaveDirectionAngle, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTableItemClicked(QModelIndex)));
		connect(_ui->AddGroup_RaoNew, SIGNAL(clicked()), this, SLOT(OnAddGroup_RaoNew()));

		RAOParRangeSet();

	}

	void ConfigWaveDialog::init()
	{
		InitTab();

		InitTable_Freq();
		InitTable_Amplitude();
		InitTable_WaveDirectionAngle();

		RAOParRangeSet();

		WaveParaRange();
		InitCheckBox();

	}

	void ConfigWaveDialog::InitTab()
	{
		_ui->tabWidget_RAO_Para->tabBar()->hide();
	}

	void ConfigWaveDialog::InitTableWidget(QTableWidget *tablewidget)
	{
		tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);    //����ѡ��һ��
		tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);   //����ֻ��ѡ��һ�У����ܶ���ѡ��
		tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);     //����ÿ�����ݲ��ɸ���
		tablewidget->setAlternatingRowColors(true);                          //���ø�һ�б�һ��ɫ������һ��һ��
		//tablewidget->seht(true)
		tablewidget->horizontalHeader()->setVisible(true);

	}

	void ConfigWaveDialog::InitTable_Freq()
	{

		InitTableWidget(_ui->table_Freq);
		_ui->table_Freq->setColumnCount(3);
		_ui->table_Freq->setColumnWidth(0, 100);
		_ui->table_Freq->setColumnWidth(1, 100);
		_ui->table_Freq->setColumnWidth(2, 150);

		_ui->table_Freq->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
		_ui->table_Freq->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
		_ui->table_Freq->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

		QHeaderView *headerGoods = _ui->table_Freq->horizontalHeader();
		//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
		headerGoods->setSortIndicator(0, Qt::AscendingOrder);
		headerGoods->setSortIndicatorShown(true);
		connect(headerGoods, SIGNAL(sectionClicked(int)), _ui->table_Freq, SLOT(sortByColumn(int)));
	}

	void ConfigWaveDialog::InitTable_Amplitude()
	{
		InitTableWidget(_ui->table_Amplitude);
		_ui->table_Amplitude->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

		QHeaderView *headerGoods = _ui->table_Amplitude->horizontalHeader();
		//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
		headerGoods->setSortIndicator(0, Qt::AscendingOrder);
		headerGoods->setSortIndicatorShown(true);
		connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT(waveheading_sortbyclounm(int)));
	}

	void ConfigWaveDialog::InitTable_WaveDirectionAngle()
	{
		InitTableWidget(_ui->table_WaveDirectionAngle);
		_ui->table_WaveDirectionAngle->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


		QHeaderView *headerGoods = _ui->table_WaveDirectionAngle->horizontalHeader();
		//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
		headerGoods->setSortIndicator(0, Qt::AscendingOrder);
		headerGoods->setSortIndicatorShown(true);
		connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT(waveheading_sortbyclounm(int)));
	}

	void ConfigWaveDialog::WaveParaRange()
	{
		if (freqGroup->checkedId() == 0)
		{
			m_FirstValue = 0;
			m_LastValue = 10000;
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
		}
		else if (freqGroup->checkedId() == 1)
		{
			m_FirstValue = 0;
			m_LastValue = 3000;
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
		}
		else if (freqGroup->checkedId() == 2)
		{
			m_FirstValue = 0;
			m_LastValue = 20;
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
		}
		else if (freqGroup->checkedId() == 3)
		{
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Amplitude);
		}
		else if (freqGroup->checkedId() == 4)
		{
			m_FirstValue = 0;
			m_LastValue = 360;
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_WaveDirectionAngle);
		}


	}

	void ConfigWaveDialog::SetAmplitude()
	{

		_ui->table_Amplitude->clearContents();
		_ui->table_Amplitude->setRowCount(0);

		//	QList<double> values = widget.getRandomValues();
		for (int i = 0; i < groupNum; i++)
		{
			_ui->table_Amplitude->insertRow(i);
			_ui->table_Amplitude->setItem(i, 0, CreateItem(QString::number(m_Min_Value + m_Step*i, 10, m_DecimalNum)));
		}
	}

	void ConfigWaveDialog::OnAddGroup_RaoNew()
	{
		// �ж������Ƿ��Ƿ�

		if (!(_ui->lineEdit_Min_Value->check() && _ui->lineEdit_MaxValue->check() && _ui->lineEdit_Step->check()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs!"));
			return;
		}

		// �ж������Ĳ����Ƿ�Ϊ��
		m_MaxValue = _ui->lineEdit_MaxValue->getDoubleData();
		m_Step = _ui->lineEdit_Step->getDoubleData();
		m_Min_Value = _ui->lineEdit_Min_Value->getDoubleData();

		if (m_MaxValue < INVALID_DOUBLE || m_Step < INVALID_DOUBLE || m_Min_Value < INVALID_DOUBLE)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The parameter value is empty, please re-enter!"));
			return;
		}

		if (m_MaxValue == 0 || m_Step == 0 || m_Min_Value == 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The parameter value is zero,please re-enter!"));
			return;
		}

		// ��Сֵ��������ֵ���ֱ���
		if (_ui->lineEdit_Min_Value->getDoubleData() > _ui->lineEdit_MaxValue->getDoubleData())
		{
			QMessageBox::warning(this, tr("Warning"), tr("The minimum should be smaller than the maximum value!"));
			return;
		}

		//����С������ֵ
		if (_ui->lineEdit_Step->getDoubleData() > _ui->lineEdit_MaxValue->getDoubleData())
		{
			QMessageBox::warning(this, tr("Warning"), tr("The step size should not be greater than the maximum."));
			return;
		}


		double gen_val1, gen_val2, gen_val3;
		m_MaxValue = _ui->lineEdit_MaxValue->getDoubleData();
		m_Min_Value = _ui->lineEdit_Min_Value->getDoubleData();
		m_Step = _ui->lineEdit_Step->getDoubleData();

		int num = (m_MaxValue - m_Min_Value) / m_Step + 1;
		//singleAddWidget.exec();
		groupNum = num;
		switch (freqGroup->checkedId())
			//switch (_ui->WaveParaOptionsCbo->currentIndex())
		{
		case Enum_Wavelength:
		{
			SetWaveLength();
		}
		break;
		case Enum_Periodic:
		{
			Setperiod();
		}
		break;
		case Enum_Freq:
		{
			SetFreq();
		}
		break;
		case Enum_Amplitude:
		{
			SetAmplitude();
		}
		break;
		case Enum_WaveDirectionAngle:
		{
			SetWaveDirectionAngel();
		}
		break;
		}
	}

	void ConfigWaveDialog::SetWaveDirectionAngel()
	{

		int rowCount = _ui->table_WaveDirectionAngle->rowCount();
		int totalCount;
		totalCount = rowCount + widget.getRandomValues().count();
		_ui->table_WaveDirectionAngle->clearContents();
		_ui->table_WaveDirectionAngle->setRowCount(0);

		//QList<double> values = widget.getRandomValues();
		for (int i = 0; i <groupNum; i++)
		{
			_ui->table_WaveDirectionAngle->insertRow(i);
			_ui->table_WaveDirectionAngle->setItem(i, 0, CreateItem(QString::number(m_Min_Value + m_Step*i, 10, m_DecimalNum)));
		}
	}

	void ConfigWaveDialog::SetFreq()
	{

		_ui->table_Freq->clearContents();
		_ui->table_Freq->setRowCount(0);

		for (int i = 0; i < groupNum; i++)
		{
			double dFreq = m_Min_Value + m_Step * i;
			double dPeriodic = 2 * PI / dFreq;
			double dWaveLength = GRAVITY*dPeriodic*dPeriodic / (2.0*PI);
			//double dWaveLength;

			_ui->table_Freq->insertRow(i);
			_ui->table_Freq->setItem(i, 2, CreateItem(QString::number(dFreq, 10, m_DecimalNum)));
			_ui->table_Freq->setItem(i, 1, CreateItem(QString::number(dPeriodic, 10, m_DecimalNum)));
			_ui->table_Freq->setItem(i, 0, CreateItem(QString::number(dWaveLength, 10, m_DecimalNum)));
		}

	}

	void ConfigWaveDialog::SetWaveLength()
	{
		//	if (QDialog::Accepted == widget.exec(tr("Wave Length Setting"), m_FirstValue, m_LastValue))
		{
			_ui->table_Freq->clearContents();
			_ui->table_Freq->setRowCount(0);
			for (int i = 0; i < groupNum; i++)
			{

				double dWaveLength = m_Min_Value + m_Step*i;
				double dPeriodic = sqrt(2.0 * PI * dWaveLength / 9.81);
				double dFreq = 2.0 * PI / dPeriodic;

				_ui->table_Freq->insertRow(i);
				_ui->table_Freq->setItem(i, 0, CreateItem(QString::number(dWaveLength, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(i, 1, CreateItem(QString::number(dPeriodic, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(i, 2, CreateItem(QString::number(dFreq, 10, m_DecimalNum)));


			}
		}
	}

	void ConfigWaveDialog::Setperiod()
	{

		_ui->table_Freq->clearContents();
		_ui->table_Freq->setRowCount(0);
		for (int i = 0; i < groupNum; i++)
		{
			double dPeriodic = m_Min_Value + m_Step*i;
			double dFreq = 2.0*PI / dPeriodic;
			double dWaveLength = 9.81*dPeriodic*dPeriodic / (PI*2.0);

			_ui->table_Freq->insertRow(i);
			_ui->table_Freq->setItem(i, 2, CreateItem(QString::number(dFreq, 10, m_DecimalNum)));
			_ui->table_Freq->setItem(i, 1, CreateItem(QString::number(dPeriodic, 10, m_DecimalNum)));
			_ui->table_Freq->setItem(i, 0, CreateItem(QString::number(dWaveLength, 10, m_DecimalNum)));
		}
	}

	void ConfigWaveDialog::RAOParRangeSet()
	{
		_ui->lineEdit_InputValue->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_InputValue->setAccuracy(m_DecimalNum);
		RadioRangeSet();
		_ui->lineEdit_InputValue->enableRangeCheck(true);
		//appendCheckObj(_ui->lineEdit_InputValue);
		_ui->lineEdit_Min_Value->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_Min_Value->setAccuracy(m_DecimalNum);
		_ui->lineEdit_Min_Value->setRange(0, 1025);
		_ui->lineEdit_Min_Value->enableRangeCheck(true);
		//appendCheckObj(_ui->lineEdit_Min_Value);

		_ui->lineEdit_MaxValue->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_MaxValue->setAccuracy(m_DecimalNum);
		_ui->lineEdit_MaxValue->setRange(0, 1025);
		_ui->lineEdit_MaxValue->enableRangeCheck(true);
		//appendCheckObj(_ui->lineEdit_MaxValue);

		_ui->lineEdit_Step->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_Step->setAccuracy(m_DecimalNum);
		_ui->lineEdit_Step->setRange(0, 1025);
		_ui->lineEdit_Step->enableRangeCheck(true);
		//appendCheckObj(_ui->lineEdit_MaxValue);

	}

	void ConfigWaveDialog::InitCheckBox()
	{
		freqGroup = new QButtonGroup(this);
		freqGroup->addButton(_ui->wavelengthRadio, 0);
		freqGroup->addButton(_ui->periodRadio, 1);
		freqGroup->addButton(_ui->freqRadio, 2);
		freqGroup->addButton(_ui->AmplitudeRadio, 3);
		freqGroup->addButton(_ui->directionRadio, 4);
		_ui->freqRadio->setChecked(true);
		//_ui->MotionCheckBox1->setChecked(true);	
	}

	void ConfigWaveDialog::RadioRangeSet()
	{
		switch (freqGroup->checkedId())
			//switch (_ui->WaveParaOptionsCbo->currentIndex())
		{
		case Enum_Wavelength:
			_ui->lineEdit_InputValue->setRange(0, 10000);
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
			break;
		case Enum_Periodic:
			_ui->lineEdit_InputValue->setRange(0, 3000);
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
			break;
		case Enum_Freq:
			_ui->lineEdit_InputValue->setRange(0, 20);
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Wavelen_Periodic_Freq);
			break;
		case Enum_Amplitude:
			_ui->lineEdit_InputValue->setRange(0, 20);
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_Amplitude);
			break;
		case Enum_WaveDirectionAngle:
			_ui->lineEdit_InputValue->setRange(0, 360);
			_ui->tabWidget_RAO_Para->setCurrentIndex(Tab_WaveDirectionAngle);
			break;
		}
	}

	void ConfigWaveDialog::AddBtn_SingleRAO()
	{
		SingleRAO_Set(Enum_Add);
	}


	void ConfigWaveDialog::ModifyBtn_RAO()
	{
		SingleRAO_Set(Enum_Modify);
	}

	void ConfigWaveDialog::SingleRAO_Set(int Operation_Type)
	{
		if (!CheckIllegalInPut(_ui->lineEdit_InputValue))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs!"));
			return;
		}

		if (!CheckParaIsempty(_ui->lineEdit_InputValue))
		{
			QMessageBox::warning(this, tr("Warning"), tr("The parameter value is empty, please re-enter!"));
			return;
		}
		if (!CheckValueZero(_ui->lineEdit_InputValue))
		{
			QMessageBox::warning(this, tr("Warning"), tr("The parameter value is zero,please re-enter!"));
			return;
		}

		if (!CheckRepeat())
		{
			return;
		}

		int num_row = 0;
		double gen_val1 = 0;
		double gen_val2 = 0;
		double gen_val3 = 0;
		int checkId;
		checkId = freqGroup->checkedId();
		if (checkId == Enum_Wavelength || checkId == Enum_Periodic || checkId == Enum_Freq)
		{
			//����
			if (checkId == Enum_Wavelength)
			{
				gen_val1 = m_SingleValue;
				gen_val2 = sqrt(2.0*PI*gen_val1 / 9.81);
				gen_val3 = 2.0*PI / gen_val2;

				if (Enum_Modify == Operation_Type)
				{
					num_row = _ui->table_Freq->currentRow();
					if (num_row < 0)
					{
						return;
					}
				}
				else if (Enum_Add == Operation_Type)
				{
					num_row = _ui->table_Freq->rowCount();
					_ui->table_Freq->insertRow(num_row);
				}

				_ui->table_Freq->setItem(num_row, 0, CreateItem(QString::number(gen_val1, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 1, CreateItem(QString::number(gen_val2, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 2, CreateItem(QString::number(gen_val3, 10, m_DecimalNum)));
			}
			else if (checkId == Enum_Periodic)
			{

				gen_val2 = m_SingleValue;
				gen_val3 = 2.0*PI / gen_val2;
				gen_val1 = 9.81*gen_val2*gen_val2 / (PI*2.0);

				if (Enum_Modify == Operation_Type)
				{
					num_row = _ui->table_Freq->currentRow();
					if (num_row < 0)
					{
						return;
					}
				}
				else if (Enum_Add == Operation_Type)
				{
					num_row = _ui->table_Freq->rowCount();
					_ui->table_Freq->insertRow(num_row);
				}
				_ui->table_Freq->setItem(num_row, 0, CreateItem(QString::number(gen_val1, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 1, CreateItem(QString::number(gen_val2, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 2, CreateItem(QString::number(gen_val3, 10, m_DecimalNum)));
			}
			else if (checkId == Enum_Freq)//Ƶ��
			{
				gen_val3 = m_SingleValue;
				gen_val2 = 2 * PI / gen_val3;
				gen_val1 = 9.81*gen_val2*gen_val2 / (2.0*PI);

				if (Enum_Modify == Operation_Type)
				{
					num_row = _ui->table_Freq->currentRow();
					if (num_row < 0)
					{
						return;
					}
				}
				else if (Enum_Add == Operation_Type)
				{
					num_row = _ui->table_Freq->rowCount();
					_ui->table_Freq->insertRow(num_row);
				}

				_ui->table_Freq->setItem(num_row, 0, CreateItem(QString::number(gen_val1, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 1, CreateItem(QString::number(gen_val2, 10, m_DecimalNum)));
				_ui->table_Freq->setItem(num_row, 2, CreateItem(QString::number(gen_val3, 10, m_DecimalNum)));
			}
		}
		else if (checkId == Enum_Amplitude)//��ֵ
		{
			if (Enum_Modify == Operation_Type)
			{
				num_row = _ui->table_Amplitude->currentRow();
				if (num_row < 0)
				{
					return;
				}
			}
			else if (Enum_Add == Operation_Type)
			{
				num_row = _ui->table_Amplitude->rowCount();
				_ui->table_Amplitude->insertRow(num_row);
			}

			_ui->table_Amplitude->setItem(num_row, 0, CreateItem(QString::number(m_SingleValue, 10, m_DecimalNum)));

		}
		else if (checkId == Enum_WaveDirectionAngle)//������
		{

			if (Enum_Modify == Operation_Type)
			{
				num_row = _ui->table_WaveDirectionAngle->currentRow();
				if (num_row < 0)
				{
					return;
				}
			}
			else if (Enum_Add == Operation_Type)
			{
				num_row = _ui->table_WaveDirectionAngle->rowCount();
				_ui->table_WaveDirectionAngle->insertRow(num_row);
			}

			_ui->table_WaveDirectionAngle->setItem(num_row, 0, CreateItem(QString::number(m_SingleValue, 10, m_DecimalNum)));
		}
	}

	bool ConfigWaveDialog::CheckRepeat()
	{
		m_SingleValue = _ui->lineEdit_InputValue->getDoubleData();
		QString SingleValue_Temp = QString::number(m_SingleValue, 10, m_DecimalNum);
		int checkid = freqGroup->checkedId();
		switch (checkid)
			//switch (_ui->WaveParaOptionsCbo->currentIndex())
		{
		case Enum_Wavelength:  //����
		{
			if (!SetRAOPara(_ui->table_Freq, 0, SingleValue_Temp))
			{
				return false;
			}
		}
		break;
		case Enum_Periodic:   //����
		{
			if (!SetRAOPara(_ui->table_Freq, 1, SingleValue_Temp))
			{
				return false;
			}
		}
		break;
		case Enum_Freq: //Ƶ��
		{
			if (!SetRAOPara(_ui->table_Freq, 2, SingleValue_Temp))
			{
				return false;
			}
		}
		break;
		case Enum_Amplitude://��ֵ
		{
			if (!SetRAOPara(_ui->table_Amplitude, 0, SingleValue_Temp))
			{
				return false;
			}
		}
		break;
		case Enum_WaveDirectionAngle://������
		{
			if (!SetRAOPara(_ui->table_WaveDirectionAngle, 0, SingleValue_Temp))
			{
				return false;
			}
		}
		break;
		default:
			break;
		}

		return true;

	}

	bool ConfigWaveDialog::SetRAOPara(QTableWidget *_TableWidget, int column, QString SingleValue_Temp)
	{
		int rownum = _TableWidget->rowCount();
		for (int i = 0; i < rownum; i++)
		{
			QString temp = _TableWidget->item(i, column)->text();
			if (temp.indexOf(SingleValue_Temp) == 0)
			{
				QMessageBox::warning(this, tr("Warning"), tr("The parameter already exists, please re-enter!"));
				return false;
			}
		}

		return true;
	}


	void ConfigWaveDialog::DeleteBtn_RAO()
	{
		int cur_row = 0;
		int checkid = freqGroup->checkedId();
		if (checkid == Enum_Wavelength || checkid == Enum_Periodic || checkid == Enum_Freq)
		{
			cur_row = _ui->table_Freq->currentRow();
			if (cur_row < 0)
				return;
			_ui->table_Freq->removeRow(cur_row);
		}
		else if (checkid == Enum_Amplitude)//��ֵ
		{
			cur_row = _ui->table_Amplitude->currentRow();
			if (cur_row < 0)
				return;
			_ui->table_Amplitude->removeRow(cur_row);

		}
		else if (checkid == Enum_WaveDirectionAngle)
		{
			cur_row = _ui->table_WaveDirectionAngle->currentRow();
			if (cur_row < 0)
				return;
			_ui->table_WaveDirectionAngle->removeRow(cur_row);
		}

	}

	bool ConfigWaveDialog::CheckIllegalInPut(SelfDefLineEdit *Edit)
	{
		bool pass = Edit->check();
		if (!pass)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool ConfigWaveDialog::CheckParaIsempty(SelfDefLineEdit *Edit)
	{
		if (Edit->getDoubleData() < INVALID_DOUBLE)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool ConfigWaveDialog::CheckValueZero(SelfDefLineEdit* Edit)
	{
		int checkId;
		checkId = freqGroup->checkedId();
		if (checkId == Enum_Wavelength || checkId == Enum_Periodic || checkId == Enum_Freq)
		{
			if (Edit->getDoubleData() == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	QTableWidgetItem* ConfigWaveDialog::CreateItem(QString str)
	{
		QTableWidgetItem* item = new QTableWidgetItem(str);
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		return item;
	}

	void ConfigWaveDialog::DeleteAllBtn_RAO()
	{
		int checkid;
		checkid = freqGroup->checkedId();
		if (checkid == Enum_Wavelength || checkid == Enum_Periodic || checkid == Enum_Freq)
		{
			_ui->table_Freq->clearContents();
			_ui->table_Freq->setRowCount(0);
		}
		else if (checkid == Enum_Amplitude)
		{
			_ui->table_Amplitude->clearContents();
			_ui->table_Amplitude->setRowCount(0);
		}
		else if (checkid == Enum_WaveDirectionAngle) 
		{
			_ui->table_WaveDirectionAngle->clearContents();
			_ui->table_WaveDirectionAngle->setRowCount(0);
		}
	}

	void ConfigWaveDialog::OnTableItemClicked(QModelIndex modeIndex)
	{

		QTableWidget *tablewidget_Flag = (QTableWidget *)sender();

		if (tablewidget_Flag == _ui->table_Amplitude)
		{
			int cur_row = _ui->table_Amplitude->currentRow();
			if (cur_row < 0)
				return;
			_ui->lineEdit_InputValue->setText(_ui->table_Amplitude->item(cur_row, 0)->text());
		}
		else if (tablewidget_Flag == _ui->table_WaveDirectionAngle)
		{
			int cur_row = _ui->table_WaveDirectionAngle->currentRow();
			if (cur_row < 0)
				return;
			_ui->lineEdit_InputValue->setText(_ui->table_WaveDirectionAngle->item(cur_row, 0)->text());
		}
		else if (tablewidget_Flag == _ui->table_Freq)
		{
			int cur_row = _ui->table_Freq->currentRow();
			if (cur_row < 0)
				return;
			if (freqGroup->checkedId()>3)
			{
				return;
			}
			//	_ui->lineEdit_InputValue->setText(_ui->table_Freq->item(cur_row, _ui->WaveParaOptionsCbo->currentIndex())->text());
			int checkid = freqGroup->checkedId();
			QTableWidgetItem* a = _ui->table_Freq->item(cur_row, freqGroup->checkedId());
			_ui->lineEdit_InputValue->setText(_ui->table_Freq->item(cur_row, freqGroup->checkedId())->text());
		}

	}


	ConfigWaveDialog::~ConfigWaveDialog()
	{
		
	}
}