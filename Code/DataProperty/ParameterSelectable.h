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
		//设置可选项
		void setOption(QStringList s);
		//获取可选项
		QStringList getOption();
		//设置当前索引
		void setCurrentIndex(const int index);
		//设置当前索引
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