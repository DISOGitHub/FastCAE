#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include "geometryAPI.h"
#include <QString>
#include <QList>
#include <QStringList>
#include "DataProperty/DataBase.h"

class TopoDS_Shape;
class QDomDocument;
class QDomElement;
class QDomNodeList;
class gp_Ax3;
class vtkPolyData;

namespace Geometry
{
	class GeometrySet;
	class GeometryDatum;
	class GeoComponent;

	class GEOMETRYAPI GeometryData : public DataProperty::DataBase 
	{
	public:
		//��ȡ����ָ��
		static GeometryData* getInstance();
		//�����״
		void appendGeometrySet(GeometrySet* set);
		//��ӻ�׼
		void appendGeometryDatum(GeometryDatum* datum);
		//��ȡȫ����׼
		QList<GeometryDatum*> getGeometryDatum();
		//��ȡ��״����
		int getGeometrySetCount();
		//���������Ƿ�Ϊ��
		bool isEmpty();
		//��ȡ��index����״
		GeometrySet* getGeometrySetAt(const int index);
		//������״��ȡ����
		int getIndexByGeoometrySet(GeometrySet* s);
		//�Ƴ���index����״
		void removeGeometrySet(const int index);
		//�滻��״
		void replaceSet(GeometrySet* newset, GeometrySet* oldset);
		//�Ƴ�������״�������ڵݹ����������״
		void removeTopGeometrySet(GeometrySet* set);
		//�Ƿ���������״���ݹ�����״ȫ������
		bool hasGeometrySet(GeometrySet* set);
//		void removeDatumPlaneByIndex(int index);
		//�Ƴ���׼
		void removeGeometryDatum(GeometryDatum* datum);
		//���ÿɼ��ԣ����漰����״
		void setVisable(int index, bool visable);
		//���ȫ������
		void clear();
		//����ID������״���ݹ�����״����
		GeometrySet* getGeometrySetByID(const int id);
		//��ȡ��index������״
		GeometryDatum* getDatumByIndex(const int index);
		QString getMD5();
		//����ID����
		void sort();
		///�ļ�д���������ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* element, bool isdiso = false);
		//�ӹ����ļ���������
		void readFromProjectFile(QDomNodeList* nodelist , bool isdiso = false);
		//���òܻ�ƽ��
		void setSketchPlane(double* loc, double* dir);
		//��ȡ
		gp_Ax3* getSketchPlane();
		//���һ���������
		void appendGeoComponent(GeoComponent*);
		//��ȡ���м������
		QList<GeoComponent*>& getGeoComponentList();
		//���ݼ������������ȡ�������
		GeoComponent* getGeoComponentByIndex(int);
		//���ݼ������ID��ȡ�������
		GeoComponent* getGeoComponentByID(int);
		//ͨ������ɾ���������
		bool removeGeoComponentByIndex(int);
		//�����м�����������ʷ֣�Ȼ����������д��XML��
		//void generateMeshAndWriteToXML(QDomDocument& doc, QDomElement& parent);

	private:
		GeometryData() = default;
		~GeometryData();

	private:
		static GeometryData* _instance;
		QList<GeometrySet*> _geometryList{};
		QList<GeometryDatum*> _geomtretryDatumList{};
		gp_Ax3* _sketchPlan{};
		QList<GeoComponent*> _geoCpList;
//		QHash<int, vtkPolyData*> _geoCp
	};

	bool compareSet(DataProperty::DataBase* s1, DataProperty::DataBase* s2);

}

#endif
