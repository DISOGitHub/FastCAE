#ifndef MESHERPY_H_
#define MESHERPY_H_
#include "ConfigOptionsAPI.h"

namespace ConfigOption
{
	class MesherPy
	{
	public:
		void static setValue(int index, char* name, char* stype, char* svalue);

	protected:
	private:
	};



}

extern"C"
{
	void CONFIGOPTIONSAPI setValue(int index, char* name, char* stype, char* svalue);
}

#endif