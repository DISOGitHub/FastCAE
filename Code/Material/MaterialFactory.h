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
		//ע����ϴ�������
		static void registerType(QString type, CREATEMATERIAL);
		//�Ƴ����ϴ�������
		static void removeType(QString type);
		//�������ʹ�������
		static Material* createMaterial(QString  type);
		//��ȡ��������
		static QStringList getRegTypes();
	
	private:
		static QHash<QString, CREATEMATERIAL> _typeFunHash;

	};

}




#endif