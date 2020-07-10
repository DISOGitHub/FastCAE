#ifndef _MESHTHREADBASE_H_
#define _MESHTHREADBASE_H_

#include "moduleBase/ThreadTask.h"
#include "meshDataExchangePluginAPI.h"
#include "meshDataExchangePlugin.h"

namespace GUI
{
	class MainWindow;
}

namespace MeshData
{	
	class MESHDATAEXCHANGEPLUGINAPI MeshThreadBase : public ModuleBase::ThreadTask
	{
		public:
			MeshThreadBase(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw);
			virtual ~MeshThreadBase();

			virtual void run() = 0;
			void defaultMeshFinished();
			void setReadResult(bool);
			void setWriteResult(bool);

		private:
			bool _success{ false };
			QString _fileName;
			MeshOperation _operation;
	};
}

#endif