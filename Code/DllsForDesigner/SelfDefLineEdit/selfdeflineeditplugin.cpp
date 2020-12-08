#include "selfdeflineedit.h"
#include "selfdeflineeditplugin.h"

#include <QtPlugin>

SelfDefLineEditPlugin::SelfDefLineEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void SelfDefLineEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool SelfDefLineEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SelfDefLineEditPlugin::createWidget(QWidget *parent)
{
    return new SelfDefLineEdit(parent);
}

QString SelfDefLineEditPlugin::name() const
{
    return QLatin1String("SelfDefLineEdit");
}

QString SelfDefLineEditPlugin::group() const
{
    return QLatin1String("");
}

QIcon SelfDefLineEditPlugin::icon() const
{
    return QIcon();
}

QString SelfDefLineEditPlugin::toolTip() const
{
    return QLatin1String("");
}

QString SelfDefLineEditPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool SelfDefLineEditPlugin::isContainer() const
{
    return false;
}

QString SelfDefLineEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SelfDefLineEdit\" name=\"selfDefLineEdit\">\n</widget>\n");
}

QString SelfDefLineEditPlugin::includeFile() const
{
    return QLatin1String("selfdeflineedit.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(selfdeflineeditplugin, SelfDefLineEditPlugin)
#endif // QT_VERSION < 0x050000
