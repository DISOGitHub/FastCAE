#include "PostConfig.h"

namespace ConfigOption
{
	void PostConfig::enablePost(bool on)
	{
		_enablePost = on;
	}
	bool PostConfig::isPostEnabled()
	{
		return _enablePost;
	}
	void PostConfig::enablePost2D(bool on)
	{
		_enablePost2D = on;
	}
	bool PostConfig::isPost2DEnabled()
	{
		return _enablePost2D;
	}
	void PostConfig::enablePost3D(bool on)
	{
		_enablePost3D = on;
	}
	bool PostConfig::isPost3DEnabled()
	{
		return _enablePost3D;
	}
	void PostConfig::setPostConfigInfo(ProjectTreeType type, PostConfigInfo* info)
	{
		if (_postConfigInfo.contains(type))
		{
			PostConfigInfo* f = _postConfigInfo.value(type);
			delete f;
			_postConfigInfo.remove(type);
		}
		_postConfigInfo[type] = info;
	}
	PostConfigInfo* PostConfig::getPostConfigInfo(ProjectTreeType type)
	{
		if (_postConfigInfo.contains(type))
		{
			return _postConfigInfo.value(type);
		}
		return nullptr;
	}

	void PostConfig::clearData()
	{
		_enablePost = _enablePost2D = _enablePost3D = false;
		QList<PostConfigInfo*> infos = _postConfigInfo.values();
		for (auto info : infos)
			delete info;
		_postConfigInfo.clear();
	}

}