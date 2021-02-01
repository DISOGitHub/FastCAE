#ifndef _GEOMETRYPARAMAKEMATRIX_H_
#define _GEOMETRYPARAMAKEMATRIX_H_

#include "geometryModelParaBase.h"
#include <QString>
#include <QMultiHash>
namespace Geometry
{
	class GeometrySet;
	class GEOMETRYAPI GeometryParaMatrix : public GeometryModelParaBase
	{
	public:
		GeometryParaMatrix();
		~GeometryParaMatrix() = default;

		void appendBody(Geometry::GeometrySet* set, int bodyindex);
		QMultiHash<Geometry::GeometrySet*, int> getBodys();

		void setCurrentIndex(int index);
		int getCurrentIndex();

		void setDirection1(double* dir);
		void getDirection1(double* dir);

		void setReverse1(bool s);
		bool getReverse1();

		void setDistance1(double dis);
		double getDistance1();

		void setCount1(int n);
		int getCount1();

		void setShowDir2(bool s);
		bool showtDir2();

		void setDirection2(double* dir);
		void getDirection2(double* dir);

		void setReverse2(bool s);
		bool getReverse2();

		void setDistance2(double dis);
		double getDistance2();

		void setCount2(int n);
		int getCount2();

		void setAxis(double* axis);
		void getAxis(double* axis);

		void setAxisDir(double* dir);
		void getAxisDir(double* dir);

		void setAxisRev(bool s);
		bool getAxisRev();

		void setAxisCount(int n);
		int getAxisCount();

		void setAngle(double angle);
		double getAngle();

		void setOriSet(Geometry::GeometrySet* s);
		Geometry::GeometrySet* getOriSet();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;


	private:
		Geometry::GeometrySet* _oriset{};
		QMultiHash<Geometry::GeometrySet*, int>_solidHash{};
		int _optionindex{};

		double _dir1[3]{};
		bool _reverse1{};
		double _dis1{};
		int _count1{};

		bool _showdir2{};
		double _dir2[3]{};
		bool _reverse2{};
		double _dis2{};
		int _count2{};

		double _axis[3]{};
		double _axisdir[3]{};
		bool _axisreverse{};
		int _axisacount{};
		double _axisangle{};
		
	};
}


#endif