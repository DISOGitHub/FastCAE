#include "ParameterGroupLabel.h"
#include "ui_ParameterGroupLabel.h"

namespace MainWidget
{
	ParameterGroupLabel::ParameterGroupLabel()
	{
		_ui = new Ui::ParameterGroupLabel;
		_ui->setupUi(this);
	}
	ParameterGroupLabel::~ParameterGroupLabel()
	{
		if (_ui != nullptr) delete _ui;
	}

	void ParameterGroupLabel::setIcon(QString sicon)
	{
		_ui->icon->setPixmap(QPixmap(sicon));
	}
	void ParameterGroupLabel::setText(QString stext)
	{
		_ui->text->setText(stext);
	}

	void ParameterGroupLabel::clicked()
	{
		_clickCount++;
		QString icon = "://QUI/icon/normalL1.png";
		bool isextend = this->isExtend();
		if (isextend)
			icon = ":/QUI/icon/expandL1.png";
		this->setIcon(icon);

	}

	int ParameterGroupLabel::getClickCount()
	{
		return _clickCount;
	}

	bool ParameterGroupLabel::isExtend()
	{
		return (_clickCount % 2 == 0);
	}



}