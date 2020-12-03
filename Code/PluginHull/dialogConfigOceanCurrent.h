#ifndef DIALOGCONFIGOCEANCURRENT_H
#define DIALOGCONFIGOCEANCURRENT_H

#include <QWidget>
#include "HullCutPlugin.h"
#include "GeometryWidgets/geoDialogBase.h"

namespace Ui
{
    class ConfigOceanCurrentDialog;
}
namespace GUI
{
    class MainWindow;
}

namespace PluginShip
{
    class HULLPLUGINAPI ConfigOceanCurrentDialog : public QWidget
    {
        Q_OBJECT

    public:
        explicit ConfigOceanCurrentDialog(GUI::MainWindow *m);
        ~ConfigOceanCurrentDialog();

    private:
		Ui::ConfigOceanCurrentDialog *_ui{};
    };
}

#endif // DIALOGCONFIGOCEANCURRENT_H
