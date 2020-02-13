#include "graphWindowBase.h"
#include "mainWindow/mainWindow.h"
#include <QMdiSubWindow>
#include <QDebug>

namespace ModuleBase
{
	GraphWindowBase::GraphWindowBase(GUI::MainWindow* mainwindow,int proID,GraphWindowType type,bool connenttomainwindow) 
		: CModuleBase(mainwindow, ModuleType::GraphWindows), _projectId(proID), _graphWindowType(type)//, _mainWindow(mainwindow)
	{
		if (connenttomainwindow)
		{
			connectSignals();
		}
	//	connect(this, SIGNAL(closeThisWindow(int, int)), mainwindow, SLOT(closeWindow(int, int)));
	}
	GraphWindowBase::~GraphWindowBase()
	{

	}
	GraphWindowType GraphWindowBase::getGraphWindowType()
	{
		return _graphWindowType;
	}
	void GraphWindowBase::saveImage(QString str, int w, int h, bool s)
	{
		
	}
	void GraphWindowBase::connectSignals()
	{
		connect(_mainWindow, SIGNAL(displayModeChangedSig(QString)), this, SLOT(setDisplay(QString)));
		//connect(_mainWindow, SIGNAL(displayGeometryChangedSig(QString)), this, SLOT(setGeometryDisplay(QString)));
		connect(_mainWindow, SIGNAL(selectModelChangedSig(int)), this, SLOT(setSelectModel(int)));
		connect(_mainWindow, SIGNAL(updateGraphOptionsSig()), this, SLOT(updateGraphOption()));
		connect(this, SIGNAL(showGraphRange(double, double)), _mainWindow, SIGNAL(showGraphRangeSig(double, double)));
	}
	int GraphWindowBase::getID()
	{
		return _projectId;
	}
	void GraphWindowBase::setView(QString view)
	{
		if (view.toLower() == "fit")
			fitView();
		else if (view.toLower() == "xplus")
			setViewXPlus();
		else if (view.toLower() == "xminus")
			setViewXMiuns();
		else if (view.toLower() == "yplus")
			setViewYPlus();
		else if (view.toLower() == "yminus")
			setViewYMiuns();
		else if (view.toLower() == "zplus")
			setViewZPlus();
		else if (view.toLower() == "zminus")
			setViewZMiuns();
	}
	void  GraphWindowBase::setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
	{
		setViewValue(x1, x2, x3, y1, y2, y3, z1, z2, z3);
	}
	/*void  GraphWindowBase::setViewPreValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3)
	{
	setViewPreValue(x1, x2, x3, y1, y2, y3, z1, z2, z3);
	}*/
	void GraphWindowBase::fitView()
	{
	}
	void GraphWindowBase::setViewXPlus()
	{
	}
	void GraphWindowBase::setViewXMiuns()
	{
	}
	void GraphWindowBase::setViewYPlus()
	{
	}
	void GraphWindowBase::setViewYMiuns()
	{
	}
	void GraphWindowBase::setViewZPlus()
	{
	}
	void GraphWindowBase::setViewZMiuns()
	{
	}
	void GraphWindowBase::setBackGround(QColor color1, QColor color2)
	{

	}
	void GraphWindowBase::setDisplay(QString m)
	{
//		qDebug() << m;
	}

	/*void GraphWindowBase::setGeometryDisplay(QString m)
	{

	}*/

	void GraphWindowBase::setSelectModel(int mode)
	{
//		qDebug() << mode;
	}
	void GraphWindowBase::keyPressEvent(QKeyEvent *e)
	{
		qDebug() << "base press event";
		QWidget::keyPressEvent(e);
	}
	void GraphWindowBase::keyReleaseEvent(QKeyEvent *e)
	{
		qDebug() << "base release event";
		QWidget::keyReleaseEvent(e);
	}
	void GraphWindowBase::updateGraphOption()
	{

	}

	QString GraphWindowBase::getStringGraphWindowType()
	{
		QString type;
		switch (_graphWindowType)
		{
		case ModuleBase::UnDefined:
			break;
		case ModuleBase::PreWindows:
			type = "PreWindow"; break;
		case ModuleBase::RealTime:
			type = "RealTime"; break;
		case ModuleBase::Post2D:
			type = "Post2D"; break;
		case ModuleBase::Post3D:
			type = "Post3D"; break;
		default:
			break;
		}
		return type;
	}



}