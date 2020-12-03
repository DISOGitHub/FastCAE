#include "dialogConfigWaveSpectrum.h"
#include "ui_dialogConfigWaveSpectrum.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeoCommandMakeHullCut.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QTreeWidget>
#include "mainWindow/mainWindow.h"
#include <QDebug>
#include "DataProperty/modelTreeItemType.h"
//#include "PostWidgets/Post2DWidget.h"
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include "PostWidgets/Post2DWidget.h"
using namespace std;

#define INVALID_DOUBLE -1e50  //无效的浮点数


namespace PluginShip
{
	ConfigWaveSpectrumDialog::ConfigWaveSpectrumDialog(GUI::MainWindow *m) :
		_mainwindow(m)
	{
		_ui = new Ui::ConfigWaveSpectrumDialog;
		_ui->setupUi(this);

		this->setWindowTitle(QString("Wave Spectrum"));

		m_Nrwvcomp = 500;//由外部获取，暂时使用默认值

		connect(_ui->AddBtn_Spectral, SIGNAL(clicked()), this, SLOT(OnAddBtn_Spectral()));
		connect(_ui->ModifyBtn_Spectral, SIGNAL(clicked()), this, SLOT(OnModifyBtn_Spectral()));
		connect(_ui->DeleteBtn_Spectral, SIGNAL(clicked()), this, SLOT(OnDeleteBtn_Spectral()));

		connect(_ui->WaveSpectrumTypeCbo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_WaveSpectrumTypeCbo_currentChanged(int)));

