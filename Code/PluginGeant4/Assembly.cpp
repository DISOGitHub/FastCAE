#include <Assembly.h>
#include <QDomElement>
#include "Physvol.h"
#include <QDebug>
namespace Geant4
{
	
	void Assembly::readGDML(QDomElement& e)
	{
		_name = e.attribute("name");
		qDebug() << _name;
		QDomNodeList phylist = e.elementsByTagName("physvol");
		const int n = phylist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement e = phylist.at(i).toElement();
			Physvol* p = new Physvol;
			p->readGDML(e);
			_physvols.append(p);
		}
	}

	QString Assembly::getName()
	{
		return _name;
	}

}