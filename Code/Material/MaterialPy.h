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

//声明为c接口，供python脚本调用
extern "C"
{
	void MATERIALAPI setValue(int ID, char* variable, char* stype, char* svalue);

}



#endif