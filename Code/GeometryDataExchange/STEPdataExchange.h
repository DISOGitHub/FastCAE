#ifndef _STEPDATAEXCHANGE_H_
#define _STEPDATAEXCHANGE_H_

#include "GeometryThreadBase.h"

namespace Geometry
{
	class GEOMETRYDATAEXCHANGEAPI STEPdataExchange : public GeometryThreadBase
	{
		public:
			STEPdataExchange(const QString& _fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId = -1);
			~STEPdataExchange();
			void run();
		
		private:
			bool readStep();
			bool writeStep();
			void deriveGeometryTaskFinished();

		private:		
			const QString _fileName;
			GeometryOperation _operation;
			bool _isRead{ false };
			bool _isWrite{ false };
	};
}
#endif