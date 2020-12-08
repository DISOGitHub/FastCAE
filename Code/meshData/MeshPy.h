#ifndef _MESHPY_H_ 
#define _MESHPY_H_

#include "meshDataAPI.h"

extern "C"
{
	//MeshKernal
	MESHDATAAPI int getKernalCountPy();
	MESHDATAAPI const char* getKernalNameByIndexPy(const int index);
	MESHDATAAPI const char* getKernalNameByKidPy(const int kid);
	MESHDATAAPI void removeKernalByIndexPy(const int index);
	MESHDATAAPI void removeKernalByKidPy(const int kid);

	//MeshSet
	MESHDATAAPI int getSetCountPy();
	MESHDATAAPI const char* getSetNameByIndexPy(const int index);
	MESHDATAAPI const char* getSetNameBySidPy(const int sid);
	MESHDATAAPI void removeSetByIndexPy(const int index);
	
	MESHDATAAPI void clearPy();
	MESHDATAAPI void generateDisplayDataSetPy();
}

#endif