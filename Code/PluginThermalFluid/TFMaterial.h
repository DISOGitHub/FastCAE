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
		double _rho{ 0 };			//密度
		double _cpp{ 0 };			//热熔
		double _ktherm{ 0 };		//导热系数
		double _emiss{ 0 };			//发射率
		double _absorp{ 0 };		//吸收率
		double _specularity{ 0 };	//反射率
		double _remiss{ 0 };		//反发射率
		double _rabsor{ 0 };		//反吸收率
		double _rsolspe{ 0 };		//反反射率

		QHash<QString, QString> _propertyHash{};

	};
}

#endif // TFMATERAIL_H

