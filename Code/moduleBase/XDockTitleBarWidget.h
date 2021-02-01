#ifndef MYDOCKTITLEBAR_H
#define MYDOCKTITLEBAR_H

#include <QSize>
#include <QDockWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>

class XDockTitleBarWidget : public QWidget
{
	Q_OBJECT
public:
	XDockTitleBarWidget(QString dockName, QWidget *parent = 0);

	QSize sizeHint() const { return minimumSizeHint(); }
	QSize minimumSizeHint() const;

	int width;
	QString name;//receive parent,s name
protected:
	//void mousePressEvent(QMouseEvent *event);

	void paintEvent(QPaintEvent *event);//paint titleBar

	void drawHat(QPainter & painter);

	void drawTitle(const QString & name, QPainter & painter);

private:
	QPixmap minPix, closePix, floatPix;

	QColor m_fromColor{ QColor(18,145,243) }, m_toColor{ QColor(18,145,243) };

	QFont m_titleFont{ QFont("Arial" , 10 , 60) };
};

#endif // MYDOCKTITLEBAR_H