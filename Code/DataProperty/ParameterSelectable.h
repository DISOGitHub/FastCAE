#ifndef _SELECTABLEPARA_H_
#define _SELECTABLEPARA_H_

#include "ParameterBase.h"
#include <QStringList>

namespace DataProperty
{
	class DATAPROPERTYAPI ParameterSelectable : public ParameterBase
	{
	public:
		ParameterSelectable();
		~ParameterSelectable() = default;

		void copy(ParameterBase* ori, bool valueOnly= false) override;
		//���ÿ�ѡ��
		void setOption(QStringList s);
		//��ȡ��ѡ��
		QStringList getOption();
		//���õ�ǰ����
		void setCurrentIndex(const int index);
		//���õ�ǰ����
		int getCurrentIndex();
		QStringList getChinese();
		bool isSameValueWith(ParameterBase* p) override;

		void writeParameter(QDomDocument* doc, QDomElement* parent);
		void readParameter(QDomElement* e);
		
		QString valueToString() override;
		void setValueFromString(QString v) override;

	private:
		QStringList _option{};
		QStringList _chinese{};
		int _currentIndex{ 0 };
	};
}

#endif