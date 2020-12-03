#ifndef TFMATERIAL_H
#define TFMATERIAL_H

#include <QHash>
#include "DataProperty/DataBase.h"

namespace ThermalFluid{
	class TFMaterial : public DataProperty::DataBase
	{
	public:
		TFMaterial();
		~TFMaterial();

		void setRho(double r);
		double getRho();

		void setCpp(double c);
		double getCpp();

		void setKtherm(double k);
		double getKtherm();

		void setEmiss(double e);
		double getEmiss();

		void setAbsorp(double a);
		double getAbsorp();

		void setSpecularity(double s);
		double getSpecularity();

		void setRemiss(double re);
		double getRemiss();

		void setRabsor(double ra);
		double getRabsor();

		void setRsolspe(double rs);
		double getRsolspe();

		void setProperty(QString p, QString v, double y[3]);

		void appendToPropertyHash(QString p, QString v);
		QString getPropertyValueFromHash(QString p);

		void writeMaterialData(QDomDocument* doc, QDomElement* mater);
		void readMaterialData(QDomElement* aMater);
	private:
		double _rho{ 0 };			//�ܶ�
		double _cpp{ 0 };			//����
		double _ktherm{ 0 };		//����ϵ��
		double _emiss{ 0 };			//������
		double _absorp{ 0 };		//������
		double _specularity{ 0 };	//������
		double _remiss{ 0 };		//��������
		double _rabsor{ 0 };		//��������
		double _rsolspe{ 0 };		//��������

		QHash<QString, QString> _propertyHash{};

	};
}

#endif // TFMATERAIL_H

