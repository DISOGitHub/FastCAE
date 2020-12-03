#pragma once 
#include <QObject>

namespace Geant4
{
	class GenerateMacFile : public QObject
	{
		Q_OBJECT
	public:
		GenerateMacFile();
		~GenerateMacFile();

		void write(QString file);

	private:
		void generate(QString f);
	};
}
