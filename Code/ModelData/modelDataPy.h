#ifndef _MODELDATAPY_H_
#define _MODELDATAPY_H_
#include "modelDataAPI.h"

namespace ModelData
{

	class ModelDataPy
	{
	public:
		static void importComponents(int caseId, const char* addcomponentsId);
		static void importGeometry(int caseId, char* addcomponentsId);
		static void addBC(int caseId, int id, char* bctypetostring);
		static void setValue(int caseID, char* variable, char* stype, char* svalue);
		static void setBCValue(int caseID, int index, char* variable, char* stype, char* svalue);
		static void setMaterial(int caseID, char* componentIDs, int materialID);
	};
}


//声明为c接口，供python脚本调用
extern "C"
{
	void MODELDATAAPI importComponents(int caseId, const char* addcomponentsId);
	void MODELDATAAPI importGeometry(int caseId, char* addcomponentsId);
	void MODELDATAAPI addBC(int caseId, int id, char* bctypetostring);
	void MODELDATAAPI setValue(int caseID, char* variable, char* stype, char* svalue);
	void MODELDATAAPI setBCValue(int caseID,int index,char* variable, char* stype, char* svalue);
	void MODELDATAAPI setMaterial(int caseID, char* componentIDs, int materialID);
}







#endif 
