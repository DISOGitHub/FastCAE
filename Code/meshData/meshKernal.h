/*************************
网格
*************************/
#ifndef MESHKERNAL_H
#define MESHKERNAL_H

#include "meshDataAPI.h"
#include "DataProperty/DataBase.h"
#include <vtkSmartPointer.h>
#include <QString>

class vtkDataSet;
class vtkCell;
class QDataStream;

namespace MeshData
{
	class MESHDATAAPI MeshKernal : public DataProperty::DataBase
	{
	public:
		MeshKernal();
		~MeshKernal() = default;
		//重置节点和单元的偏移量
		static void resetOffset();

		void setID(int id) override;
// 		int getID();
// 		QString getName();
// 		void setName(const QString &name);
		//设置网格条数据，包括节点和拓扑
		void setMeshData(vtkDataSet* dataset);
		//获取网格数据，vtk表示
		vtkDataSet* getMeshData();
		//获取节点位置，double[3],index从0开始，不加偏移
		double* getPointAt(const int index);
		//获取单元，index从0开始，不加偏移
		vtkCell* getCellAt(const int index);
		//是否可见
		bool isVisible();
		//设置可见状态
		void setVisible(bool v);
		//获取节点数目
		int getPointCount();
		//获取单元数量
		int getCellCount();
 		void setPath(const QString& path);
 		QString getPath();
		//标注网格维度
		void setDimension(int d);
		//获取网格维度
		int getDimension();

		void dataToStream(QDataStream* s) override;
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele) override;
		void readDataFromProjectFile(QDomElement* e) override;
		//写出二进制文件
		void writeBinaryFile(QDataStream* dataStream);
		//读入二进制文件
		void readBinaryFile(QDataStream* dataStream);

	private:
		///设置节点ID偏移量，为读入工程文件调用，谨慎修改
		void setPointIDOFfset(int offset);
		///设置单元ID偏移量，为读入工程文件调用，谨慎修改
		void setCellIDOFfset(int offset);

	private:
		bool _visible{ true };
		QString _path{};
		int _pointIDOffset{ -1 };  //起始ID，终止ID=起始ID+mesh.pointnum-1
		int _cellIDOffset{ -1 };
		vtkSmartPointer<vtkDataSet> _mesh{};
		int _dimension{ 3 };

	private:
		static int idOffset;
		static int pointIDOffset;
		static int cellIDOffset;

	};
}

#endif