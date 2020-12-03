#ifndef _VTKDATARELATED_H_
#define _VTKDATARELATED_H_
#include <vtkSmartPointer.h>
#include <set>
#include "IOAPI.h"

class vtkIdTypeArray;
class vtkTransform;
class vtkPolyData;
class vtkDataSet;
class vtkDataArray;

#define VTKIDPAIR std::pair<vtkIdType, vtkIdType>

class IOAPI vtkDataRelated
{
	public:
		vtkDataRelated();
		vtkDataRelated(vtkDataSet* data);
		virtual ~vtkDataRelated() = default;

		//设置数据
		void SetData(vtkDataSet* data);
		//xyz{1, 0, 0}代表绕X轴旋转，{0, 1, 0}代表绕Y轴旋转，{0, 0, 1}代表绕Z轴旋转
		void SetRotateWXYZ(double angle, double x, double y, double z);
		//设置需要转换的单元IDs
		void SetTransCellIDs(vtkIdTypeArray* cellIDs);
		//沿xyz方向移动
		void Translate(double x, double y, double z);
		//设置缩放比例，x = 0 || y = 0 || z = 0时无意义
		void Scale(double x, double y, double z);
		//开始变形
		vtkDataSet* StartTransform();
		//normalsType: 0代表点法向量，1代表单元法向量，单元法向中也包括点法向
		static vtkPolyData* GetVTKDataNormals(vtkPolyData* data, int normalsType);
		//获取共面点id；data表示总数据集，seed表示单元id，minAngle小于这个角度表示在一个平面
		std::set<vtkIdType> GetCoplanarPointId(vtkDataSet* data, vtkIdType seedId, int minAngle);
		//获取共面单元id；data表示总数据集，seed表示点id，minAngle小于这个角度表示在一个平面
		std::set<vtkIdType> GetCoplanarCellId(vtkDataSet* data, vtkIdType seedId, int minAngle);

	private:
		//根据指定的cellid获取周围cellid信息
		std::set<VTKIDPAIR> GetNeighborCellID(vtkPolyData* data, int cellId);
		//根据指定的ptid获取周围ptid信息
		std::set<vtkIdType> GetNeighborPointID(vtkPolyData* data, int ptId, int minAngle);
		//根据normal数组获取两个法向夹角，ids为法向id
		double GetNormalAngle(vtkDataArray* pointNormals, const VTKIDPAIR& ids);
		//提取dataset表面数据
		vtkPolyData* GetSurfaceData(vtkDataSet* data);
		//将vtkDataSet的id转换为表面数据的id
		vtkIdType ConvertToPolyDataId(vtkIdType seedId, vtkDataSet* data, vtkPolyData* polyData);
		//将表面数据的id转为vtkDataSet的id
		std::set<vtkIdType> ConvertToDataSetId(vtkDataSet* data, vtkPolyData* polyData, const std::set<vtkIdType>& coplanarPointIds);

	private:
		vtkSmartPointer<vtkTransform> _transform;
		vtkSmartPointer<vtkIdTypeArray> _cellIDs;
		vtkDataSet* _data;
};

#endif