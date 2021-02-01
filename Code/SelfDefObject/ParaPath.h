#ifndef _PARAPATH_H_
#define _PARAPATH_H_

#include "ParaMore.h"
#include "SelfDefParaWidgetBase.h"
#include <QString>

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class ParameterBase;
	class ParameterPath;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI ParaPath : public ParaMore, public SelfDefParaWidgetBase
	{
		Q_OBJECT

	public:
		ParaPath(GUI::MainWindow * m, DataProperty::ParameterBase* p);
		~ParaPath() = default;

	signals:
		void dataChanged(DataProperty::ParameterBase* p);

	protected slots:
		void on_moreButton_clicked() override;

	protected:
		void updateLineEdit() override;
		
	private:
		QString generateCode() override;

	private:
		DataProperty::ParameterPath* _data{};
		QString _file{};
		QStringList _fileList{};
		

	};


}



#endif