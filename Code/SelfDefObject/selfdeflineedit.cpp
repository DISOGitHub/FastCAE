#include "selfdeflineedit.h"
#include "ui_selfdeflineedit.h"
#include <QHelpEvent>
#include <QToolTip>
#include <QValidator>

SelfDefLineEdit::SelfDefLineEdit(QWidget *parent)
	: QWidget(parent), _ui(new Ui::SelfDefLineEdit), SelfDefObjBase(SelfDef_ValueLine)
{
	_ui->setupUi(this);
	_range[0] = -1e66;
	_range[1] = 1e66;
	connect(_ui->lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(soltCheck()));
	connect(_ui->lineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(dataChanged()));
}
SelfDefLineEdit::~SelfDefLineEdit()
{
	if (_ui != nullptr)
		delete _ui;
}
void SelfDefLineEdit::setDataType(LineEdieDataType t)
{
	_type = t;
}
void SelfDefLineEdit::setData(double data)
{
	_ui->lineEdit->setText(QString::number(data));
}
void SelfDefLineEdit::setData(int data)
{
	_ui->lineEdit->setText(QString::number(data));
}
void SelfDefLineEdit::setText(const QString &text)
{
	_ui->lineEdit->setText(text);
}
void SelfDefLineEdit::setRange(double min, double max)
{
	_range[0] = min;
	_range[1] = max;
	_checkable = true;
}
void SelfDefLineEdit::enableRangeCheck(bool on)
{
	_checkable = on;
}
int SelfDefLineEdit::getIntData()
{	
	if (_ui->lineEdit->styleSheet() == "background-color:red") return -1e10;
	QString text = _ui->lineEdit->text();
	
	if (_type == DATATYPEINT)
	{
		bool ok = false;
		int temp = text.toInt(&ok);
		if (ok) return temp;
	}
	return -1e10;
}
double SelfDefLineEdit::getDoubleData()
{
	if (_ui->lineEdit->styleSheet() == "background-color:red") return -1e66;
	QString text = _ui->lineEdit->text();
	if (_type == DATATYPEDOUBLE)
	{
		bool ok = false;
		double temp = text.toDouble(&ok);
		if (ok) return temp;
	}
	return -1e66;
}
QString SelfDefLineEdit::getText()
{
	QString text = "FFFFFFFFFFF";
	if (_type == DATATYPESTRING)
	{
		text = _ui->lineEdit->text();
		return text;
	}
	return text;
}
void SelfDefLineEdit::soltCheck()
{
	_toolTip.clear();
	if (!_checkable)
	{
		_ui->lineEdit->setStyleSheet("background-color:white");//背景色
		return;
	}
	QString text = _ui->lineEdit->text();
	bool pass = false;
	bool format = false;
	bool range = false;
	if (text.isEmpty() || _type == DATATYPESTRING) pass = true;
	QRegExp reg; int pos = 0; QRegExpValidator* validator;
	switch (_type)
	{
	case DATATYPEINT:
		reg = QRegExp("^[-?\\d+]?\\d*$");
		validator = new QRegExpValidator(reg, 0);
		pos = 0;
		if (QValidator::Acceptable == validator->validate(text, pos)) format = true;
		if (format)
		{
			bool ok = false;
			int d = text.toInt(&ok);
			if (ok && d <= _range[1] && d >= _range[0]) range = true;
		}
		if (!format) _toolTip = tr("Must Input an integer number!");
		else if (!range) _toolTip = QString(tr("The number must in range [%1,%2]")).arg(_range[0]).arg(_range[1]);
		break;
	case DATATYPEDOUBLE:
		reg = QRegExp("^-?\\d+(\\.\\d+)?$");
		validator = new QRegExpValidator(reg, 0);
		pos = 0;
		if (QValidator::Acceptable == validator->validate(text, pos)) format = true;
		if (format)
		{
			bool ok = false;
			double d = text.toDouble(&ok);
			if (ok && d <= _range[1] && d >= _range[0]) range = true;
		}
		if (format && range && _accuracy > 0 && text.contains("."))
		{
			pos = text.indexOf(".");
			pos = text.size() - 1 - pos;//小数位数
			if (pos > _accuracy)
			{
				range = false;
				_toolTip = QString(tr("The accuracy of number can not more than %1")).arg(_accuracy);
			}
		}
		if (!_toolTip.isEmpty()) break;
		if (!format) _toolTip = tr("Must Input an float number!");
		else if (!range) _toolTip = QString(tr("The number must in range [%1,%2]")).arg(_range[0]).arg(_range[1]);
		break;
	case DATATYPESTRING:
		format = range = true;
		break;
	}

	if (range && format) pass = true;

	if (!pass)
	{
		_ui->lineEdit->setStyleSheet("background-color:red");//背景色
		QHelpEvent evt(QEvent::ToolTip, this->pos(), this->pos());
		QApplication::sendEvent(this, &evt);
	}
	else
		_ui->lineEdit->setStyleSheet("background-color:white");//背景色

}
void SelfDefLineEdit::setRemindText(QString text)
{
	_ui->lineEdit->setPlaceholderText(text);
}

bool SelfDefLineEdit::check()
{
	soltCheck();
	if (_ui->lineEdit->styleSheet() == "background-color:red") return false;
	else
	{
		QString text = _ui->lineEdit->text();
		if (text.isEmpty())
		{
			_ui->lineEdit->setStyleSheet("background-color:red");//背景色
			return false;
		}
	}
	return true;
}
void SelfDefLineEdit::setAccuracy(int acc)
{
	_accuracy = acc;
}
int SelfDefLineEdit::getAccuracy()
{
	return _accuracy;
}
void SelfDefLineEdit::setApplyCheckable(bool checkable)
{
	_applyCheckable = checkable;
}
bool SelfDefLineEdit::applyCheckable()
{
	return _applyCheckable;
}
void SelfDefLineEdit::setEditable(bool editable)
{
	if (editable) _ui->lineEdit->setFocusPolicy(Qt::ClickFocus);
	else _ui->lineEdit->setFocusPolicy(Qt::NoFocus);
	
}

bool SelfDefLineEdit::event(QEvent *even)
{
	if (!_toolTip.isEmpty())
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent*>(even);
		QPoint pt2 = helpEvent->pos();
		QPoint pt = mapToGlobal(pt2);
		QPoint show_pt = pt - pt2 + QPoint(20, 20);

		QToolTip::showText(show_pt, _toolTip);

		_toolTip.clear();
	}

	return QWidget::event(even);
}