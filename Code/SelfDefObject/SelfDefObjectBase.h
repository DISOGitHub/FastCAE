/*******************
自定义控件基类
所有的自定义控件都需要继承该类
libaojun
******************/
#ifndef SELFDEFOBJBASE_H 
#define SELFDEFOBJBASE_H

#include "SelfDefObjectAPI.h"
#include <QString>


enum  ObjType
{
	SelfDef_None = 0,
	SelfDef_ValueLine,
};

class SELFDEFINEOBJAPI SelfDefObjBase
{
public:
	SelfDefObjBase(ObjType t = SelfDef_None);
	~SelfDefObjBase() = default;

	virtual bool check();
	virtual bool applyCheckable();

	void setType(ObjType type);
	ObjType getType();

	void setName(const QString& name);
	QString getName();
	
protected:
	ObjType _type{ SelfDef_None };
	QString _name{};

};




#endif