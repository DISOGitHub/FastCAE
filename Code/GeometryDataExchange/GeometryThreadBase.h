#ifndef _GEOMETRYTHREADBASE_H_
#define _GEOMETRYTHREADBASE_H_

#include "moduleBase/ThreadTask.h"
#include "GeometryDataExchangeAPI.h"
#include "moduleBase/messageWindowBase.h"

namespace Geometry
{
	enum GeometryOperation
	{
		GEOMETRY_NONE,
		GEOMETRY_READ,
		GEOMETRY_WRITE
	};

	class GeometrySet;

	class GEOMETRYDATAEXCHANGEAPI GeometryThreadBase : public ModuleBase::ThreadTask
	{
		Q_OBJECT

		public:
			GeometryThreadBase(GUI::MainWindow *mw, QList<Geometry::GeometrySet*>& res);
			virtual ~GeometryThreadBase();
			void defaultGeometryFinished();
			QList<GeometrySet*>& GeometryThreadBase::getResult();

		signals:
			void updateGeometryTree();
			void updateActionsStates();
			void ShowSetSig(Geometry::GeometrySet* s, bool r = true);
			void showGeometryMessageSig(ModuleBase::Message);
			void updatePreGeometryActor();

		private:
			QList<Geometry::GeometrySet*>& _result;
	};
}

#endif 