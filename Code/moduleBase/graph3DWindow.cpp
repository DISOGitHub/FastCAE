#include "graph3DWindow.h"
#include "ui_graph3DWindow.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkAxesActor.h>
#include <vtkTextActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkTextWidget.h>
#include <vtkTextProperty.h>
#include <vtkTextRepresentation.h>
#include <vtkScalarBarWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkLookupTable.h>
#include <QDebug>
#include <QColor>
#include <QDir>
#include <assert.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageResize.h>
#include <vtkPNGWriter.h>
#include "settings/busAPI.h"
#include "settings/GraphOption.h"
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkAppendFilter.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionActor2D.h>
#include <vtkCaptionWidget.h>
#include <vtkAreaPicker.h>
#include <vtkProperty2D.h>
#include <vtkAppendFilter.h>
#include "PreWindowInteractorStyle.h"
#include "mainWindow/mainWindow.h"
#include "meshData/meshSet.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSingleton.h"
#include "PreWindowInteractorStyle.h"
#include <vtkVersionMacros.h>

namespace ModuleBase
{
    Graph3DWindow::Graph3DWindow(GUI::MainWindow* mainwindow, int id, GraphWindowType type, bool connentToMainWindow)
        :_ui(new Ui::Graph3DWindow), GraphWindowBase(mainwindow, id, type, connentToMainWindow)
    {
        _ui->setupUi(this);
        
        init();
        _render->GlobalWarningDisplayOff();
        this->setFocusPolicy(Qt::ClickFocus);
        connect(_mainWindow, SIGNAL(enableGraphWindowKeyBoard(bool)), this, SLOT(enableKeyBoard(bool)));
        connect(this, SIGNAL(reRenderSig()), this, SLOT(reRender()));
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetParallelProjection(1);

    }
    Graph3DWindow::~Graph3DWindow()
    {
        if (_ui != nullptr)
        {
            delete _ui;
            _ui = nullptr;
        }

        if (_interactionStyle)
            _interactionStyle->Delete();
    }
    void Graph3DWindow::enableKeyBoard(bool on)
    {
        if (on)
            this->grabKeyboard();
        else
            this->releaseKeyboard();
    }
    void Graph3DWindow::init()
    { 
#if VTK_MAJOR_VERSION < 9
        _renderWindow = _ui->qvtkWidget->GetRenderWindow();
#else
        _renderWindow = _ui->qvtkWidget->renderWindow();
#endif
        
        _render = vtkSmartPointer<vtkRenderer>::New();
        _render->SetGradientBackground(true);
// 		_render->SetBackground2(0.0, 0.333, 1.0);
// 		_render->SetBackground(1.0, 1.0, 1.0);
        _interactor = _renderWindow->GetInteractor();
        _renderWindow->AddRenderer(_render);
        if (_graphWindowType == PreWindows)
        {
            PropPickerInteractionStyle* style = PropPickerInteractionStyle::New();
            style->connectToMainWindow(_mainWindow,this);
            style->SetDefaultRenderer(_render);
            style->setRender(_render);
            style->setRenderWindow(_renderWindow);
            _interactor->SetInteractorStyle(style);
            vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
            _interactor->SetPicker(areaPicker);
            _interactionStyle = style;

            //关联信号
            connect(style, SIGNAL(selectGeometry(vtkActor*, bool)), this, SIGNAL(selectGeometry(vtkActor*, bool)));
            connect(style, SIGNAL(preSelectGeometry(vtkActor*, QVector<double*>)), this, SIGNAL(preSelectGeometry(vtkActor *, QVector<double*>)));
            
            connect(this, SIGNAL(keyEvent(int, QKeyEvent*)), style, SLOT(keyEvent(int, QKeyEvent*)));
            connect(style, SIGNAL(highLight(QMultiHash<vtkDataSet*, int>*)), this, SIGNAL(highLighSet(QMultiHash<vtkDataSet*, int>*)));
            connect(style, SIGNAL(higtLightActorDisplayPoint(bool)), this, SIGNAL(highLightActorDispalyPoint(bool)));
            connect(style, SIGNAL(clearAllHighLight()), this, SIGNAL(clearAllHighLight()));

            connect(_mainWindow, SIGNAL(selectModelChangedSig(int)), this, SLOT(setSelectType(int)));
            //
            connect(style, SIGNAL(grabKeyBoard(bool)), this, SLOT(enableKeyBoard(bool)));
            connect(style, SIGNAL(mouseWhellMove()), this, SLOT(mouseWheelMove()));
            connect(style, SIGNAL(rightDownMenu()), this, SIGNAL(rightDownMenuSig()));
        }
        initAxes();
        updateGraphOption();
        
    }
    void Graph3DWindow::initAxes()
    {
        vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
        _axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
        _axesWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
        _axesWidget->SetOrientationMarker(axesActor);
        _axesWidget->SetInteractor(_interactor);
        _axesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
        _axesWidget->SetEnabled(1);
        ///禁止交互操作移动位置2
        _axesWidget->InteractiveOff();
    }
    void Graph3DWindow::initText()
    {
        vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
        textActor->SetInput("QianFan");
        textActor->GetTextProperty()->SetColor(0.8, 0.75, 0.75);
        textActor->SetDragable(false);
        _textWidget = vtkSmartPointer<vtkTextWidget>::New();

        vtkSmartPointer<vtkTextRepresentation> textRepresentation = vtkSmartPointer<vtkTextRepresentation>::New();
        textRepresentation->GetPositionCoordinate()->SetValue(.02, 0.9);
        textRepresentation->GetPosition2Coordinate()->SetValue(.3, 0.1);
        _textWidget->SetRepresentation(textRepresentation);

        _textWidget->SetInteractor(_interactor);
        _textWidget->SetTextActor(textActor);
        _textWidget->SelectableOff();
        _textWidget->On();
        _textWidget->SelectableOff();

    }
    void Graph3DWindow::initScalarBar()
    {
        vtkSmartPointer<vtkTextProperty> propLable = vtkSmartPointer<vtkTextProperty>::New();
        propLable->SetBold(0);
        propLable->SetItalic(0);
        propLable->SetShadow(0);
        propLable->SetJustification(VTK_TEXT_LEFT);
        propLable->SetColor(0, 0, 0);
        propLable->SetFontSize(14);

        _scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
        _scalarBarWidget->GetScalarBarActor()->SetVerticalTitleSeparation(1);
        _scalarBarWidget->GetScalarBarActor()->SetBarRatio(0.02);
        _scalarBarWidget->GetBorderRepresentation()->SetPosition(0.90, 0.05);
        _scalarBarWidget->GetBorderRepresentation()->SetPosition2(0.91, 0.45);
        _scalarBarWidget->GetBorderRepresentation()->SetShowBorderToOff();
        _scalarBarWidget->GetScalarBarActor()->SetTitleTextProperty(propLable);
        _scalarBarWidget->GetScalarBarActor()->SetLabelTextProperty(propLable);
        _scalarBarWidget->GetScalarBarActor()->SetUnconstrainedFontSize(true);
//		_scalarBarWidget->GetScalarBarActor()->SetLookupTable(_interactor);
        _scalarBarWidget->SetInteractor(_interactor);
        _scalarBarWidget->Off();
    }
    void Graph3DWindow::updateScalarBar(vtkLookupTable* lookuptable, QString title /* = QString("") */)
    {
        _scalarBarWidget->GetScalarBarActor()->SetLookupTable(lookuptable);
        _lookupTable = lookuptable;
        _scalarBarWidget->On();
    }
    void Graph3DWindow::enableActor(vtkActor* actor, bool show /* = true */)
    {
        if (actor == nullptr) return;
        if (show)
        {
            actor->VisibilityOn();
        }
        else
        {
            actor->VisibilityOff();
        }
        _renderWindow->Render();
    }

     
    void Graph3DWindow::AppendActor(vtkProp* actor, ActorType type, bool reRender, bool reset)
    {
        if (type == ActorType::D3)
        {
            _render->AddActor(actor);
        }
        else if (type == ActorType::D2)
        {
            _render->AddActor2D(actor);
        }
        else assert(0);
        if (reRender)
        {
            if(reset) 
                _render->ResetCamera();
            _renderWindow->Render();
        }
        
    }
    void Graph3DWindow::RemoveActor(vtkProp* actor)
    {
        if (nullptr != _render&& nullptr != actor)
        {
            _render->RemoveActor(actor);
        }
//		_renderWindow->Render();
    }


