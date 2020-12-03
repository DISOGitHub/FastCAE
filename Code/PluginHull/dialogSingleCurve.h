#ifndef DIALOGSINGLECURVE_H_
#define  DIALOGSINGLECURVE_H_

#include "HullPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"
#include "PostWidgets/qcustomplot.h"
namespace Ui
{
	class SingleCurveDialog;
}
namespace PluginShip
{

	class HULLPLUGINAPI SingleCurveDialog : public QDialog
	{
		Q_OBJECT
	public:
		SingleCurveDialog(int index, QList<QList<double*>> r);
		~SingleCurveDialog();
		
	signals:
		void signal_dataChanged(int index, QList<QList<double*>>data);
	private:
		void updateTable();
		void updateCustom(); 
		void updateData();
		void accept() override;
	private slots:
		void on_pushButtonRedraw_clicked();
		void on_pushButtonAdd_clicked();
		void on_pushButtonCancel_clicked();
		void getAxis(double, double);
 
	private:
		Ui::SingleCurveDialog* _ui{};
		int _curveindex{};
		QList<QList<double*>> _waterlist{};
		QCustomPlot* _singlecustom{};
	};
}



#endif