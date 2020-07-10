#include "IGESdataExchange.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <QFileInfo>
#include <TopoDS_Shape.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Reader.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace Geometry
{
	IGESdataExchange::IGESdataExchange(const QString& fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId) :
		GeometryThreadBase(mw, res),
		_fileName(fileName),
		_operation(operation)
	{

	}

	IGESdataExchange::~IGESdataExchange()
	{

	}

	void IGESdataExchange::run()
	{
		ModuleBase::ThreadTask::run();
		switch (_operation)
		{
		case GEOMETRY_READ:
			emit showInformation(tr("Import Geometry File From \"%1\"").arg(_fileName));
			_isRead = readIges();
			break;
		case GEOMETRY_WRITE:
			emit showInformation(tr("Export Geometry File From \"%1\"").arg(_fileName));
			_isWrite = writeIges();
			break;
		}
		deriveGeometryTaskFinished();
	}

	bool IGESdataExchange::readIges()
	{		
		TopoDS_Shape aShape;
		IGESControl_Controller::Init();
		IGESControl_Reader Reader;
		std::string str = _fileName.toStdString();
		const char* c_fn = str.c_str();
		Standard_Integer status = Reader.ReadFile(c_fn);
		if (status != IFSelect_RetDone) return false;

		Reader.SetReadVisible(Standard_True);
		Reader.PrintCheckLoad(Standard_True, IFSelect_GeneralInfo);
		Reader.ClearShapes();
		Reader.TransferRoots();

		aShape = Reader.OneShape();
		auto geoSet = new GeometrySet(IGES);

		TopTools_HSequenceOfShape* sequanceOfShape = new TopTools_HSequenceOfShape;
		sequanceOfShape->Append(aShape);

		Standard_Integer index = sequanceOfShape->Length();
		TopoDS_Shape theCompSolid = sequanceOfShape->ChangeValue(index);
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = theCompSolid;
		geoSet->setShape(shape);
		QFileInfo info(_fileName);
		const QString name = info.fileName();
		geoSet->setName(name);
		GeometryData::getInstance()->appendGeometrySet(geoSet);
		getResult().append(geoSet);
		return true;
	}

	bool IGESdataExchange::writeIges()
	{
		return false;
	}

	void IGESdataExchange::deriveGeometryTaskFinished()
	{
		ModuleBase::Message msg;
		if (_operation == GEOMETRY_READ)
		{
			if (_isRead)
			{
				emit updateActionsStates();
				emit showInformation(QString("Successful Import Geometry From \"%1\"").arg(_fileName));				
				msg.type = ModuleBase::Normal_Message;
				msg.message = QString("Successful Import Geometry From \"%1\"").arg(_fileName);
				auto result = getResult();
				for (auto set : result)
				{
					if (set != result.last())
						emit ShowSetSig(set, false);
					else
						emit ShowSetSig(set);
				}
			}
			else
			{
				emit showInformation(QString("Failed Import Geometry From \"%1\"").arg(_fileName));
				msg.type = ModuleBase::Error_Message;
				msg.message = QString("Failed Import Geometry From \"%1\"").arg(_fileName);
			}
		}
		else if (_operation == GEOMETRY_WRITE)
		{
			if (_isWrite)
			{
				emit showInformation(QString("Successful Export Geometry From \"%1\"").arg(_fileName));
			}
			else
			{
				emit showInformation(QString("Failed Export Geometry From \"%1\"").arg(_fileName));
				msg.type = ModuleBase::Error_Message;
				msg.message = QString("Failed Export Geometry From \"%1\"").arg(_fileName);
			}
		}
		emit showGeometryMessageSig(msg);
		defaultGeometryFinished();
	}
}
