#ifndef _AIRCRAFTPLUGIN_
#define _AIRCRAFTPLUGIN_

#include "PluginManager/pluginBase.h"
#include "AircraftPlugin.h"
#include "ModelData/modelDataBase.h"
#include "AircraftPluginAPI.h"

namespace Aircraft
{
	class AircraftControlPanel;

	class AircraftPlugin : public Plugins::PluginBase
	{
		Q_OBJECT
	public:
		AircraftPlugin(GUI::MainWindow*  m);
		~AircraftPlugin();

		static GUI::MainWindow* getMainWindow();

		//加载插件
		virtual bool install() override;
		//卸载插件
		virtual bool uninstall() override;
		//翻译
		virtual void reTranslate(QString lang) override;
		//写工程文件
		virtual void writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		// 从工程文件读取
		virtual void readFromProjectFile(QDomElement* parent) override;
		// 是否有信息需要保存
		//bool hasInfoToSave() override;

	private slots:
		//void slot_show_geant4_result();

		

	private:
		static GUI::MainWindow* _mainWindow;
		AircraftControlPanel* _gent4ControlPanel;
		QWidget* _FastCAEControlPanel{};
		//Geant4PropertyWidget * _geant4PropertyWidget{};
		/*QMenu* _menu{};
		QToolBar* _toolBar{};
	*/
	};
}

extern "C"
{

	// 注册插件接口
	void AIRCRAFTPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);

}

#endif
