#include "BREPdataExchange.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <QFileInfo>
#include <QDebug>

namespace Geometry
{
	BREPdataExchange::BREPdataExchange(const QString& _fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId) :
		GeometryThreadBase(mw, res),
		_fileName(_fileName),
		_operation(operation)
	{

	}

	BREPdataExchange::~BREPdataExchange()
	{
		//qDebug() << "Brep Delete!" << endl;
	}

	void BREPdataExchange::run()
	{
		ModuleBase::ThreadTask::run();
		switch (_operation)
		{
		case GEOMETRY_READ:
			emit showInformation(tr("Import Geometry File From \"%1\"").arg(_fileName));
			_isRead = readBrep();
			break;
		case GEOMETRY_WRITE:
			emit showInformation(tr("Export Geometry File From \"%1\"").arg(_fileName));
			_isWrite = writeBrep();
			break;
		}
		deriveGeometryTaskFinished();
	}

	bool BREPdataExchange::readBrep()
	{
		std::string str = _fileName.toStdString();
		const char* c_fn = str.c_str();
		TopoDS_Shape ashape;
		BRep_Builder builder;
		bool ok = BRepTools::Read(ashape, (const Standard_CString)c_fn, builder);
		if (!ok) return false;
		auto geoSet = new GeometrySet(IGES);

		TopTools_HSequenceOfShape* sequanceOfShape = new TopTools_HSequenceOfShape;
		sequanceOfShape->Append(ashape);

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

	bool BREPdataExchange::writeBrep()
	{
		return false;
	}

	void BREPdataExchange::deriveGeometryTaskFinished()
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