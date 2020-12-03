#ifndef DIALOGMEASUREDISTANCE_H_
#define DIALOGMEASUREDISTANCE_H_
 
#include "geoDialogBase.h"
#include <QMultiMap>
namespace Ui
{
	class MeasureaDistanceDialog;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI MeasureDistanceDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MeasureDistanceDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~MeasureDistanceDialog();

	private slots:
	    void on_closeButton_clicked();
		void on_geoSelectPoint_clicked();
		/*���㳤�ȣ�ѡȡ��*/
		void on_geoSelectCurve_clicked();
		/*���������ѡȡ��*/
		void on_geoSelectSurface_1_clicked();
		/*���������ѡȡ��*/
		void on_geoSelectSurface_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_TypeChanged(int index);

	private:

		void setPointLocation(double* p1, double* p2);
		/*��������֮��ľ���*/
		void DistanceTwoPts(Geometry::GeometrySet* set, int index);
		/*���㻡���ĳ���*/
		void LengthOfCurve(Geometry::GeometrySet* set, int index);
		/*������������*/ 
		void AreaOfSurface(Geometry::GeometrySet* set, int index);
		/*����ʵ������*/
		void VolumeOfSolid(Geometry::GeometrySet* set, int index);

		/*�㲻����*/
		void ImHighlightPoints();
		/*�߲�����*/
		void ImHighlightCurve();
		/*�治����*/
		void ImHighlightSurface();
		/*�岻����*/
		void ImHighlightSolid();

	private:
		Ui::MeasureaDistanceDialog* _ui{};
		int _typeindex{ 0 };
		
		QList<QPair<Geometry::GeometrySet*, int>>_ptlist{};
		double _point[3];
		
		QPair<Geometry::GeometrySet*, int> _curve{};
		QPair<Geometry::GeometrySet*, int> _surface{};
		QPair<Geometry::GeometrySet*, int> _solid{};




		
	};

}


#endif
