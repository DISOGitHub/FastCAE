#ifndef DIALOGMODELIMPORT_H_
#define  DIALOGMODELIMPORT_H_

#include "HullPluginAPI.h"
#include "GeometryWidgets/geoDialogBase.h"
#include "PostWidgets/qcustomplot.h"
namespace Ui
{
	class ModelImportDialog;
}
namespace GUI
{
	class MainWindow;
}

namespace PluginShip
{


	class HULLPLUGINAPI ModelImportDialog : public QDialog
	{
		Q_OBJECT
	public:
		ModelImportDialog(GUI::MainWindow* m);
		~ModelImportDialog();
	
	private:
		void init();
		void updateDialog();
		void updateTableImport();
		void updateTableSection();
		double getLength(int k);
		void initsection();
		void initwater();
		void getMinMaxValue();
		int findPointlistIndex(int index, double* pt);

	private slots:
		void on_pushButton_import_clicked();
		void on_pushButton_save_clicked();
		void graphClicked(QCPAbstractPlottable*plottable, int dataIndex);
		void getCurveData(int ,QList<QList<double*>> );
	private:
		Ui::ModelImportDialog* _ui{};
		int _Nums{};
		QList<QList<double*>> _pointlist{};
		QList<QList<double*>> _waterlist{};//ˮ��ͼ����
		QList<QList<double*>> _symwaterlist{};//ˮ��ͼ�Գ�����

		QCustomPlot* _customplotsection;
		QCustomPlot* _customplotwater;

		double _minX{};
		double _maxX{};
		double _minY{};
		double _maxY{};
		double _minZ{};
		double _maxZ{};
		
	};
}



#endif