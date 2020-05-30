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
	//��������ֵ����Ч��,��Ҫͨ������ֵ�ж�
	bool MESHDATAEXCHANGEPLUGINAPI CGNSimportMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI MSHimportMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI NEUimportMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI VTK_DAT_STL_importMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI INPimportMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI CNTMimportMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI SU2importMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI AnotherSU2importMesh(QString AbFileName);
	bool MESHDATAEXCHANGEPLUGINAPI KEYimportMesh(QString AbFileName);

	bool MESHDATAEXCHANGEPLUGINAPI CGNSexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI MSHexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI NEUexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI VTK_DAT_STL_exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI INPexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI CNTMexportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI SU2exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI AnotherSU2exportMesh(QString AbFileName, int id);
	bool MESHDATAEXCHANGEPLUGINAPI KEYexportMesh(QString AbFileName, int id);
}

#endif