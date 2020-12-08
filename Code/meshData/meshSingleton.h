/***************************
前处理网格数据
**************************/
#ifndef MESHDATA_H
#define MESHDATA_H

#include "meshDataAPI.h"
#include <QList>
#include "DataProperty/DataBase.h"

class QDomDocument;
class QDomElement;
class QDomNodeList;
class QDataStream;
class vtkDataSet;

namespace MeshData
{
	class MeshKernal;
	class MeshSet;

	class MESHDATAAPI MeshData : public DataProperty::DataBase
	{
	public:
		//获取单例指针
		static MeshData* getInstance();
		//添加Kernal
		void appendMeshKernal(MeshKernal* keneral);
		//获取Kernal数量
		int getKernalCount();
		//获取第index个Kernal
		MeshKernal* getKernalAt(const int index);
		//通过ID获取Kernal
		MeshKernal* getKernalByID(const int id);
		//通过网格的数据表示获取Kernal ID
		int getIDByDataSet(vtkDataSet* datset);
		//移除第index个Kernal
		void removeKernalAt(const int index);
		//移除ID为i的Kernal
		void removeKernalByID(const int id);
		//添加组件
		void appendMeshSet(MeshSet* s);
		//获取组件数目
		int getMeshSetCount();
		//获取第index个组件
		MeshSet* getMeshSetAt(const int index);
		//通过ID获取网格组件
		MeshSet* getMeshSetByID(const int id);
		//通过名字获取组件，大小写敏感
		MeshSet* getMeshSetByName(const QString name);
		//移除第index个组件
		void removeMeshSetAt(const int index);
		//获取与ID为kid的Kernal相关的全部组件ID
		QList<int> getSetIDFromKernal(int kid);
		//清楚数据
		void clear();
		QString getMD5();
		///写出到工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//写出二进制文件
		void writeBinaryFile(QDataStream* dataStream);
		///从工程文件读入数据
		void readFromProjectFile(QDomNodeList* nodelist);
		//读入二进制文件
		void readBinaryFile(QDataStream* dataFile);
		///产生全部组件的显示模型
		void generateDisplayDataSet();
	
	private:
		MeshData() = default;
		~MeshData();

	private:
		static MeshData* _instance;
		QList<MeshKernal*> _meshList{};
		QList<MeshSet*> _setList{};
	};


}

#endif