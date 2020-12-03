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

		//��������
		void SetData(vtkDataSet* data);
		//xyz{1, 0, 0}������X����ת��{0, 1, 0}������Y����ת��{0, 0, 1}������Z����ת
		void SetRotateWXYZ(double angle, double x, double y, double z);
		//������Ҫת���ĵ�ԪIDs
		void SetTransCellIDs(vtkIdTypeArray* cellIDs);
		//��xyz�����ƶ�
		void Translate(double x, double y, double z);
		//�������ű�����x = 0 || y = 0 || z = 0ʱ������
		void Scale(double x, double y, double z);
		//��ʼ����
		vtkDataSet* StartTransform();
		//normalsType: 0����㷨������1����Ԫ����������Ԫ������Ҳ�����㷨��
		static vtkPolyData* GetVTKDataNormals(vtkPolyData* data, int normalsType);
		//��ȡ�����id��data��ʾ�����ݼ���seed��ʾ��Ԫid��minAngleС������Ƕȱ�ʾ��һ��ƽ��
		std::set<vtkIdType> GetCoplanarPointId(vtkDataSet* data, vtkIdType seedId, int minAngle);
		//��ȡ���浥Ԫid��data��ʾ�����ݼ���seed��ʾ��id��minAngleС������Ƕȱ�ʾ��һ��ƽ��
		std::set<vtkIdType> GetCoplanarCellId(vtkDataSet* data, vtkIdType seedId, int minAngle);

	private:
		//����ָ����cellid��ȡ��Χcellid��Ϣ
		std::set<VTKIDPAIR> GetNeighborCellID(vtkPolyData* data, int cellId);
		//����ָ����ptid��ȡ��Χptid��Ϣ
		std::set<vtkIdType> GetNeighborPointID(vtkPolyData* data, int ptId, int minAngle);
		//����normal�����ȡ��������нǣ�idsΪ����id
		double GetNormalAngle(vtkDataArray* pointNormals, const VTKIDPAIR& ids);
		//��ȡdataset��������
		vtkPolyData* GetSurfaceData(vtkDataSet* data);
		//��vtkDataSet��idת��Ϊ�������ݵ�id
		vtkIdType ConvertToPolyDataId(vtkIdType seedId, vtkDataSet* data, vtkPolyData* polyData);
		//���������ݵ�idתΪvtkDataSet��id
		std::set<vtkIdType> ConvertToDataSetId(vtkDataSet* data, vtkPolyData* polyData, const std::set<vtkIdType>& coplanarPointIds);

	private:
		vtkSmartPointer<vtkTransform> _transform;
		vtkSmartPointer<vtkIdTypeArray> _cellIDs;
		vtkDataSet* _data;
};

#endif