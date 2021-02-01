#include "SelfDefObjectBase.h"
#include <QHBoxLayout>

SelfDefObjBase::SelfDefObjBase(ObjType type)
	:_type(type)
{
}
bool SelfDefObjBase::check()
{
	return true;
}
void SelfDefObjBase::setName(const QString& name)
{
	_name = name;
}
QString SelfDefObjBase::getName()
{
	return _name;
}
void SelfDefObjBase::setType(ObjType type)
{
	_type = type;
}
ObjType SelfDefObjBase::getType()
{
	return _type;
}
bool SelfDefObjBase::applyCheckable()
{
	return true;
}