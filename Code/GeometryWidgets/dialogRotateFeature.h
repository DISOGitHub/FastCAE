#ifndef DIALOGROTATEFEATURE_H_
#define DIALOGROTATEFEATURE_H_

#include "geoDialogBase.h"

namespace Geometry
{
    class GeometrySet;
}

namespace Ui
{
    class RotateFeatureDialog;
}

class vtkActor;

namespace GeometryWidget
{
    class GeoPointWidget;

    class GEOMETRYWIDGETSAPI RotateFeatureDialog : public GeoDialogBase
    {
        Q_OBJECT
    public:
        RotateFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p);
		RotateFeatureDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set);
		~RotateFeatureDialog();

    private:
        void init();
		void initEdit();
        void closeEvent(QCloseEvent *) override;
        void reject() override;
        void accept() override;
        bool getVector(double* vec);

    private slots:
        void on_TypeChanged(int index);
        void on_geoSelectSurface_clicked();
        void on_geoSelectCurve_clicked();
        void pointWidgetClicked(GeoPointWidget* w);
        void on_radioButtonUser();
        void selectActorShape(vtkActor* actor, int index, Geometry::GeometrySet* set) override;

    private:
        Ui::RotateFeatureDialog* _ui{};
        GeoPointWidget*  _baseWidget{};

        bool _selectBody{ false };
        bool _selectEdge{ false };

        QList<Geometry::GeometrySet*> _geobodyList{};
        vtkActor* _edgeActor{};
        int _edgeIndex{ -1 };
        Geometry::GeometrySet* _edgeSet{};

    };


}


#endif