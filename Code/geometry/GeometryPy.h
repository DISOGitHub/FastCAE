#ifndef GEOMETRYPY_H_
#define GEOMETRYPY_H_

#include "geometryAPI.h"

extern "C"
{
	//��ȡset����
	int GEOMETRYAPI getGeometrySetCount();

	//�����Ƿ�Ϊ��
	bool GEOMETRYAPI isEmpty();

	//����������ȡ����
	GEOMETRYAPI	char* getGeometrySetNameByIndex(int index);

	//����ID��ȡ����
	GEOMETRYAPI	char* getGeometrySetNameByID(int id);

	//��ȡ����
	int GEOMETRYAPI getIndexOfGeoometrySet(char* name);
	
	//��ȡID
	int GEOMETRYAPI getIDOfGeoometrySet(char* name);

	//����Index��ȡID
	int GEOMETRYAPI getIDByIndex(int index);

	//����ID��ȡIndex
	int GEOMETRYAPI getIndexByID(int id);

	//���������Ƴ�
	void GEOMETRYAPI removeGeometrySetByIndex(int index);

	//����ID�Ƴ�
	void GEOMETRYAPI removeGeometrySetByID(int id);

	//���
	void  GEOMETRYAPI clear();



}

#endif
