#ifndef GEOCOMMANDCREATECOMPONENT_H
#define GEOCOMMANDCREATECOMPONENT_H

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include "geometry/GeoComponent.h"

namespace Geometry
{
	class GeoComponent;
	class GeometryData;
}

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandCreateComponent : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandCreateComponent(GUI::MainWindow* m, MainWidget::PreWindow* p);
	
		virtual bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setName(const QString& name);
		void setType(const QString& type);
		void setMembers(const QStringList& geoIds, const QStringList& itemIds);

	signals:
		void clearAllHighLight();

	private:
		QString _name{};
		Geometry::GeoComponentType _type{};
		QStringList _geoIds{};
		QStringList _itemIds{};
		Geometry::GeoComponent* _gc{};
		Geometry::GeometryData* _geoData{};
	};
}

#endif