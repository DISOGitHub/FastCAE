#ifndef DIALOGCONFIGWIND_H
#define DIALOGCONFIGWIND_H

#include <QWidget>
#include "HullCutPlugin.h"
#include "GeometryWidgets/geoDialogBase.h"
#include "SelfDefObject/selfdeflineedit.h"
#include "qtablewidget.h"

namespace Ui
{
    class ConfigWindDialog;
}
namespace GUI
{
    class MainWindow;
}

namespace PluginShip
{
    class HULLPLUGINAPI ConfigWindDialog : public QWidget
    {
        Q_OBJECT
    
    public:
        explicit ConfigWindDialog(GUI::MainWindow* m);
        ~ConfigWindDialog();

	private slots:
		void OnTableFreqItemClicked(QModelIndex modelIndex);

    private:
		Ui::ConfigWindDialog *_ui{};
    };
}
#endif // DIALOGCONFIGWIND_H
