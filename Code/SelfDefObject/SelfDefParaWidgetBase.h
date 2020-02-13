#ifndef SELFDEFWIDGETBASE_H_
#define SELFDEFWIDGETBASE_H_

#include "SelfDefObjectAPI.h"
#include <QString>

namespace Py
{
	class PythonAagent;
}
namespace DataProperty
{
	class ParameterBase;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI SelfDefParaWidgetBase
	{
	public:
		SelfDefParaWidgetBase(DataProperty::ParameterBase* d);
		~SelfDefParaWidgetBase() = default;

	protected:
		void submitCode();
		virtual QString generateCode();

	private:
		void genBasicCode();

	protected:
		bool _inited{ false };

	private:
		Py::PythonAagent* _pyagent{};
		DataProperty::ParameterBase* _data{};
		QString _basicCode;

	};
}



#endif