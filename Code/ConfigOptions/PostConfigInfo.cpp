#include "PostConfigInfo.h"
#include "PostCurve.h"

namespace ConfigOption
{
// 	void PostConfigInfo::setPost2DFile(QStringList name)
// 	{
// 		_post2DFile = name;
// 	}
	QStringList PostConfigInfo::getPost2DFile()
	{
		QStringList s;
		for (int i = 0; i < _curves.size(); ++i)
		{
			PostCurve* c = _curves.at(i);
			QString f = c->getFile();
			if (!s.contains(f))
				s.append(f);
		}
		return s;
	}
	QStringList PostConfigInfo::get2DVariables(QString f)
	{
		QStringList v;
		for (int i = 0; i < _curves.size(); ++i)
		{
			PostCurve* c = _curves.at(i);
			QString file = c->getFile();
			if (!f.endsWith(file)) continue;
			QString x = c->getXVariable();
			QString y = c->getYVariable();
			if (!v.contains(x)) v.append(x);
			if (!v.contains(y)) v.append(y);
		}
		return v;

	}

	void PostConfigInfo::setPost3DFile(QString f)
	{
		_post3DFile = f;
	}
	QString PostConfigInfo::getPost3DFile()
	{
		return _post3DFile;
	}
// 	void PostConfigInfo::set2DVariable(QStringList v)
// 	{
// 		_post2DVariable = v;
// 	}
// 	QStringList PostConfigInfo::get2DVariable()
// 	{
// 		return _post2DVariable;
// 	}
	void PostConfigInfo::appendPostCurve(PostCurve* c)
	{
		_curves.append(c);
	}
	QList<PostCurve*> PostConfigInfo::getCurveList(/*QString f*/)
	{
		return _curves;
	}
	void PostConfigInfo::setNodeScalarVariable(QStringList sca)
	{
		_postNodeScalarVariable = sca;
	}
	QStringList PostConfigInfo::getNodeScalarVariable()
	{
		return _postNodeScalarVariable;
	}
	void PostConfigInfo::setNodeVectorVariable(QStringList s)
	{
		_postNodeVectorVariable = s;
	}
	QStringList PostConfigInfo::getNodeVectorVariable()
	{
		return _postNodeVectorVariable;
	}
	void PostConfigInfo::setCellScalarVariable(QStringList sca)
	{
		_postCellScalarVariable = sca;
	}
	QStringList PostConfigInfo::getCellScalarVariable()
	{
		return _postCellScalarVariable;
	}
	void PostConfigInfo::setCellVectorVariable(QStringList s)
	{
		_postCellVectorVariable = s;
	}
	QStringList PostConfigInfo::getCellVectorVariable()
	{
		return _postCellVectorVariable;
	}
	PostCurve* PostConfigInfo::getCurveByName(QString name)
	{
		for (int i = 0; i < _curves.size(); ++i)
		{
			PostCurve* curve = _curves.at(i);
			QString n = curve->getDescribe();
			if (name == n) return curve;
		}
		return nullptr;
	}
}