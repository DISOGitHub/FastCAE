#include "MSHdataExchange.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include <QFileInfo>
#include <QTextStream>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <QFile>
#include <QMessageBox>

namespace MeshData
{
	MSHdataExchange::MSHdataExchange(const QString& fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
	//	_fileName(fileName),
		_operation(operation), 
		_meshData(MeshData::getInstance()),
		_totalNumber(0),
		MeshThreadBase(fileName, operation, mw),
		_modelId(modelId)
	{
		_fileName = fileName;
		_file = new QFile(_fileName);
	}

	MSHdataExchange::~MSHdataExchange()
	{
		if (_stream != nullptr) 
			delete _stream;
		if (_file != nullptr)
			delete _file;
		_file = nullptr;
		_stream = nullptr;
	}

	QString MSHdataExchange::readLine()
	{
		while (_threadRuning)
		{
			if (_stream->atEnd())
			{
				//_threadRuning = false;
				return QString();;
			}
			QString line = _stream->readLine().simplified();
			if (line.isEmpty()) continue;
			return line;
		}
		return QString();
	}

	bool MSHdataExchange::readHeader()
	{
		QFileInfo tempinfo(_fileName);
		if (!tempinfo.exists()) 
			return false;
		QString name = tempinfo.fileName();
		QString path = tempinfo.filePath();
		QFile tempfile(_fileName);
		if (!tempfile.open(QIODevice::ReadOnly)) 
			return false;
		//QTextStream* tempStream=new QTextStream(&tempfile);
		QTextStream tempStream(&tempfile);
		while (!tempStream.atEnd())
		{
			if (!_threadRuning)return false;
			QString line = tempStream.readLine().simplified();
			if (line.contains("GAMBIT"))
			{
				mMeshType = typeGambit;
				tempfile.close();
				return true;
			}
			else if (line.contains("Fluent"))
			{
				mMeshType = typeFluent;
				tempfile.close();
				return true;
			}			
		}
		//delete tempStream;
		return false;
	}

	bool MSHdataExchange::write()
	{
		if (_modelId)	return false;
	}

	void MSHdataExchange::run()
	{		
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import MSH Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export MSH Mesh File From \"%1\"").arg(_fileName));
//			result = write();
//			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}

	bool MSHdataExchange::read()
	{
		
		if (!readHeader())
		{
			//QMessageBox::warning(nullptr, tr("Prompt"), tr("The file format could not parse the amount!"), QMessageBox::Ok);
			return false;
		}
 			
		//_file = new QFile();
		QFileInfo info(_fileName);
		if (!info.exists()) 
			return false;
		_baseFileName = info.fileName();
		_filePath= info.filePath();
		//_file->setFileName(_fileName);
		if (!_file->open(QIODevice::ReadOnly)) 
			return false;
		_stream = new QTextStream(_file);
		vtkSmartPointer<vtkUnstructuredGrid> dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
		if (mMeshType == typeGambit)
		{
			if (!readGambitFile(dataset))
				return false;
			else
				return true;
		}
		else if (mMeshType==typeFluent)
		{
			if (!readFluentFile(dataset))
				return false;
			else
				return true;
		}

	}

	void MSHdataExchange::readPoints10(vtkUnstructuredGrid* dataset, QString info)
	{
		vtkSmartPointer<vtkPoints> points = dataset->GetPoints();
		if (points == nullptr)
		{
			points = vtkSmartPointer<vtkPoints>::New();
			dataset->SetPoints(points);
		}
		int type = 0;
		int index=0;
		int setid = 0;
		int startIndex = 0;
		int endIndex = 0;
		QStringList infos = info.split(" ");
		if (infos.length() > 5)
		{
			if (!_threadRuning) return;
			index = strToInt(infos.at(0),true);
			setid = strToInt(infos.at(1),true);
			startIndex = strToInt(infos.at(2),true);
			endIndex = strToInt(infos.at(3),true);
			_ND = strToInt(infos.at(5));
			MeshSet* set = new MeshSet;
			set->setType(SetType::Node);
			set->setName("Point");
			MeshSet* s = _idsetList.value(setid);
			if (s != nullptr)
			{
				delete s;
				_idsetList.remove(setid);
			}
		//	_idsetList[setid] = set;

//			vtkIdTypeArray* idlist = vtkIdTypeArray::New();
			double coordinate[3] = { 0.0, 0.0, 0.0 };
			//fluent文件会多出一行(
			if (mMeshType == typeFluent)
			{
				QString line = this->readLine();
			}
	//		
			for (int i = startIndex; i <= endIndex; i++)
			{
				if (!_threadRuning) return;
//				idlist->InsertNextValue(i);
				set->appendTempMem(i);
			
				QString scoor = this->readLine();
				QStringList scoors = scoor.split(" ");
//				qDebug() << i << endl;
				coordinate[0] = scoors.at(0).toDouble();
				coordinate[1] = scoors.at(1).toDouble();
				if (_ND == 3) coordinate[2] = scoors.at(2).toDouble();
				points->InsertNextPoint(coordinate);
			}
//			set->setIDList(idlist);
		}
	}