    void Graph3DWindow::saveImage(QString filePath, int w,int h,bool showdlg)
    {

//		QString dir = Setting::BusAPI::instance()->getWorkingDir();

        vtkSmartPointer<vtkWindowToImageFilter> report_windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        vtkSmartPointer<vtkImageResize> report_resize = vtkSmartPointer<vtkImageResize>::New();
        vtkSmartPointer<vtkPNGWriter> report_writer = vtkSmartPointer<vtkPNGWriter>::New();

        QString png_name =  filePath;

        report_windowToImageFilter->SetInput(_renderWindow);
        report_resize->SetInputConnection(report_windowToImageFilter->GetOutputPort());
        report_resize->SetOutputDimensions(w, h, 1);
        report_resize->Update();
        report_writer->SetFileName(png_name.toLocal8Bit().data());
        report_writer->SetInputConnection(report_resize->GetOutputPort());
        report_writer->Write();
    }
    void Graph3DWindow::updateScalarBarLecel(const int n)
    {
        _lookupTable->SetNumberOfColors(n);
        _scalarBarWidget->GetScalarBarActor()->SetLookupTable(_lookupTable);
//		int na = _render->GetActors()->GetNumberOfItems();
        _renderWindow->Render();
    }
    int Graph3DWindow::getScalarBarLevel()
    {
        if (_lookupTable == nullptr) return 0;
        int n = _lookupTable->GetNumberOfColors();
        return n;
    }
    void Graph3DWindow::resetCamera()
    {
        _render->ResetCamera();
        _renderWindow->Render();
        this->mouseWheelMove();
    }
    void Graph3DWindow::setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(x1, x2, x3);
        camera->SetPosition(y1, y2, y3);
        camera->SetFocalPoint(z1, z2, z3);
        resetCamera();
    }
    void Graph3DWindow::fitView()
    {
        resetCamera();
    }
    void Graph3DWindow::setViewXPlus()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 0, 1);
        camera->SetPosition(5000, 0, 0);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    void Graph3DWindow::setViewXMiuns()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 0, 1);
        camera->SetPosition(-5000, 0, 0);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    void Graph3DWindow::setViewYPlus()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 0, 1);
        camera->SetPosition(0, 5000, 0);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    void Graph3DWindow::setViewYMiuns()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 0, 1);
        camera->SetPosition(0, -5000, 0);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    void Graph3DWindow::setViewZPlus()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 1, 0);
        camera->SetPosition(0, 0, 5000);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    void Graph3DWindow::setViewZMiuns()
    {
        vtkCamera* camera = _render->GetActiveCamera();
        camera->SetViewUp(0, 1, 0);
        camera->SetPosition(0, 0, -1);
        camera->SetFocalPoint(0, 0, 0);
        resetCamera();
    }
    
    void Graph3DWindow::keyPressEvent(QKeyEvent *e)
    {
        emit keyEvent(0, e);
    }
    void Graph3DWindow::keyReleaseEvent(QKeyEvent *e)
    {
        emit keyEvent(1, e);
    }

    SelectModel Graph3DWindow::getSelectModel()
    {
        return _selectModel;
    }
    void Graph3DWindow::setSelectType(int model)
    {
        _selectModel = (SelectModel)model;
    }

    void Graph3DWindow::updateGraphOption()
    {
        Setting::GraphOption* option = Setting::BusAPI::instance()->getGraphOption();
        QColor topcolor = option->getBackgroundTopColor();
        QColor bottomcolor = option->getBackgroundBottomColor();
        _render->SetBackground2(topcolor.redF(), topcolor.greenF(), topcolor.blueF());
        _render->SetBackground(bottomcolor.redF(), bottomcolor.greenF(), bottomcolor.blueF());
        _renderWindow->Render();
    }
    void Graph3DWindow::reTranslate()
    {
        _ui->retranslateUi(this);
    }

    void Graph3DWindow::addCaption(double* pos, QString cap)
    {
        QByteArray array = cap.toLatin1();
        vtkSmartPointer<vtkCaptionRepresentation> captionRepresentation = vtkSmartPointer<vtkCaptionRepresentation>::New();
        captionRepresentation->GetCaptionActor2D()->SetCaption(array.data());
        captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(50);
        captionRepresentation->SetAnchorPosition(pos);
        captionRepresentation->GetCaptionActor2D()->GetProperty()->SetColor(1.0, 0, 0);

        vtkSmartPointer<vtkCaptionWidget> captionWidget = vtkSmartPointer<vtkCaptionWidget>::New();
        captionWidget->SetInteractor(_interactor);
        captionWidget->SetRepresentation(captionRepresentation);
        captionWidget->On();
        _captionList.append(captionWidget);
    }

    void Graph3DWindow::reRender()
    {
        _renderWindow->Render();
    }

    vtkRenderer* Graph3DWindow::getRenderer()
    {
        return _render;
    }

    PropPickerInteractionStyle* Graph3DWindow::getInteractionStyle()
    {
        return _interactionStyle;
    }


    double Graph3DWindow::getWorldHight()
    {
        double s[3] = { 0.0 };
        double e[3] = { 0.0 };
        vtkSmartPointer<vtkCoordinate> coor = vtkSmartPointer<vtkCoordinate>::New();
        coor->SetCoordinateSystemToNormalizedDisplay();
        coor->SetValue(0, 0, 0);
        double* c = coor->GetComputedWorldValue(_render);
        for (int i = 0; i < 3; ++i)
            s[i] = c[i];
        coor->SetValue(0, 1, 0);
        c = coor->GetComputedWorldValue(_render);
        for (int i = 0; i < 3; ++i)
            e[i] = c[i];

        double m = 0;
        for (int i = 0; i < 3; ++i)
            m += pow(e[i] - s[i], 2);

        return sqrt(m);
    }

    double Graph3DWindow::getWorldWidth()
    {
        double s[3] = { 0.0 };
        double e[3] = { 0.0 };
        vtkSmartPointer<vtkCoordinate> coor = vtkSmartPointer<vtkCoordinate>::New();
        coor->SetCoordinateSystemToNormalizedDisplay();
        coor->SetValue(0, 0, 0);
        double* c = coor->GetComputedWorldValue(_render);
        for (int i = 0; i < 3; ++i)
            s[i] = c[i];
        coor->SetValue(1, 0, 0);
        c = coor->GetComputedWorldValue(_render);
        for (int i = 0; i < 3; ++i)
            e[i] = c[i];

        double m = 0; 
        for (int i = 0; i < 3; ++i)
            m += pow(e[i] - s[i], 2);

        return sqrt(m);
    }

    void Graph3DWindow::resizeEvent(QResizeEvent * e)
    {
        QWidget::resizeEvent(e);
        double w = this->getWorldWidth();
        double h = this->getWorldHight();
        emit showGraphRange(w, h);
    }

    void Graph3DWindow::leaveEvent(QEvent *event)
    {
        _renderWindow->Render();
    }

    void Graph3DWindow::enterEvent(QEvent *event)
    {
        _renderWindow->Render();
    }

    void Graph3DWindow::mouseWheelMove()
    {
        double w = this->getWorldWidth();
        double h = this->getWorldHight();
        emit showGraphRange(w, h);
    }
}