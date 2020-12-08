#ifndef _BREFDATAEXCHANGE_H_
#define _BREFDATAEXCHANGE_H_

#include "GeometryThreadBase.h"

namespace GUI
{
	class MainWindow;
}

namespace Geometry
{
	class GeometrySet;

	class GEOMETRYDATAEXCHANGEAPI BREPdataExchange : public GeometryThreadBase
	{
		public:		
			BREPdataExchange(const QString& _fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId = -1);
			~BREPdataExchange();
			void run();

		private:
			bool readBrep();
			bool writeBrep();
			void deriveGeometryTaskFinished();

		private:
			const QString _fileName;
			GeometryOperation _operation;
			bool _isRead{ false };
			bool _isWrite{ false };
	};
}
#endif