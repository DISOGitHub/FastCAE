#include "Random.h"
#include "XRandomWidget.h"

class RandomPrivate
{
public:
	RandomPrivate()
	{

	}
	~RandomPrivate()
	{

	}

	int exec(const QString & name, int min, int max, int mode = Mode_ALL,const QSize & size = QSize());


	QList<double> getRandomValues() const
	{
		return m_widget.getRandomValues();
	}

	XRandomWidget m_widget;
};

int RandomPrivate::exec(const QString & name, int min, int max, int mode /*= Mode_ALL*/, const QSize & size /*= QSize()*/)
{
	m_widget.setMode(mode);
	m_widget.setRange(min, max);
	m_widget.setWindowTitle(name);
	if (size != QSize())
	{
		m_widget.setFixedSize(size);
	}
	return m_widget.exec();
}

Random::Random()
{
	m_private = new RandomPrivate();
}


Random::~Random()
{
	if (m_private)
	{
		delete m_private;
	}
	m_private = NULL;
}

int Random::exec(const QString & name, int min, int max, int mode /*= Mode_ALL*/, const QSize size /*= QSize()*/)
{
	return m_private->exec(name, min, max, mode, size);
}

QList<double> Random::getRandomValues() const
{
	return m_private->getRandomValues();
}
