#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "MaterialAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>

class QDomElement;
class QDomDocument;

namespace Material
{
	class MATERIALAPI Material : public DataProperty::DataBase
	{
	public:
		Material(bool IDplus = true);
		~Material() = default;
		//��ȡ���ID
		static int getMaxID();
		void enableID(bool on);
		//��ȡ����
		QString getType();
		//��������
		void setType(QString type);
		//��������
		void copy(DataBase* data) override;
		//�ӹ����ļ�����
		void readDataFromProjectFile(QDomElement* e) override;
		//д���������ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		
	private:
		QString _type{};
		bool _needID{true};

	private:
		static int maxID;
	};
}



#endif
