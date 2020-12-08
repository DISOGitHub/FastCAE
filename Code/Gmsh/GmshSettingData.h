#ifndef GMSHSETTINGDATA_H
#define GMSHSETTINGDATA_H

#include <QList>
#include <QMultiHash>
#include <QString>
#include "GmshModuleAPI.h"
#include "DataProperty/DataBase.h"

namespace Gmsh{
	class LocalField;
	class LocalPoint;
	class LocalDensity;

	class GMSHAPI GmshSettingData:public DataProperty::DataBase
	{
	public:
		GmshSettingData();
		~GmshSettingData();
		
		void copy(DataBase* data) override;

		//����ʵ��Hash
		void setSolidHash(QMultiHash<int, int> sh);
		//��ȡʵ��Hash
		QMultiHash<int, int> getSolidHash();
		//�������ʷ�Hash
		void setSurfaceHash(QMultiHash<int, int> sh);
		//��ȡ���ʷ�Hash
		QMultiHash<int, int> getSurfaceHash();
		//�����ʷ�����
		void setElementType(QString type);
		//��ȡ�ʷ�����
		QString getElementType();
		//�����ʷֽ״�
		void setElementOrder(int order);
		//��ȡ�ʷֽ״�
		int getElementOrder();
		//�����ʷַ���
		void setMethod(int m);
		//��ȡ�ʷַ���
		int getMethod();
		//���óߴ�����
		void setSizeFactor(double sf);
		//��ȡ�ߴ�����
		double getSizeFactor();
		//������С�ߴ�
		void setMinSize(double min);
		//��ȡ��С�ߴ�
		double getMinSize();
		//�������ߴ�
		void setMaxSize(double max);
		//��ȡ���ߴ�
		double getMaxSize();
		//������������
		void setGeoClean(bool c);
		//��ȡ��������
		bool getGeoClean();
		//���ù⻬��
		void setSmoothIteration(int smooth);
		//��ȡ�⻬��
		int getSmoothIteration();
		//��������������
		void setGridCoplanar(bool gc);
		//��ȡ���������� 
		bool getGridCoplanar();
		//���þֲ��ܶ�
		void setSizeAtPoints(QString ps);
		void setSizeFields(QString fs);
		void setLocalDesities(QList<LocalDensity*> list);
		//��ȡ�ֲ��ܶ�
		QList<LocalDensity*> getLocalDesities();
		//����ȫѡ״̬
		void setSelectAll(bool b);
		//��ȡȫѡ״̬
		bool getSelectAll();
		//����ѡ��ɼ���״̬
		void setSelectVisiable(bool b);
		//��ȡѡ��ɼ���״̬
		bool getSelectVisiable();
		//����������
		void setMeshID(int id);
		//��ȡ������
		int getMeshID();
		//����ָ����Ԫ����
		void setCells(QList<int> cells);
		//��ȡָ����Ԫ����
		QList<int> getCells();

	private:
		void appendBoxField(QStringList list);
		void appendBallField(QStringList list);
		void appendCylinderField(QStringList list);
		void appendSolidsField(QStringList list);
		void appendFrustumField(QStringList list);

	private:
		int _meshID{ -1 };
		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _gridCoplanar{ false };
		bool _selectall{ false };
		bool _selectvisible{ false };

		QList<LocalDensity*> _locals{};
		QList<int> _cells{};
	};
}

#endif