	void MSHdataExchange::readPoints130(vtkUnstructuredGrid* dataset, QString info)
	{
		vtkSmartPointer<vtkPoints> points = dataset->GetPoints();
		if (points == nullptr)
		{
			points = vtkSmartPointer<vtkPoints>::New();
			dataset->SetPoints(points);
		}
		QStringList infos = info.split(" ");
		const int setid = infos.at(1).toInt();
		const int startIndex = infos.at(2).toInt();
		const int endIndex = infos.at(3).toInt();
		_ND = infos.at(5).toInt();
		MeshSet* set = new MeshSet;
		set->setType(SetType::Node);
		set->setName("001");
		MeshSet* s = _idsetList.value(setid);
		if (s != nullptr)
		{
			delete s;
			_idsetList.remove(setid);
		}
		_idsetList[setid] = set;

//		vtkIdTypeArray* idlist = vtkIdTypeArray::New();
		double coordinate[3] = { 0.0, 0.0, 0.0 };
		for (int i = startIndex; i <= endIndex; ++i)
		{
			if (!_threadRuning) return;
//			idlist->InsertNextValue(i);
			set->appendTempMem(i);
			QString scoor = this->readLine();
			QStringList scoors = scoor.split(" ");
			coordinate[0] = scoors.at(0).toDouble();
			coordinate[1] = scoors.at(1).toDouble();
			if (_ND == 3) coordinate[2] = scoors.at(2).toDouble();
			points->InsertNextPoint(coordinate);
		}
//		set->setIDList(idlist);
	}

	void MSHdataExchange::readCells120(vtkUnstructuredGrid* dataset, QString info)
	{
		QStringList infos = info.split(" ");
		const int setid = infos.at(1).toInt();
		const int startIndex = infos.at(4).toInt();
		const int endIndex = infos.at(5).toInt();
		MeshSet* set = new MeshSet;
		set->setType(SetType::Element);
		set->setName("002");
		MeshSet* s = _idsetList.value(setid);
		if (s != nullptr)
		{
			delete s;
			_idsetList.remove(setid);
		}
		_idsetList[setid] = set;

//		vtkIdTypeArray* idlist = vtkIdTypeArray::New();
		double coordinate[3] = { 0.0, 0.0, 0.0 };
		for (int i = startIndex; i <= endIndex; ++i)
		{
			if (!_threadRuning) return;
//			idlist->InsertNextValue(i);
			set->appendTempMem(i);
			QString sele = this->readLine();
			QStringList selelist = sele.split(" ");
			QList<int> member;
			vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
			for (int i = 0; i < selelist.size(); ++i)
			{
				if (!_threadRuning) return;
				int d = selelist.at(i).toInt();
				if (!member.contains(d))
				{
					member.append(d);
					idlist->InsertNextId(d);
				}
			}
			if (member.size() == 3)
			{
				dataset->InsertNextCell(VTK_TRIANGLE, idlist);
			}
			else if (member.size() == 4)
			{
				dataset->InsertNextCell(VTK_QUAD, idlist);
			}

		}
//		set->setIDList(idlist);
	}

	void MSHdataExchange::readFluentCells12(vtkUnstructuredGrid* dataset, QString info)
	{
		QStringList infos = info.split(" ");
		const int setid = strToInt(infos.at(1),true);
		const int startIndex = strToInt(infos.at(2),true);
		const int endIndex = strToInt(infos.at(3), true);
		MeshSet* set = new MeshSet;
		set->setType(SetType::Element);
		set->setName("002");
		MeshSet* s = _idsetList.value(setid);
		if (s != nullptr)
		{
			delete s;
			_idsetList.remove(setid);
		}
		_idsetList[setid] = set;

//		vtkIdTypeArray* idlist = vtkIdTypeArray::New();
		double coordinate[3] = { 0.0, 0.0, 0.0 };
		for (int i = startIndex; i <= endIndex; ++i)
		{
//			idlist->InsertNextValue(i);
			set->appendTempMem(i);
			QString sele = this->readLine();
			QStringList selelist = sele.split(" ");
			QList<int> member;
			vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
			for (int i = 0; i < selelist.size(); ++i)
			{
				int d = selelist.at(i).toInt();
				if (!member.contains(d))
				{
					member.append(d);
					idlist->InsertNextId(d);
				}
			}
			if (member.size() == 3)
			{
				dataset->InsertNextCell(VTK_TRIANGLE, idlist);
			}
			else if (member.size() == 4)
			{
				dataset->InsertNextCell(VTK_QUAD, idlist);
			}

		}
//		set->setIDList(idlist);
	}

