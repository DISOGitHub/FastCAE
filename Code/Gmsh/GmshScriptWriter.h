#ifndef GMSHSCRIPTWRITER_H
#define GMSHSCRIPTWRITER_H

#include <QString>
#include <QTextStream>
#include <QMultiHash>

class TopoDS_Compound;

namespace Gmsh{
	class GmshScriptWriter
	{
	public:
		GmshScriptWriter();
		~GmshScriptWriter();

		//дgmsh�ű�
		void writeGmshScript(QString path);
		//�����������ʷ�
		void writeFluidMeshScript(QString dir, QMultiHash<int,int> solid,QList<int> curve, QList<int> surface);
		
		//���������
		void setCompound(TopoDS_Compound* compound);
		//���������ʷ�����
		void setElementType(QString type);
		//���������ʷֽ״�
		void setElementOrder(int order);
		//���������ʷַ���
		void setMethod(int method);
		//���������ʷֳߴ�����
		void setFactor(int factor);
		//������С�ߴ�
		void setMinSize(double min);
		//�������ߴ�
		void setMaxSize(double max);
		//���ù⻬ϵ��
		void setSmooth(int smooth);
		//��������
		void setGeoClean(bool clean);
		//����������
		void setGridCoplanar(bool cop);
		//���þֲ��ܶ�-��
		void setSizePoints(QString points);
		//���þֲ��ܶ�-����
		void setSizeFields(QString fields);
		//����������Χ
		//void setFluidField(QList<double*> field);

	private:
		//gmsh��������
		void writeGeneralSetting(QTextStream* out);
		//gmsh����������
		void writeGridCoplanar(QTextStream* out);
		//�ֲ��ܶ�---��
		void writeSizeAtPoints(QTextStream* out);
		//�ֲ��ܶ�---����
		void writeSizeFields(QTextStream* out);
		//�ֲ��ܶ�---������
		void writeBoxFieldScript(QTextStream* out, QStringList list, int& index);
		//�ֲ��ܶ�---��
		void writeBallFieldScript(QTextStream* out, QStringList list, int& index);
		//�ֲ��ܶ�---Բ��
		void writeCylinderFieldScript(QTextStream* out, QStringList list, int& index);
		//�ֲ��ܶ�---ʵ��
		void writeSolidsFieldScript(QTextStream* out, QStringList list, int& index);
		void writeSolidFieldScript(QTextStream* out, int& index, QMultiHash<int, int> solidHash, double* val, bool back);
		//ȥ����������
		void writeSpecifiedMeshScript(QTextStream* out);
		
		//�����������ʷ�
		void writeFluidFieldScript(QTextStream* out, QList<double> banbox, QList<int> curve, QList<int> surface);


		//	void physicalsGroup(QTextStream* out);
		//	void physicalsScript(QTextStream* out,QString type,QMultiHash<QString,int> pHash);

		//��ȡ��������������,����ֵ����0�� ������0��Ϊ������
		//type 1-�� 2-�� 3-�� 4-ʵ��
		int getShapeIndexInCompound(int setid, int index, int itype);
		QList<int> getShapeIndexListInSolid(QMultiHash<int, int> setHash, int itype);
		//��ȡʵ���Χ��
		//����ֵ Xmin Xmax Ymin Ymax Zmin Zmax
		//����ֵ ����Ϊ��
		QList<double> getSolidBndBox(QMultiHash<int, int> setHash);

		double getFluidMeshSize(double xl, double yl, double zl);
		double getShrinkFactor(double xl, double yl, double zl);

	private:
		int _dim{ -1 };
	
		TopoDS_Compound* _compound{};
		int _elementType{ 0 };
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
		//QString _physicals{};

		//QList<double*> _fluidField{};
	};
}
#endif // GMSHSCRIPTWRITER_H
