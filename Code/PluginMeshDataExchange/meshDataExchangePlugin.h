#ifndef _MESHDATAEXCHANGEPLUGIN_H_
#define _MESHDATAEXCHANGEPLUGIN_H_

#include "meshDataExchangePluginAPI.h"
#include "PluginManager/pluginBase.h"

namespace MeshData
{
	enum MeshOperation
	{
		MESH_NONE,
		MESH_READ,
		MESH_WRITE
	};

	class MESHDATAEXCHANGEPLUGINAPI MeshDataExchangePlugin : public Plugins::PluginBase
	{
		public:
			MeshDataExchangePlugin(GUI::MainWindow* m);
			~MeshDataExchangePlugin() = default;

			bool install();
			bool uninstall();
			void reTranslate(QString);			
			static GUI::MainWindow* getMWpt();

		private:
			static GUI::MainWindow* _mainwindow;
	};
}

extern "C"
{
	void MESHDATAEXCHANGEPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
	//函数返回值是无效的,不要通过返回值判断
	bool MESHDATAEXCHANGEPLUGINAPI CGNSimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI MSHimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI NEUimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI VTK_DAT_STL_importMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI INPimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI CNTMimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI SU2importMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI AnotherSU2importMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI KEYimportMesh(QString AbFileName, int modelId);
	bool MESHDATAEXCHANGEPLUGINAPI PDBimportMesh(QString AbFileName, int modelId);

	bool MESHDATAEXCHANGEPLUGINAPI CGNSexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI MSHexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI NEUexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI VTK_DAT_STL_exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI INPexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI CNTMexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI SU2exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI AnotherSU2exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI KEYexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI PDBexportMesh(QString AbFileName, int id);
}

#endif