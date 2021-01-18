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
		/*计算长度，选取线*/
		void on_geoSelectCurve_clicked();
		/*计算面积，选取面*/
		void on_geoSelectSurface_1_clicked();
		/*计算体积，选取体*/
		void on_geoSelectSurface_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_TypeChanged(int index);

	private:

		void setPointLocation(double* p1, double* p2);
		/*计算两点之间的距离*/
		void DistanceTwoPts(Geometry::GeometrySet* set, int index);
		/*计算弧长的长度*/
		void LengthOfCurve(Geometry::GeometrySet* set, int index);
		/*计算曲面的面积*/ 
		void AreaOfSurface(Geometry::GeometrySet* set, int index);
		/*计算实体的体积*/
		void VolumeOfSolid(Geometry::GeometrySet* set, int index);

		/*点不高亮*/
		void ImHighlightPoints();
		/*线不高亮*/
		void ImHighlightCurve();
		/*面不高亮*/
		void ImHighlightSurface();
		/*体不高亮*/
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
