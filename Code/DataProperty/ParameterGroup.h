#ifndef _PARAMETERGROUP_H_
#define _PARAMETERGROUP_H_

#include "ParameterList.h"
#include "ParameterBase.h"
#include <QDataStream>
#include <QString>

namespace DataProperty
{
	class ParameterBase;
	class DataBase;

	class DATAPROPERTYAPI ParameterGroup : public ParameterList
	{
	public:
		ParameterGroup() = default;
		~ParameterGroup() = default;
		//����data�е�����
		void copy(ParameterGroup* data);

		void appendParameter(ParameterBase* p) override;
		ParameterBase* appendParameter(ParaType type) override;
		//���ò����������
		void setDescribe(QString des);
		//��ȡ����������
		QString getDescribe();
		//���ÿɼ���
		void setVisible(bool v);
		//��ȡ�ɼ�״̬
		bool isVisible();
		//����g��״̬
		void copyStates(ParameterGroup* g);

		void writeParameters(QDomDocument* doc, QDomElement* parent) override;
		void readParameters(QDomElement* ele) override;

		virtual void dataToStream(QDataStream* datas) override;

	private:
		QString _describe{};
		bool _visible{ true };
	};


}


#endif