	void MSHdataExchange::readZone45(QString zone)
	{
//		qDebug() << zone;
		QStringList zoneinfo = zone.split(" ");
		const int setid = zoneinfo.at(1).toInt();
		MeshSet* set = _idsetList.value(setid);
		if (set != nullptr)
			set->setName(zoneinfo.at(3));
	}

	bool MSHdataExchange::readFace13(vtkUnstructuredGrid* dataset,QString info)
	{
		int index;
		
		int bctype;
		int facenumber;
		int facetype;
		int elemtype;
		int startIndex;
		int endIndex;
		int setid;
		char*  ch;
//		qDebug() << info;
		QStringList infos = info.split(" ");

		setid = strToInt(infos.at(1),true);
		
		startIndex = strToInt(infos.at(2),true);
		endIndex = strToInt(infos.at(3),true);
		bctype = strToInt(infos.at(4),true);
		facetype = strToInt(infos.at(5));
		
		MeshSet* set = new MeshSet;
		set->setType(SetType::Element);
		set->setName("002");
		MeshSet* s = _idsetList.value(setid);
		if (s != nullptr)
		{
			delete s;
			_idsetList.remove(setid);
		}
		_idsetList[setid] = set;
		//获取面的数量
		facenumber = endIndex - startIndex + 1;
//		vtkIdTypeArray* idlist = vtkIdTypeArray::New();
		double coordinate[3] = { 0.0, 0.0, 0.0 };
		for (int i = 0; i < facenumber; ++i)
		{
			if (!_threadRuning) 
				return false;
			QList<int> member;
//			idlist->InsertNextValue(_staticid);
			set->appendTempMem(_staticid);
			_staticid++;
			QString sele = this->readLine();
			QStringList selelist = sele.split(" ");
			int firstFaceType = 0;
			if (mMeshType == typeGambit)
			{
				//取每一个面对应的类型，不是头中的类型
				if (selelist.count() > 1)
				{
					firstFaceType = strToInt(selelist.at(0));
				}
			}
			else if (mMeshType==typeFluent)//fluent类型文件，面中没有类型，这里取的是头中的类型
			{
				firstFaceType = facetype;
			}
			
			vtkSmartPointer<vtkIdList> pointIdList = vtkSmartPointer<vtkIdList>::New();
			//第一个字符串是单元类型，不需要保存,最后两个字符串不需要保存，一个是cellid,另外一个为0
			//4 1  77  32b0  29 0 1
			int j = 0;
			if (mMeshType == typeFluent)
			{
				j = 0;
			}
			else if (mMeshType == typeGambit)
			{
				j = 1;
			}
			for (j; j <= selelist.count() - 3; j++)
			{
				if (!_threadRuning)
					return false;
				int pointIndex = strToInt(selelist.at(j), true);
				if (pointIndex > 0)
				{
					pointIndex = pointIndex - 1;
					if (!member.contains(pointIndex))
					{
						member.append(pointIndex);
						pointIdList->InsertNextId(pointIndex);
					}
				}
				
			}
			
			if (firstFaceType == 3)
			{
				dataset->InsertNextCell(VTK_TRIANGLE, pointIdList);
			}
			else if (firstFaceType == 4)
			{
				dataset->InsertNextCell(VTK_QUAD, pointIdList);
			}
			else if (firstFaceType == 2)
			{
				dataset->InsertNextCell(VTK_LINE, pointIdList);
			}
			else if (firstFaceType == 5)
			{
				dataset->InsertNextCell(VTK_POLYGON, pointIdList);
			}
		}
//		set->setIDList(idlist);
		return true;
	}

	QString MSHdataExchange::getNextLineInfo(QString line)
	{
		QString nextLine;
		nextLine = line.remove("(").remove(")");
		return nextLine;
	}

	bool MSHdataExchange::isHex(QString line)
	{
		bool isHex = false;
		for (int i = 0; i < line.length(); i++)
		{
			if (line.at(i) > '9')
			{
				isHex = true;
				return isHex;
			}
		}
		return isHex;
	}

