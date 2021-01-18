#ifndef DIALOGMAKEFILLGAP_H_
#define DIALOGMAKEFILLGAP_H_
 
#include "geoDialogBase.h"
#include <QMultiMap>
namespace Ui
{
	class DialogFillGap;
}

namespace GeometryWidget
{
	class GEOMETRYWIDGETSAPI MakeFillGapDialog : public GeoDialogBase
	{
		Q_OBJECT
	public:
		MakeFillGapDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		MakeFillGapDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~MakeFillGapDialog();
		
	private slots:
	
		void on_geoSelectCurve_clicked();
		void on_geoSelectSurface_1_clicked();
		void on_geoSelectSurface_clicked();
		void shapeSlected(Geometry::GeometrySet* set, int index) override;
		void on_TypeChanged(int index);
		
	private:
		void FillGapOfCurve(Geometry::GeometrySet* set, int index);
		void FillGapOfSurface(Geometry::GeometrySet* set, int index);
		void FillGapOfSolid(Geometry::GeometrySet* set, int index);

		/*线高亮*/
		void HighlightCurve(bool f);
		/*面高亮*/
		void HighlightSurface(bool f);
		/*体高亮*/
		void HighlightSolid(bool f);

//		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;
		
	private:
		Ui::DialogFillGap* _ui{};
		int _typeindex{ 0 };
		QList<QPair<Geometry::GeometrySet*, int>> _curve{};
		QList<QPair<Geometry::GeometrySet*, int>> _surface{};
		QList<QPair<Geometry::GeometrySet*, int>> _solid{};


		
	};

}


#endif
