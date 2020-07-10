#ifndef GEOMETRYPY_H_
#define GEOMETRYPY_H_

#include "geometryAPI.h"

extern "C"
{
	//获取set数量
	int GEOMETRYAPI getGeometrySetCount();

	//几何是否为空
	bool GEOMETRYAPI isEmpty();

	//根据索引获取名称
	GEOMETRYAPI	char* getGeometrySetNameByIndex(int index);

	//根据ID获取名称
	GEOMETRYAPI	char* getGeometrySetNameByID(int id);

	//获取索引
	int GEOMETRYAPI getIndexOfGeoometrySet(char* name);
	
	//获取ID
	int GEOMETRYAPI getIDOfGeoometrySet(char* name);

	//根据Index获取ID
	int GEOMETRYAPI getIDByIndex(int index);

	//根据ID获取Index
	int GEOMETRYAPI getIndexByID(int id);

	//根据索引移除
	void GEOMETRYAPI removeGeometrySetByIndex(int index);

	//根据ID移除
	void GEOMETRYAPI removeGeometrySetByID(int id);

	//清空
	void  GEOMETRYAPI clear();



}

#endif
