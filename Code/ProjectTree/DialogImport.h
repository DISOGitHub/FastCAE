#ifndef _DIALOGIMPORTMESHSET_
#define _DIALOGIMPORTMESHSET_

#include "moduleBase/componentDialogBase.h"

namespace GUI
{
	class MainWindow;
}
namespace ModelData
{
	class ModelDataBaseExtend;
}

namespace ProjectTree
{
	enum ImportType
	{
		None,
		Geometry,
		MeshSet
	};

	class ImportDialog : public ModuleBase::ComponentSelectDialogBase
	{
		Q_OBJECT
	public:
		ImportDialog(GUI::MainWindow* mainwindow, ModelData::ModelDataBaseExtend* data, ImportType t);
		~ImportDialog() = default;

	
	protected:
		void init() override;
		void accept() override;
	
	private:
		void initGeo();
		void initMeshSet();
		void initGeoComponent();

		void acceptGeo();
//		void acceptComponents();

	private:
		ImportType _type{ None };
		ModelData::ModelDataBaseExtend* _data{};
	};

}


#endif
