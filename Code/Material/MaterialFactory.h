#ifndef MATERIALFACTORY_H__
#define MATERIALFACTORY_H__

#include <QStringList>
#include <QHash>
#include "MaterialAPI.h"

namespace Material
{
	class Material;

	typedef Material*(*CREATEMATERIAL)(QString);

	class MATERIALAPI MaterialFactory
	{
	public:
		MaterialFactory() = default;
		~MaterialFactory() = default;
		//注册材料创建方法
		static void registerType(QString type, CREATEMATERIAL);
		//移除材料创建方法
		static void removeType(QString type);
		//根据类型创建材料
		static Material* createMaterial(QString  type);
		//获取所有类型
		static QStringList getRegTypes();
	
	private:
		static QHash<QString, CREATEMATERIAL> _typeFunHash;

	};

}




#endif