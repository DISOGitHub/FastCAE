#ifndef GEOMETRYSET_H
#define GEOMETRYSET_H

#include "geometryAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>
//#include <TopoDS_Shape.hxx>
//#include <vtkSmartPointer.h>

class TopoDS_Shape;
class vtkDataSet;

namespace Geometry
{
	enum GeometryType
	{
		NONE = 0,
		IGES,
		STEP,
		STL,
		DATUM, 
	};

	class GeometryModelParaBase;

	class GEOMETRYAPI GeometrySet : public DataProperty::DataBase
	{
	public:
		//���캯��
		GeometrySet(GeometryType type = NONE, bool needID = true);
		~GeometrySet();
		//ɾ��ȫ������״���ͷ�����״�ڴ�
		void releaseSubSet();
		void setID(int id) override;
		//�������ID����Ҫ���׵���
		static void resetMaxID();
		//����ID��ȡ����״
		GeometrySet* getSetByID(int id);
// 		int getID();
// 		void setName(const QString& name);
// 		QString getName();
// 		void setFilePath(const QString& filepath);
// 		QString getFilePath();

		//��ȡ���������� 1-�� 2-�� 3-�� 4-ʵ�壬���󷵻�ֵ��-1
		int getGeoMemberCount(int type);
		//��ȡ��index����״ type 1-�� 2-�� 3-�� 4-ʵ�壬
		TopoDS_Shape* getShape(int type, int index);
		////��ȡ��index����״ type 1-�� 2-�� 3-�� 4-ʵ�壬
		const TopoDS_Shape& getRealShape(int type, int index);
		//���ÿɼ���
		void setVisible(bool v);
		//��״�Ƿ�ɼ�
		bool isVisible();
		//��������
		void setType(GeometryType type);
		//��ȡ����
		GeometryType getType();
		//������״����
		void setShape(TopoDS_Shape* shape);
		//��ȡ��״����
		TopoDS_Shape* getShape();
// 		void setStlDataSet(vtkSmartPointer<vtkDataSet> polyData);
// 		vtkDataSet* getStlDataSet();
		//��ȡ���ID
		static int getMaxID();
		//���ò�������
		void setParameter(GeometryModelParaBase* p);
		//��ȡ��������
		GeometryModelParaBase* getParameter();
		bool isEditable();
		//�Ƴ�����״
		void removeSubSet(GeometrySet* set);
		//�������״
		void appendSubSet(GeometrySet* set);
		//��ȡ����״��Ŀ
		int getSubSetCount();
		//��ȡ��index������״
		GeometrySet* getSubSetAt(int index);

		void dataToStream(QDataStream* s) override;
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele, bool isDisp = false);
		virtual void readDataFromProjectFile(QDomElement* e, bool isDiso = false);
		//д��brep�ļ���·�����ܳ�������
		bool writeBrep(QString name);
		//����brep�ļ���·�����ܳ�������
		bool readBrep(QString name);

	protected:
		void writeSubSet(QDomDocument* doc, QDomElement* parent, bool isDiso = false);
		void readSubSet(QDomElement* e, bool isDiso = false);

	protected:
//		int _id{ -1 };
		GeometryType _type{ NONE };
//		QString _name{};
//		QString _filePath{};
		bool _visible{ true };
		TopoDS_Shape* _shape{};
//		vtkSmartPointer<vtkDataSet> _polyData{};
		QList<GeometrySet*> _subSetList{};
		GeometryModelParaBase* _parameter{};

	private:
		static int idOffset;
		static TopoDS_Shape* tempShape;
	};
}

#endif
