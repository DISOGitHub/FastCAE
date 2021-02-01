#include "PostCurve.h"

namespace ConfigOption
{

	void PostCurve::setDescribe(QString d)
	{
		_describe = d;
		this->appendProperty("Name",d);
	}
	QString PostCurve::getDescribe()
	{
		return _describe;
	}
	void PostCurve::setXVariable(QString x)
	{
		_xVariable = x;
		this->appendProperty("X Axis", x);
	}
	QString PostCurve::getXVariable()
	{
		return _xVariable;
	}
	void PostCurve::setYVariable(QString y)
	{
		_yVariable = y;
		this->appendProperty("Y Axis", y);
	}
	QString PostCurve::getYVariable()
	{
		return _yVariable;
	}
	void PostCurve::setFile(QString f)
	{
		_file = f;
		this->appendProperty("File", f);
	}
	QString PostCurve::getFile()
	{
		return _file;
	}
	void PostCurve::copy(DataBase* data)
	{
		PostCurve* d = dynamic_cast<PostCurve*>(data);
		if (d == nullptr) return;
		QString des = d->getDescribe();
		QString x = d->getXVariable();
		QString y = d->getYVariable();
		QString f = d->getFile();
		this->setDescribe(des);
		this->setXVariable(x);
		this->setYVariable(y);
		this->setFile(f);
	}

}