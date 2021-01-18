#ifndef PLUGINCUSTOMIZER_H_
#define PLUGINCUSTOMIZER_H_

#include "PluginManager/pluginBase.h"
#include "pluginCustomizerAPI.h"



namespace Plugins
{
	class CustomActionManager;

	class CUSTOMIZERPLUGINAPI CustomizerPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		CustomizerPlugin(GUI::MainWindow* m);
		~CustomizerPlugin();

		bool install() override;

		bool uninstall() override;

		void isUsing(bool u);

		void reTranslate(QString lang);

	private:
		void readINI(QSettings* settings) override;
		void writeINI(QSettings* settings) override;

	private:
		GUI::MainWindow* _mainWindow{};
		CustomActionManager* _actionManager{};

		bool _isUsing{ false };

	};
}

extern "C"
{
	void CUSTOMIZERPLUGINAPI Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}


#endif
