#ifndef _IGESDATAEXCHANGE_H_
#define _IGESDATAEXCHANGE_H_

#include "GeometryThreadBase.h"

namespace Geometry
{
	class GEOMETRYDATAEXCHANGEAPI IGESdataExchange : public GeometryThreadBase
	{
		public:
			IGESdataExchange(const QString& _fileName, GeometryOperation operation, GUI::MainWindow *mw, QList<GeometrySet*>& res, int KernalId = -1);
			~IGESdataExchange();
			void run();

		private:
			bool readIges();
			bool writeIges();
			void deriveGeometryTaskFinished();

		private:
			const QString _fileName;
			GeometryOperation _operation;
			bool _isRead{ false };
			bool _isWrite{ false };
	};
}
#endif