		connect(_ui->table_SpectralAnalysisPara, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTableFreqItemClicked(QModelIndex)));

		initPost2d();
		initLineEdit();
		on_WaveSpectrumTypeCbo_currentChanged(0);
	}

	void ConfigWaveSpectrumDialog::initPost2d()
	{
		PreProcessing_Curve = new Post::Post2DWindow(_mainwindow, 0);//参数2为project_ID,作为显示，默认设为0，有问题再进行更改
		_ui->horizontalLayoutCurve->addWidget(PreProcessing_Curve);

		double Data_Max_Min[2] = { 0 };
		Data_Max_Min[1] = 20;
		Data_Max_Min[0] = 0;
		PreProcessing_Curve->setAxisRange("y", Data_Max_Min);
		PreProcessing_Curve->setAxisRange("x", Data_Max_Min);

		PreProcessing_Curve->setTitle(tr("Wave Spectrum Type"));
		PreProcessing_Curve->setAxisName("x", QStringLiteral("ω(rad/s)"));
		PreProcessing_Curve->setAxisName("y", QStringLiteral("s(m².s)"));
		PreProcessing_Curve->setTitleFontSize(15);
		PreProcessing_Curve->setAxisFontSize("x", 12);
		PreProcessing_Curve->setAxisFontSize("y", 12);
		PreProcessing_Curve->replot();
	}

	ConfigWaveSpectrumDialog::~ConfigWaveSpectrumDialog()
	{
	}

	void ConfigWaveSpectrumDialog::initLineEdit()
	{
		_ui->Edit_SignificantWaveHeight->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_LowCutoffFreq->setDataType(DATATYPEDOUBLE);
		_ui->lineEdit_HighCutoffFreq->setDataType(DATATYPEDOUBLE);
		_ui->Edit_SpectrumHeartCycle->setDataType(DATATYPEDOUBLE);
		_ui->Edit_SpectralWindFactor->setDataType(DATATYPEDOUBLE);
		_ui->randomSecondLineEdit->setDataType(DATATYPEDOUBLE);
		_ui->Edit_ShowAngleCbo->setDataType(DATATYPEDOUBLE);

	}

	void ConfigWaveSpectrumDialog::Spectral_Set(int Operation_Type)
	{

		if (!(Check_Inputs()))
		{
			return;
		}

		int num_row = 0;
		if (Enum_Modify == Operation_Type)
		{
			num_row = _ui->table_SpectralAnalysisPara->currentRow();
			if (num_row < 0)
			{
				return;
			}
		}
		else if (Enum_Add == Operation_Type)
		{
			num_row = _ui->table_SpectralAnalysisPara->rowCount();
			_ui->table_SpectralAnalysisPara->insertRow(num_row);
		}

		// 海浪谱类型
		switch (_ui->WaveSpectrumTypeCbo->currentIndex())
		{
		case Enum_ITTC_Single:

			_ui->table_SpectralAnalysisPara->setItem(num_row, 0, CreateItem(tr("ITTCSingleParaSpectrum")));

			_ui->table_SpectralAnalysisPara->item(num_row, 0)->setData(Qt::UserRole, "single");
			//海浪谱心周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 6, CreateItem("---"));

			//谱峰周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 7, CreateItem("---"));

			//谱峰因子
			_ui->table_SpectralAnalysisPara->setItem(num_row, 8, CreateItem("---"));

			break;
		case Enum_ISSC_Double:

			_ui->table_SpectralAnalysisPara->setItem(num_row, 0, CreateItem(tr("ITTCDoubleParaSpectrum")));
			_ui->table_SpectralAnalysisPara->item(num_row, 0)->setData(Qt::UserRole + 1, "double");
			//海浪谱心周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 6, CreateItem(QString::number(_ui->Edit_SpectrumHeartCycle->getDoubleData(), 10, m_DecimalNum)));

			//谱峰周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 7, CreateItem("---"));

			//谱峰因子
			_ui->table_SpectralAnalysisPara->setItem(num_row, 8, CreateItem("---"));

			break;
		case Enum_JONSWAP:
			_ui->table_SpectralAnalysisPara->setItem(num_row, 0, CreateItem(tr("JONSWAPWaveSpectrum")));
			_ui->table_SpectralAnalysisPara->item(num_row, 0)->setData(Qt::UserRole + 2, "JONSWAP");
			//海浪谱心周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 6, CreateItem("---"));

			//谱峰周期
			_ui->table_SpectralAnalysisPara->setItem(num_row, 7, CreateItem(QString::number(_ui->Edit_SpectrumHeartCycle->getDoubleData(), 10, m_DecimalNum)));

			//谱峰因子
			_ui->table_SpectralAnalysisPara->setItem(num_row, 8, CreateItem(QString::number(_ui->Edit_SpectralWindFactor->getDoubleData(), 10, m_DecimalNum)));
			break;
		}

		// 有义高波
		_ui->table_SpectralAnalysisPara->setItem(num_row, 1, CreateItem(QString::number(_ui->Edit_SignificantWaveHeight->getDoubleData(), 10, m_DecimalNum)));

		// 浪向角
		_ui->table_SpectralAnalysisPara->setItem(num_row, 2, CreateItem(QString::number(_ui->Edit_ShowAngleCbo->getDoubleData(), 10, m_DecimalNum)));

		// 低频截止频率	
		_ui->table_SpectralAnalysisPara->setItem(num_row, 3, CreateItem(QString::number(_ui->lineEdit_LowCutoffFreq->getDoubleData(), 10, m_DecimalNum)));

		// 高频截止频率
		_ui->table_SpectralAnalysisPara->setItem(num_row, 4, CreateItem(QString::number(_ui->lineEdit_HighCutoffFreq->getDoubleData(), 10, m_DecimalNum)));

		//离散的海浪谱密度
		_ui->table_SpectralAnalysisPara->setItem(num_row, 5, CreateItem(QString::number(_ui->randomSecondLineEdit->getDoubleData(), 10, m_DecimalNum)));

	}

	bool ConfigWaveSpectrumDialog::Check_Inputs()
	{
		// 判断输入的参数是否为空
		m_SignificantWaveHeight = _ui->Edit_SignificantWaveHeight->getDoubleData();	//有义波高
		m_LowCutoffFreq = _ui->lineEdit_LowCutoffFreq->getDoubleData();				//低频截至频率
		m_HighCutoffFreq = _ui->lineEdit_HighCutoffFreq->getDoubleData();			//高频截至频率
		m_SpectrumHeartCycle = _ui->Edit_SpectrumHeartCycle->getDoubleData();		//海浪谱心周期
		m_SpectralWindFactor = _ui->Edit_SpectralWindFactor->getDoubleData();		//谱峰因子
		m_DiscreteWavesNum = _ui->randomSecondLineEdit->getDoubleData();			//离散波数
		m_Angel = _ui->Edit_ShowAngleCbo->getDoubleData();							//浪向角


		// 海浪谱类型
		switch (_ui->WaveSpectrumTypeCbo->currentIndex())
		{
		case Enum_ITTC_Single:

			// 判断输入是否非法
			if (!(_ui->Edit_ShowAngleCbo->check() && _ui->Edit_SignificantWaveHeight->check()
				&& _ui->lineEdit_LowCutoffFreq->check() && _ui->lineEdit_HighCutoffFreq->check()
				&& _ui->randomSecondLineEdit->check()))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs exists !"));
				return false;
			}

			// 判断输入的参数是否为空

			if (m_Angel < INVALID_DOUBLE || m_LowCutoffFreq < INVALID_DOUBLE || m_HighCutoffFreq < INVALID_DOUBLE || m_SignificantWaveHeight < INVALID_DOUBLE
				|| m_DiscreteWavesNum < INVALID_DOUBLE)
			{
				QMessageBox::warning(this, tr("Warning"), tr("The parameter value is empty, please re-enter!"));
				return false;
			}
			break;
		case Enum_ISSC_Double:

			// 判断输入是否非法
			if (!(_ui->Edit_ShowAngleCbo->check() && _ui->Edit_SignificantWaveHeight->check()
				&& _ui->lineEdit_LowCutoffFreq->check() && _ui->lineEdit_HighCutoffFreq->check()
				&& _ui->Edit_SpectrumHeartCycle->check() && _ui->randomSecondLineEdit->check()))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs exists !"));
				return false;
			}

			// 判断输入的参数是否为空

			if (m_Angel < INVALID_DOUBLE || m_LowCutoffFreq < INVALID_DOUBLE || m_HighCutoffFreq < INVALID_DOUBLE || m_SignificantWaveHeight < INVALID_DOUBLE
				|| m_DiscreteWavesNum < INVALID_DOUBLE || m_SpectrumHeartCycle < INVALID_DOUBLE)
			{
				QMessageBox::warning(this, tr("Warning"), tr("The parameter value is empty, please re-enter!"));
				return false;
			}

			break;
		case Enum_JONSWAP:

			// 判断输入是否非法
			if (!(_ui->Edit_ShowAngleCbo->check() && _ui->Edit_SignificantWaveHeight->check()
				&& _ui->lineEdit_LowCutoffFreq->check() && _ui->lineEdit_HighCutoffFreq->check()
				&& _ui->Edit_SpectrumHeartCycle->check() && _ui->Edit_SpectralWindFactor->check() && _ui->randomSecondLineEdit->check()))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs exists !"));
				return false;
			}

			// 判断输入的参数是否为空
			if (m_Angel < INVALID_DOUBLE || m_LowCutoffFreq < INVALID_DOUBLE || m_HighCutoffFreq < INVALID_DOUBLE || m_SignificantWaveHeight < INVALID_DOUBLE
				|| m_DiscreteWavesNum < INVALID_DOUBLE || m_SpectralWindFactor < INVALID_DOUBLE || m_SpectrumHeartCycle < INVALID_DOUBLE)
			{
				QMessageBox::warning(this, tr("Warning"), tr("The parameter value is empty, please re-enter!"));
				return false;
			}
			break;
		}

		return true;
	}

	QTableWidgetItem* ConfigWaveSpectrumDialog::CreateItem(QString str)
	{
		QTableWidgetItem* item = new QTableWidgetItem(str);
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		return item;
	}

	void ConfigWaveSpectrumDialog::OnTableFreqItemClicked(QModelIndex modeIndex)
	{
		int cur_row = _ui->table_SpectralAnalysisPara->currentRow();
		if (cur_row < 0)
			return;
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 0) != 0)
		{
			_ui->WaveSpectrumTypeCbo->setCurrentText(_ui->table_SpectralAnalysisPara->item(cur_row, 0)->text());
		}
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 1) != 0)
		{
			_ui->Edit_SignificantWaveHeight->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 1)->text());
		}
		//_ui->ShowAngleCbo->setCurrentText(_ui->table_SpectralAnalysisPara->item(cur_row, 2)->text());
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 2) != 0)
		{
			_ui->Edit_ShowAngleCbo->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 2)->text());
		}
		
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 3) != 0)
		{
			_ui->lineEdit_LowCutoffFreq->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 3)->text());
		}
		
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 4) != 0)
		{
			_ui->lineEdit_HighCutoffFreq->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 4)->text());
		}
		if (_ui->table_SpectralAnalysisPara->item(cur_row, 6) != 0)
		{
			_ui->randomSecondLineEdit->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 6)->text());
		}

		PreProcessing_Curve->delAllCurves();

		switch (_ui->WaveSpectrumTypeCbo->currentIndex())
		{
		case Enum_ITTC_Single:

			SinglePara_ITTC_Curve();
			break;
		case Enum_ISSC_Double:
			if (_ui->table_SpectralAnalysisPara->item(cur_row, 6) != 0)
			{
				_ui->Edit_SpectrumHeartCycle->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 6)->text());
			}

			DoublePara_ISSC_Curve();
			break;
		case Enum_JONSWAP:
			if (_ui->table_SpectralAnalysisPara->item(cur_row, 7) != 0)
			{
				_ui->Edit_SpectrumHeartCycle->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 7)->text());
			}
			if (_ui->table_SpectralAnalysisPara->item(cur_row, 8) != 0)
			{
				_ui->Edit_SpectralWindFactor->setText(_ui->table_SpectralAnalysisPara->item(cur_row, 8)->text());
			}

			JONSWAP_Curve();
			break;
		}
	}

	//添加
	void ConfigWaveSpectrumDialog::OnAddBtn_Spectral()
	{
		Spectral_Set(Enum_Add);
		_ui->table_SpectralAnalysisPara->scrollToBottom();//wzy20181012新增，添加后自动滚到最后一项
	}

	//修改
	void ConfigWaveSpectrumDialog::OnModifyBtn_Spectral()
	{
		Spectral_Set(Enum_Modify);
	}

	//删除
	void ConfigWaveSpectrumDialog::OnDeleteBtn_Spectral()
	{
		PreProcessing_Curve->delAllCurves();

		int cur_row = _ui->table_SpectralAnalysisPara->currentRow();
		if (cur_row < 0)
			return;
		_ui->table_SpectralAnalysisPara->removeRow(cur_row);
	}

	//ITTC单参数谱
	void ConfigWaveSpectrumDialog::SinglePara_ITTC_Curve()
	{
		GetValue();
		generate_ITTC_Single_Curve(m_SignificantWaveHeight, m_LowCutoffFreq, m_HighCutoffFreq, m_Nrwvcomp);
	}

	// ISSC双参数谱
	void ConfigWaveSpectrumDialog::DoublePara_ISSC_Curve()
	{
		GetValue();
		generate_ISSC_Double_Curve(m_SignificantWaveHeight, m_LowCutoffFreq, m_HighCutoffFreq, m_SpectrumHeartCycle, m_Nrwvcomp);
	}

	// JONSWAP海浪谱
	void ConfigWaveSpectrumDialog::JONSWAP_Curve()
	{
		GetValue();
		generate_JONSWAP_Curve(m_SignificantWaveHeight, m_LowCutoffFreq, m_HighCutoffFreq, m_SpectrumHeartCycle, m_SpectralWindFactor, m_Nrwvcomp);
	}

	void ConfigWaveSpectrumDialog::GetValue()
	{
		m_SignificantWaveHeight = _ui->Edit_SignificantWaveHeight->getDoubleData();
		m_LowCutoffFreq = _ui->lineEdit_LowCutoffFreq->getDoubleData();
		m_HighCutoffFreq = _ui->lineEdit_HighCutoffFreq->getDoubleData();
		m_SpectrumHeartCycle = _ui->Edit_SpectrumHeartCycle->getDoubleData();
		m_SpectralWindFactor = _ui->Edit_SpectralWindFactor->getDoubleData();
		m_DiscreteWavesNum = _ui->randomSecondLineEdit->getDoubleData();
		m_Angel = _ui->Edit_ShowAngleCbo->getDoubleData();
		//m_Nrwvcomp = data->getSpectralNrwvcomp().toInt();
		m_Nrwvcomp = 500;//由外部获取，暂时使用默认值500
		PreProcessing_Curve->replot();
	}

	void ConfigWaveSpectrumDialog::on_WaveSpectrumTypeCbo_currentChanged(int index)
	{
		InitShow(index);
	}

	void ConfigWaveSpectrumDialog::InitShow(int index)
	{
		_ui->label_SpectrumHeartCycle->setVisible(true);
		_ui->label_SpectralWindFactor->setVisible(true);
		_ui->Edit_SpectrumHeartCycle->setVisible(true);
		_ui->Edit_SpectralWindFactor->setVisible(true);


		switch (index)
		{
		case Enum_ITTC_Single:
			_ui->label_SpectrumHeartCycle->setVisible(false);
			_ui->Edit_SpectrumHeartCycle->setVisible(false);
			_ui->label_SpectralWindFactor->setVisible(false);
			_ui->Edit_SpectralWindFactor->setVisible(false);
			break;
		case Enum_ISSC_Double:
			_ui->label_SpectralWindFactor->setVisible(false);
			_ui->Edit_SpectralWindFactor->setVisible(false);
			_ui->label_SpectrumHeartCycle->setText(tr("Spectrum Heart Cycle(T1/s)"));
			break;
		case Enum_JONSWAP:
			_ui->label_SpectrumHeartCycle->setText(tr("Peak Spectral Period(s)"));
			break;
		}

	}

	void ConfigWaveSpectrumDialog::SetCurveProperty(QVector<double> x, QVector < double> y, QString CurveName)
	{
		QString fileName = CurveName;
		PreProcessing_Curve->addCurve(x, y, fileName);
		PreProcessing_Curve->setTitle(CurveName);
		//PreProcessing_Curve->setCurveName(fileName, 1, "curve4_array_y"); //图例
		PreProcessing_Curve->setLegendPosition(0); //无图例
		PreProcessing_Curve->setAxisNum(1);
		//	PreProcessing_Curve->setAxisName("x", QStringLiteral("w(Hz)"));
		PreProcessing_Curve->setAxisName("x", QStringLiteral("ω(rad/s)"));
		PreProcessing_Curve->setAxisName("y", QStringLiteral("s(m².s)"));
		PreProcessing_Curve->setCurveColor(fileName, 1, QColor(255, 0, 0));
		PreProcessing_Curve->setCurveStyle(fileName, 1, Qt::PenStyle::DashDotLine);
		PreProcessing_Curve->setTitleFontSize(15);
		PreProcessing_Curve->setAxisFontSize("x", 12);
		PreProcessing_Curve->setAxisFontSize("y", 12);
		PreProcessing_Curve->replot();
	}

	void ConfigWaveSpectrumDialog::generate_ITTC_Single_Curve(double hs, double min, double max, int nrwvcomp)
	{
		double step = (max - min) / nrwvcomp;
		double A = 8.10 / 1000 * 9.81*9.81;
		double B = 3.11 / (hs*hs);
		double w, w4, w5, s, mi;
		QVector<double> x;
		QVector < double> y;
		x.clear();
		y.clear();

		for (int i = 0; i<nrwvcomp; i++)
		{
			w = min + step*i;
			w4 = w*w*w*w;
			w5 = w4*w;
			mi = -B / w4;
			s = A / w5*pow(10, mi);

			x << w;
			y << s;
		}

		SetCurveProperty(x, y, tr("ITTC single spectrum"));
	}


	void ConfigWaveSpectrumDialog::generate_ISSC_Double_Curve(double hs, double min, double max, double tp, int nrwvcomp)
	{

		double step = (max - min) / nrwvcomp;
		double A = 173 * hs*hs / (tp*tp*tp*tp);
		double B = 691 / (tp*tp*tp*tp);
		double w, w4, w5, s, mi;

		QVector<double> x;
		QVector < double> y;
		x.clear();
		y.clear();

		for (int i = 0; i<nrwvcomp; i++)
		{
			w = min + step*i;
			w4 = w*w*w*w;
			w5 = w4*w;
			mi = -B / w4;
			s = A / w5*pow(2.71828, mi);

			x << w;
			y << s;
		}

		SetCurveProperty(x, y, tr("ITTC double spectrum"));
	}

	void ConfigWaveSpectrumDialog::generate_JONSWAP_Curve(double hs, double min, double max, double tp, double gama, int nrwvcomp)
	{
		double step = (max - min) / nrwvcomp;
		double A = 0.658 * 173 * hs*hs / (tp*tp*tp*tp);
		double B = -691 / (tp*tp*tp*tp);
		double w, w0, w4, w5, s, mi_1, mi_2, shape;

		QVector<double> x;
		QVector < double> y;
		x.clear();
		y.clear();

		for (int i = 0; i<nrwvcomp; i++)
		{
			w = min + step*i;
			w4 = w*w*w*w;
			w5 = w4*w;
			w0 = 1.0 / tp;
			if (w <= w0)
				shape = 0.07;
			else
				shape = 0.09;
			mi_1 = B / w4;
			mi_2 = pow(2.71828, -0.5 / (shape*shape)*(w*tp / 4.85 - 1)*(w*tp / 4.85 - 1));
			s = A / w5*pow(2.71828, mi_1)*pow(3.3, mi_2);
			x << w;
			y << s;
		}

		SetCurveProperty(x, y, tr("JONSWAP  wave spectrum"));
	}

}