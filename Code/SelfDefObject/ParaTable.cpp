#include "ParaTable.h"
#include "DataProperty/ParameterTable.h"
#include "ParaTabViewer.h"
#include <QString>

namespace SelfDefObj
{
	ParaTable::ParaTable(GUI::MainWindow* m, DataProperty::ParameterBase* data)
		: ParaMore(m, data), _mainWindow(m)
	{
		_data = (DataProperty::ParameterTable*)data;
		updateLineEdit();
	}

	void ParaTable::on_moreButton_clicked()
	{
		ParaTabViewer viewer(_mainWindow, _data);
		viewer.exec();
		updateLineEdit();
		emit dataChanged(_data);
	}

	void ParaTable::updateLineEdit()
	{
		int r = _data->getRowCount();
		int c = _data->getColumnCount();
		QString text = QString(tr("Row:%1,Col:%2").arg(r).arg(c));
		this->setText(text);
	}

}