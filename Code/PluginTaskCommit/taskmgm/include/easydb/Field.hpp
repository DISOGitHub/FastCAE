#if !defined(_FIELD_HPP)
#define _FIELD_HPP

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strdup  _strdup
#endif

class Field
{
public:
    Field()
    {
        mValue	=	NULL;
    }
    ~Field()
    {
        if(mValue)
        {
            ::free(mValue);
        }
    }

    void SetValue(char* value) 
    {
        if(mValue)
        {
            ::free(mValue);
        }
        if(value)
        {
            mValue = strdup(value);	
        }
        else
        {
            mValue = NULL;
        }
    }

	///inline void SetValue(char* value) { mValue = value; }

	inline const char *GetString() { return mValue; }
	inline float GetFloat() { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	inline bool GetBool() { return mValue ? atoi(mValue) > 0 : false; }
	inline uint8 GetUInt8() { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
	inline int8 GetInt8() { return mValue ? static_cast<int8>(atol(mValue)) : 0; }
	inline uint16 GetUInt16() { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
	inline uint32 GetUInt32() { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
	inline uint32 GetInt32() { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
	uint64 GetUInt64() 
	{
		if(mValue)
		{
			uint64 value;
			#ifndef WIN32	// Make GCC happy.
			sscanf(mValue,I64FMTD,(long long unsigned int*)&value);
			#else
			sscanf_s(mValue, I64FMTD, &value);
			#endif
			return value;
		}
		else
			return 0;
	}

private:
		char *mValue;
};

#endif
