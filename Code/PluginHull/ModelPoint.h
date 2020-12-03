#ifndef _MODELPOINT_
#define _MODELPOINT_

#include "gp_Pnt.hxx"
#include "HullPluginAPI.h"
namespace PluginShip
{

	class HULLPLUGINAPI ModelPoint
	{
	public:
		ModelPoint();
		~ModelPoint();
		gp_Vec getVec();
		void setVec(gp_Vec vec);
		void setIndex(int i);
		int getIndex();
		void setPoint(gp_Pnt pnt);
		gp_Pnt getPoint();
		void setDistance(double d);
		double getDistance();
		void setCurvature(double c);
		double getCurvature();
		void setSave(bool s);
		bool getSave();
		void setVariance(double s);
		double getVariance();
		void isFinStabilizer(bool b);
		bool getFinStabilizer();
		void setIsJyq(bool s);
		bool getIsJyq();
		void setSkip(bool s);
		bool getSkip();
		void setIsSeaPt(bool s);
		bool getIsSeaPt();
		void setInset(bool s);
		bool getInsert();
	private:
		gp_Vec _vec{};
		gp_Pnt _point{};
		int _index{ -1 };
		double _distance{};
		double _curvature{};
		double _variance{};

		bool _save{ false };
		bool _isFinSta{ false };
		bool _isJyq{ false };
		bool _skip{ false };
		bool _isseapt{ false };
		bool _insert{ false };
	};
}
#endif