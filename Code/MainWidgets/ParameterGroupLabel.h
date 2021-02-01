#ifndef _PARAMETERGROUPLABEL_H_
#define _PARAMETERGROUPLABEL_H_

#include <QWidget>
#include <QString>

namespace Ui
{
	class ParameterGroupLabel;
}

namespace MainWidget
{
	class ParameterGroupLabel : public QWidget
	{
	public:
		ParameterGroupLabel();
		~ParameterGroupLabel();

		void setIcon(QString icon);
		void setText(QString text);
		void clicked();
		int getClickCount();
		bool isExtend();

	protected:
	private:
		Ui::ParameterGroupLabel* _ui{};
		int _clickCount{ 0 };

	};
}


#endif
