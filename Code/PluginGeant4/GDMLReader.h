#ifndef GDMLREADER_H_
#define GDMLREADER_H_

#include <QString>
#include "QDomDocument"

class QDomNodeList;
class QDomNode;

namespace Geant4{
	class Value3D;

	class GDMLReader
	{
	public:
		GDMLReader(QString filename);
		~GDMLReader() = default;

		bool read();
		bool write();


	private:
		QString _filename{};
	};
}

#endif
