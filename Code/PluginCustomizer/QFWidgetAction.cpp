#include "QFWidgetAction.h"
#include <QPushButton>
#include <QSplitter>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QCheckBox>

namespace FastCAEDesigner
{

	QFWidgetAction::QFWidgetAction(QWidget *parent) :QWidgetAction(parent)
	{
	}

	QFWidgetAction::~QFWidgetAction()
	{
		if (nullptr == _checkBox)
		{
			delete _checkBox;
			_checkBox = nullptr;
		}

		if (nullptr == _widget)
		{
			delete _widget;
			_widget = nullptr;
		}

		if (nullptr == _hlayout)
		{
			delete _hlayout;
			_hlayout = nullptr;
		}
	}

	//建立widget
	QWidget *QFWidgetAction::createWidget(QWidget *parent)
	{
		_checkBox = new QCheckBox(_text, parent);
		_hlayout = new QHBoxLayout;
		_hlayout->setMargin(5);
		_hlayout->setSpacing(0);
		_hlayout->addWidget(_checkBox);
		_widget = new QWidget(parent); //如果写成 QSplitter* sp = new QSplitter;  就无法显示！！！
		_widget->setLayout(_hlayout);

		connect(_checkBox, SIGNAL(stateChanged(int)), this, SLOT(OnCheckBoxStateChanged(int)));
		return _widget;
	}

	//槽函数--checkbox控件被点击
	void QFWidgetAction::OnCheckBoxStateChanged(int state)
	{
		//qDebug() << "CheckBox clicked!";
		emit signal_CheckBoxStateChanged(state);
	}

	//设置文本
	void QFWidgetAction::SetText(QString text)
	{
		this->_text = text;
	}

	//获取文本
	QString QFWidgetAction::GetText()
	{
		return _text;
	}

	void QFWidgetAction::SetChechBoxChecked(bool on)
	{
		_checkBox->setChecked(on);
	}

	bool QFWidgetAction::getCheckBoxChecked()
	{
		return _checkBox->isChecked();
	}
}