	int MSHdataExchange::strToInt(QString line, bool isHex)
	{
		if (isHex)
		{
			return line.toInt(0, 16);
		}
		else
		{
			return line.toInt();
		}
	}

	bool MSHdataExchange::readGambitFile(vtkUnstructuredGrid* dataset)
	{
		QString line;
		do
		{
			if (!_threadRuning) 
				return false;
			line = this->readLine();
			char*  ch;
			QByteArray ba = line.toLatin1();
			ch = ba.data();
			if (line.endsWith(")("))
			{
				line = line.remove("(").remove(")");
				if (line.isEmpty()) continue;
				QStringList slist = line.split(" ");

				int index;
				//取出数字标识
				sscanf(ch, "(%d", &index);
				if (slist.at(0) == "0")
				{
					_describe = slist.at(1);
					_describe.remove("\"");
				}

				if (index == 130)
				{
					readPoints130(dataset, line);
				}
				else if (slist.at(0) == "120")
				{
					readCells120(dataset, line);
				}
				//10为GAMBIT to Fluent File
				else if (index == 10)
				{
					//读取16进制
					readPoints10(dataset, line);
				}
				//读取Face
				if (index == 13)
				{
					//(13(0 1 1a7c9 0))
					//13如果为13标志，在13后插入空格，和其他格式对应上
					QString replaceLine;
					replaceLine = line;
					replaceLine.insert(2, " ");
					readFace13(dataset, replaceLine);
				}
			}
			else if (line.endsWith("))") && line.startsWith("("))
			{
				line = line.remove("(").remove(")");
				if (line.isEmpty()) continue;
				QStringList slist = line.split(" ");
				if (slist.at(0) == "45")
					readZone45(line);				
			}

		} while (!_stream->atEnd());
		_file->close();
		QList<MeshSet*> setList = _idsetList.values();
		if (dataset != nullptr)
		{
			MeshKernal* k = new MeshKernal;
			k->setName(_baseFileName);
			k->setPath(_filePath);
			k->setMeshData((vtkDataSet*)dataset);
			_meshData->appendMeshKernal(k);
			int kid = k->getID();

			for (int i = 0; i < setList.size(); ++i)
			{
				if (!_threadRuning) 
					return false;
				MeshSet* set = setList.at(i);
//				set->setDataSet(dataset);
				set->setKeneralID(kid);
				_meshData->appendMeshSet(set);
			}
			return true;
		}

		for (int i = 0; i < setList.size(); ++i)
		{
			MeshSet* set = setList.at(i);
			delete set;
		}
		_idsetList.clear();
		return false;
	}

	bool MSHdataExchange::readFluentFile(vtkUnstructuredGrid* dataset)
	{
		QString line;
		do
		{
			if (!_threadRuning) 
				return false;
			line = this->readLine();
			char*  ch;
			QByteArray ba = line.toLatin1();
			ch = ba.data();
			line = line.remove("(").remove(")");
			if (line.isEmpty()) continue;
			QStringList slist = line.split(" ");

			int startIndex = 0;
			int index;
			//取出数字标识
			sscanf(ch, "(%d", &index);
			if (slist.at(0) == "0")
			{
				_describe = slist.at(1);
				_describe.remove("\"");
			}
			//10为GAMBIT to Fluent File
			else if (index == 10)
			{

				startIndex = strToInt(slist.at(1), true);
				//如果startindex<1应该是区的标识，不读取
				if (startIndex < 1) continue;
				//读取16进制
				readPoints10(dataset, line);
			}
			//读取Face
			else if (index == 13)
			{
				//face面总的信息不读取
				startIndex = strToInt(slist.at(1), true);
				if (startIndex < 1)
				{
					continue;
				}
				readFace13(dataset, line);
			}
			//读取区域名称，包含边界
			if (index == 39)
			{
				QStringList slist = line.split(" ");
				readZone45(line);
			}
		} while (!_stream->atEnd());
		_file->close();
		QList<MeshSet*> setList = _idsetList.values();
		if (dataset != nullptr)
		{
			MeshKernal* k = new MeshKernal;
			k->setName(_baseFileName);
			k->setPath(_filePath);
			k->setMeshData((vtkDataSet*)dataset);
			_meshData->appendMeshKernal(k);

			int kid = k->getID();

			for (int i = 0; i < setList.size(); ++i)
			{
				MeshSet* set = setList.at(i);
//				set->setDataSet(dataset);
				set->setKeneralID(kid);
				_meshData->appendMeshSet(set);
			}
			return true;
		}

		for (int i = 0; i < setList.size(); ++i)
		{
			MeshSet* set = setList.at(i);
			delete set;
		}
		_idsetList.clear();
		return false;
	}
}