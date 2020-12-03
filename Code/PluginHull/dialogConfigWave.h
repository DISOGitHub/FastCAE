#ifndef DIALOGCONFIGWAVE_H
#define DIALOGCONFIGWAVE_H

#include <QWidget>
#include "HullCutPlugin.h"
#include "GeometryWidgets/geoDialogBase.h"
#include "SelfDefObject/selfdeflineedit.h"
#include "qbuttongroup.h"
#include "qtablewidget.h"
#include <qmessagebox.h>
#include "moduleBase/Random.h"

#define INVALID_DOUBLE -1e50  //无效的浮点数
#define PI 3.14
#define GRAVITY 9.81

namespace Ui
{
    class ConfigWaveDialog;
}
namespace GUI
{
    class MainWindow;
}

namespace PluginShip
{
    class HULLPLUGINAPI ConfigWaveDialog : public QWidget
    {
        Q_OBJECT

    public:
        explicit ConfigWaveDialog(GUI::MainWindow *m);
        ~ConfigWaveDialog();

	private slots:
		void AddBtn_SingleRAO();
		void ModifyBtn_RAO();
		void DeleteBtn_RAO();
		void DeleteAllBtn_RAO();
		void OnAddGroup_RaoNew();

		void OnTableItemClicked(QModelIndex modeIndex);


	private:
		void init();
		void InitTab();
		void InitTable_Freq();
		void InitTable_Amplitude();
		void InitTable_WaveDirectionAngle();
		void InitTableWidget(QTableWidget *tablewidget);

		void SetAmplitude();
		void SetFreq();
		void SetWaveLength();
		void Setperiod();
		void SetWaveDirectionAngel();

		void WaveParaRange();

		void RAOParRangeSet();
		void RadioRangeSet();
		void SingleRAO_Set(int Operation_Type);
		QTableWidgetItem* CreateItem(QString str);
		bool CheckIllegalInPut(SelfDefLineEdit *Edit);
		bool CheckParaIsempty(SelfDefLineEdit *Edit);
		bool CheckValueZero(SelfDefLineEdit* Edit);
		bool CheckRepeat();
		void InitCheckBox();

		bool SetRAOPara(QTableWidget *_TableWidget, int column, QString SingleValue_Temp);

    private:
		Ui::ConfigWaveDialog *_ui{};

		double m_FirstValue;
		double m_LastValue;
		int groupNum;

		Random widget;

		double m_SingleValue;
		double m_MaxValue;
		double m_Step;
		double m_Min_Value;

		const int m_DecimalNum = 4;
		QButtonGroup *freqGroup{};



		typedef enum _RAO_Radio_Type
		{
			Enum_Wavelength,
			Enum_Periodic,
			Enum_Freq,
			Enum_Amplitude,
			Enum_WaveDirectionAngle

		}RAO_Radio_Type;

		typedef enum _RAO_Tab_Type
		{
			Tab_Wavelen_Periodic_Freq,
			Tab_Amplitude,
			Tab_WaveDirectionAngle

		}RAO_Tab_Type;

		typedef enum _Single_Set_Type
		{
			Enum_Add,
			Enum_Modify

		}Single_Set_Type;

		typedef enum   _WaveSpectrumType
		{
			Enum_ITTC_Single,
			Enum_ISSC_Double,
			Enum_JONSWAP
		}WaveSpectrumType;
    };
}

#endif // DIALOGCONFIGWAVE_H
