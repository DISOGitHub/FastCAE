#ifndef MATERIALPY_H_
#define MATERIALPY_H_

#include "MaterialAPI.h"

namespace Material
{
	class MaterialPy
	{
	public:
		static void setValue(int caseID, char* variable, char* stype, char* svalue);
	};
}

//����Ϊc�ӿڣ���python�ű�����
extern "C"
{
	void MATERIALAPI setValue(int ID, char* variable, char* stype, char* svalue);

}



#endif