#include "DialogGraphOption.h"
#include "GraphOption.h"
#include "ui_DialogGraphOption.h"
#include "mainWindow/mainWindow.h"

namespace Setting
{
	GraphOptionDialog::GraphOptionDialog(GUI::MainWindow* mainwindow, GraphOption* op) :/* QFDialog(mainwindow),*/
		_ui(new Ui::GraphOptionDialog), _mainWindow(mainwindow), _graphOption(op)
	{
		_ui->setupUi(this);
		connect(this, SIGNAL(updateGraph()), mainwindow, SIGNAL(updateGraphOptionsSig()));
		init();
	}
	GraphOptionDialog::~GraphOptionDialog()
	{
		if (_ui != nullptr) delete _ui;
	}
	void GraphOptionDialog::init()
	{
		bool has = false;
		QColor color;
		//background top
		_ui->bgTopComboBox->appendBackgroundColors();
		color = _graphOption->getBackgroundTopColor();
		has = _ui->bgTopComboBox->hasColor(color);
		if (!has) _ui->bgTopComboBox->updateOtherColor(color);
		_ui->bgTopComboBox->setCurrentColor(color);
		
		//background bottom
		_ui->bgBottomComboBox->appendBackgroundColors();
		color = _graphOption->getBackgroundBottomColor();
		has = _ui->bgBottomComboBox->hasColor(color);
		if (!has) _ui->bgBottomComboBox->updateOtherColor(color);
		_ui->bgBottomComboBox->setCurrentColor(color);

		//high light
		_ui->highLightComboBox->appendPredefinedColors();
		color = _graphOption->getHighLightColor();
		has = _ui->highLightComboBox->hasColor(color);
		if (!has) _ui->highLightComboBox->updateOtherColor(color);
		_ui->highLightComboBox->setCurrentColor(color);

		//pre high light
		_ui->preHighLightcomboBox->appendPredefinedColors();
		color = _graphOption->getPreHighLightColor();
		has = _ui->preHighLightcomboBox->hasColor(color);
		if (!has) _ui->preHighLightcomboBox->updateOtherColor(color);
		_ui->preHighLightcomboBox->setCurrentColor(color);

		//geo surface color
		_ui->geoSurfaceComboBox->appendPredefinedColors();
		color = _graphOption->getGeometrySurfaceColor();
		has = _ui->geoSurfaceComboBox->hasColor(color);
		if (!has) _ui->geoSurfaceComboBox->updateOtherColor(color);
		_ui->geoSurfaceComboBox->setCurrentColor(color);

		//geo curve color
		_ui->geoCurveComboBox->appendPredefinedColors();
		color = _graphOption->getGeometryCurveColor();
		has = _ui->geoCurveComboBox->hasColor(color);
		if (!has) _ui->geoCurveComboBox->updateOtherColor(color);
		_ui->geoCurveComboBox->setCurrentColor(color);

		//geo point color
		_ui->geoPointComboBox->appendPredefinedColors();
		color = _graphOption->getGeometryPointColor();
		has = _ui->geoPointComboBox->hasColor(color);
		if (!has) _ui->geoPointComboBox->updateOtherColor(color);
		_ui->geoPointComboBox->setCurrentColor(color);

		//mesh face
		_ui->meshFaceComboBox->appendPredefinedColors();
		color = _graphOption->getMeshFaceColor();
		has = _ui->meshFaceComboBox->hasColor(color);
		if (!has) _ui->meshFaceComboBox->updateOtherColor(color);
		_ui->meshFaceComboBox->setCurrentColor(color);

		//mesh edge
		_ui->meshEdgeConboBox->appendPredefinedColors();
		color = _graphOption->getMeshEdgeColor();
		has = _ui->meshEdgeConboBox->hasColor(color);
		if (!has) _ui->meshEdgeConboBox->updateOtherColor(color);
		_ui->meshEdgeConboBox->setCurrentColor(color);

		//mesh node
		_ui->meshNodeComboBox->appendPredefinedColors();
		color = _graphOption->getMeshNodeColor();
		has = _ui->meshNodeComboBox->hasColor(color);
		if (!has) _ui->meshNodeComboBox->updateOtherColor(color);
		_ui->meshNodeComboBox->setCurrentColor(color);

		//mesh node size
		float nodesize = _graphOption->getMeshNodeSize();
		_ui->meshNodeSpinBox->setValue(nodesize);

		//mesh edge size
		float edgewidth = _graphOption->getMeshEdgeWidth();
		_ui->meshEdgeSpinBox->setValue(edgewidth);

		//geo point size
		float pointsize = _graphOption->getGeoPointSize();
		_ui->geoPointSpinBox->setValue(pointsize);

		//geo curve width
		float curvewidth = _graphOption->getGeoCurveWidth();
		_ui->geoCurveSpinBox->setValue(curvewidth);

		int trans = _graphOption->getTransparency();
		_ui->TranspSlider->setValue(trans);
	}
	void GraphOptionDialog::accept()
	{
		_graphOption->setBackgroundTopColor(_ui->bgTopComboBox->currentColor());
		_graphOption->setBackgroundBottomColor(_ui->bgBottomComboBox->currentColor());
		_graphOption->setHighLightColor(_ui->highLightComboBox->currentColor());
		_graphOption->sePretHighLightColor(_ui->preHighLightcomboBox->currentColor());
		_graphOption->setGeometrySurfaceColor(_ui->geoSurfaceComboBox->currentColor());
		_graphOption->setGeometryCurveColor(_ui->geoCurveComboBox->currentColor());
		_graphOption->setGeometryPointColor(_ui->geoPointComboBox->currentColor());
		_graphOption->setMeshFaceColor(_ui->meshFaceComboBox->currentColor());
		_graphOption->setMeshEdgeColor(_ui->meshEdgeConboBox->currentColor());
		_graphOption->setMeshNodeColor(_ui->meshNodeComboBox->currentColor());
		_graphOption->setMeshNodeSize(_ui->meshNodeSpinBox->value());
		_graphOption->setMeshEdgeWidth(_ui->meshEdgeSpinBox->value());
		_graphOption->setGeoPointSize(_ui->geoPointSpinBox->value());
		_graphOption->setGeoCurveWidth(_ui->geoCurveSpinBox->value());
		_graphOption->setTransparency(_ui->TranspSlider->value());
		emit updateGraph();
		
	}
	void GraphOptionDialog::on_out_ApplyButton_clicked()
	{
		accept();
	}
	void GraphOptionDialog::on_out_CancelButton_clicked()
	{
		QDialog::reject();
//		this->close();
		
	}
	void GraphOptionDialog::on_out_OkButton_clicked()
	{
		
		accept();
		QDialog::accept();
//		QDialog::accepted();
	}

}
