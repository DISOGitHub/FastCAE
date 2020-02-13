#ifndef GEOCREATEBOXCOMMAND_H_
#define GEOCREATEBOXCOMMAND_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandCreateBox : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandCreateBox(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandCreateBox() = default;
		
		void setName(QString name);
		void setLocation(double* loca);
		void setGeoPara(double* para);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		QString _name{};
		double _loaction[3];
		double _geoPara[3];

		Geometry::GeometrySet* _res{};
	};
}

#endif