#include "ModelPoint.h"

namespace PluginShip
{

	ModelPoint::ModelPoint()
	{
		_point = *new gp_Pnt;
	}
	ModelPoint::~ModelPoint()
	{
	}

	gp_Vec ModelPoint::getVec()
	{
		return _vec;
	}

	void ModelPoint::setVec(gp_Vec vec)
	{
		_vec = vec;
	}

	void ModelPoint::setIndex(int i)
	{
		_index = i;
	}

	int ModelPoint::getIndex()
	{
		return _index;
	}

	void ModelPoint::setPoint(gp_Pnt pnt)
	{
		_point = pnt;
	}

	gp_Pnt ModelPoint::getPoint()
	{
		return _point;
	}

	void ModelPoint::setDistance(double d)
	{
		_distance = d;
	}

	double ModelPoint::getDistance()
	{
		return _distance;
	}

	void ModelPoint::setCurvature(double c)
	{
		_curvature = c;
	}

	double ModelPoint::getCurvature()
	{
		return _curvature;
	}

	void ModelPoint::setSave(bool s)
	{
		_save = s;
	}

	bool ModelPoint::getSave()
	{
		return _save;
	}

	void ModelPoint::setVariance(double s)
	{
		_variance = s;
	}

	double ModelPoint::getVariance()
	{
		return _variance;
	}

	void ModelPoint::isFinStabilizer(bool b)
	{
		_isFinSta = b;
	}

	bool ModelPoint::getFinStabilizer()
	{
		return _isFinSta;
	}

	void ModelPoint::setIsJyq(bool s)
	{
		_isJyq = s;
	}

	bool ModelPoint::getIsJyq()
	{
		return _isJyq;
	}

	void ModelPoint::setSkip(bool s)
	{
		_skip = s;
	}
	bool ModelPoint::getSkip()
	{
		return _skip;
	}

	void ModelPoint::setIsSeaPt(bool s)
	{
		_isseapt = s;
	}

	bool ModelPoint::getIsSeaPt()
	{
		return _isseapt;
	}

	void ModelPoint::setInset(bool s)
	{
		_insert = s;
	}

	bool ModelPoint::getInsert()
	{
		return _insert;
	}

}