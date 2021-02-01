#include "XDockTitleBarWidget.h"
XDockTitleBarWidget::XDockTitleBarWidget(QString dockName, QWidget *parent)
	: QWidget(parent)
{
// 	this->setFixedHeight(40);
// 	this->setMinimumHeight(40);
// 	this->setMaximumHeight(40);
	//pixMap
	minPix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
	closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
	floatPix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
	name = dockName;
}

//set TitleBar minimumSize
QSize XDockTitleBarWidget::minimumSizeHint() const
{
	return QSize(256, 23);
}

//绘制titleBar
void XDockTitleBarWidget::paintEvent(QPaintEvent*)
{

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

//	drawHat(painter);
	drawTitle(name, painter);
}

void XDockTitleBarWidget::drawHat(QPainter & painter)
{
	QRect rect = this->rect();
	int radius = rect.height() / 5;

	painter.setPen(QPen(QColor(m_toColor)));

	QLinearGradient linear(QPoint(rect.left() + radius, rect.top()), QPoint(rect.right() - radius, rect.top()));
	linear.setColorAt(0, m_fromColor);
	linear.setColorAt(1, m_toColor);
	painter.setBrush(linear);
	painter.drawRect(QRect(rect.left(), rect.top(), rect.width(), radius));

	painter.setPen(Qt::gray);
	
	//painter.drawLine(QPoint(rect.left(), rect.bottom()), QPoint(rect.right(), rect.bottom()));
	QPointF points[4] = { QPointF(rect.left() + radius, rect.top()), QPointF(rect.right() - radius, rect.top()), QPointF(rect.right(), rect.top() + radius), QPointF(rect.left(), rect.top() + radius) };
	painter.drawPolygon(points, 4);
}

void XDockTitleBarWidget::drawTitle(const QString & name, QPainter & painter)
{
	///<MG　color way
	QRect rect = this->rect();
	int radius = rect.height() / 5;
	QLinearGradient linear(QPoint(rect.left(), rect.top()), QPoint(rect.left(), rect.bottom()));
	linear.setColorAt(0, m_toColor);
	linear.setColorAt(1, m_fromColor);
	painter.setPen(QPen(QBrush(linear),10));

	painter.setFont(m_titleFont);
	painter.drawText(rect.left() + 10, rect.top() + radius /2, rect.width(), rect.height(), Qt::AlignVCenter, name);
	///<MG picture way
}