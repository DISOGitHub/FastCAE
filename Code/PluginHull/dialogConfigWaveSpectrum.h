#ifndef DIALOGCONFIGWAVESPECTRUM_H
#define DIALOGCONFIGWAVESPECTRUM_H

#include <QWidget>
#include "HullCutPlugin.h"
#include "GeometryWidgets/geoDialogBase.h"
#include "SelfDefObject/selfdeflineedit.h"
#include <QTableWidget>
//#include "PostWidgets\Post2DWidget.h"

namespace Ui
{
    class ConfigWaveSpectrumDialog;
}
namespace GUI
{
    class MainWindow;
}
namespace Post
{
	class Post2DWindow;
}

namespace PluginShip
{
    class HULLPLUGINAPI ConfigWaveSpectrumDialog : public QWidget
    {
        Q_OBJECT
		enum WaveSpectrumType{
			Enum_ITTC_Single = 0,
			Enum_ISSC_Double,
			Enum_JONSWAP
		};

		enum ChangeType{
			Enum_Modify,
			Enum_Add
		};

			
    public:
        explicit ConfigWaveSpectrumDialog(GUI::MainWindow *m);
        ~ConfigWaveSpectrumDialog();

	private:
		void initPost2d();

		void initLineEdit();

		void Spectral_Set(int Operation_Type);

		bool Check_Inputs();

		QTableWidgetItem *CreateItem(QString str);

		void DoublePara_ISSC_Curve();
		void SinglePara_ITTC_Curve();
		void JONSWAP_Curve();

		void GetValue();

		void generate_ITTC_Single_Curve(double hs, double min, double max, int nrwvcomp);
		void generate_ISSC_Double_Curve(double hs, double min, double max, double tp, int nrwvcomp);
		void generate_JONSWAP_Curve(double hs, double min, double max, double tp, double gama, int nrwvcomp);

		void SetCurveProperty(QVector<double> x, QVector < double> y, QString CurveName);

		void InitShow(int index);
	private slots:
		void OnAddBtn_Spectral();
		void OnModifyBtn_Spectral();
		void OnDeleteBtn_Spectral();

		void OnTableFreqItemClicked(QModelIndex modeIndex);

		void on_WaveSpectrumTypeCbo_currentChanged(int index);

    private:
		Ui::ConfigWaveSpectrumDialog *_ui{};
		GUI::MainWindow *_mainwindow{};

		Post::Post2DWindow* PreProcessing_Curve;

		const int m_DecimalNum = 4;

		double m_Angel;
		double m_SignificantWaveHeight;
		double m_LowCutoffFreq;
		double m_HighCutoffFreq;
		double m_SpectrumHeartCycle;
		double m_SpectralWindFactor;
		double m_DiscreteWavesNum;
		double m_Nrwvcomp;
    };
}

#endif // DIALOGCONFIGWAVESPECTRUM_H
