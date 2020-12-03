#ifndef MESHSET_H_
#define MESHSET_H_

#include "meshDataAPI.h"
#include "DataProperty/ComponentBase.h"
#include <QString>
#include <QMultiHash>

class vtkDataSet;
class vtkUnstructuredGrid;
class vtkIdTypeArray;

namespace MeshData
{
	enum  SetType
	{
		None = 0,
		Node,
		Element,
		Family,
		BCZone,
		UserDef = 101,
		EndType = 100000,
	};
	
	class SetMember;

	class MESHDATAAPI MeshSet : public DataProperty::ComponentBase
	{
	public :
		//构造函数
		MeshSet(QString name, SetType type);
		MeshSet();
		~MeshSet();
		//获取最大ID
		//int static getMaxID();
		//重置最大ID
		//void static resetMaxID();
		///设置ID，谨慎调用
		//void setID(int id) override;
		///设置类型 
		void setType(SetType t);
		///获取类型
		SetType getSetType();
		//添加成员
		void appendMember(int ker, int id);
		//获取Kernal ID列表
		QList<int> getKernals();
		//根据kernal ID获取成员
		QList<int> getKernalMembers(int k);
		//获取数量
		int getAllCount();
		//临时保存MemberID，当void setKeneralID(int id)时清空，指定为Keneral为id的的子集
		void appendTempMem(int m);
		//设置Kenenal， 与 void appendTempMem(int m)配合使用
		void setKeneralID(int id);
		//是否包含kernal
		bool isContainsKernal(int id);
		//设置可见性
		void  isVisible(bool v);
		//获取可见性
		bool isVisible();
		//合并组件
		void merge(MeshSet* set);
		//减去组件
		void cut(MeshSet* set);
		//md5
		void dataToStream(QDataStream* s) override;	
		//写出到XML文件
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//从XML文件读取数据
		virtual void readDataFromProjectFile(QDomElement* e) override;
		//写出二进制文件
		virtual void writeBinaryFile(QDataStream* dataStream);
		//读入二进制文件
		virtual void readBinaryFile(QDataStream* dataStream);
		//生成可以显示的模型, 每个实例只能调用一次
		virtual void generateDisplayDataSet();
		//获取显示模型
		virtual vtkDataSet* getDisplayDataSet();
		//字符串转化为枚举
		static SetType stringToSettype(QString s);
		//枚举转字符串
		static	QString setTypeToString(SetType);



	protected:
		SetType _type{ None };
		bool _visible{ true };

		QMultiHash<int, int> _members{};  //keneralID - node/elementID

		QList<int> _tempMemberID{};

		vtkDataSet* _displayDataSet{};

	private:
	//	static int maxID;
	};



}


#endif
