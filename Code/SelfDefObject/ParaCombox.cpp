#include "ParaCombox.h"
#include "DataProperty/ParameterSelectable.h"
#include "mainWindow/mainWindow.h"
#include <QStringList>
#include "settings/busAPI.h"

namespace SelfDefObj
{
	ParaCombox::ParaCombox(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		: SelfDefParaWidgetBase(data)
	{
		connect(this, SIGNAL(lockGraphFocus(bool)), m, SIGNAL(enableGraphWindowKeyBoard(bool)));
		connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged()));
		
		blockSignals(true);
		QString lang = Setting::BusAPI::instance()->getLanguage();
		_data = (DataProperty::ParameterSelectable*)data;
		QStringList option = _data->getOption();
		if (lang.toLower() == "chinese")
			option = _data->getChinese();
		const int index = _data->getCurrentIndex();

		for (int i = 0; i < option.size(); ++i)
		{
			QString s = option.at(i);
			this->addItem(s);
		}
		this->setCurrentIndex(index);
		blockSignals(false);

		_inited = true;
	}

	void ParaCombox::focusInEvent(QFocusEvent *e)
	{
		emit lockGraphFocus(false);
		QComboBox::focusInEvent(e);
	}
	void ParaCombox::focusOutEvent(QFocusEvent *e)
	{
		const int index = this->currentIndex();
		_data->setCurrentIndex(index);
		emit lockGraphFocus(true);
		QComboBox::focusOutEvent(e);
	}
	
	void ParaCombox::valueChanged()
	{
// 		const int index = this->currentIndex();
// 		_data->setCurrentIndex(index);
		submitCode();
		emit dataChanged(_data);
	}

	QString ParaCombox::generateCode()
	{
		QString v = QString::number(this->currentIndex());
		QString bc = SelfDefParaWidgetBase::generateCode();
		bc.replace("%Value%", v);
		return bc;
	}

}