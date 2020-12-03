#ifndef DIALOGDEFINE_H_
#define DIALOGDEFINE_H_

#include "Geant4PluginAPI.h"
#include "PreDefineBase.h"
#include <QDialog>

class QTableWidget;

namespace Ui
{
	class dialogDefine;
}

namespace Geant4{
	class DefineManager;
	class Value3D;
	class PreDefineBase;

	class  CreateDefineDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateDefineDialog(PreDefineBase* pd);
		~CreateDefineDialog()=default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

		void setTitle();
		void displayInterface();
		void displayPreDefineData();
		void updataPreDefineData();

	private:
		Ui::dialogDefine* _ui{};
		PreDefineBase* m_defineData;
		DataType m_type;
	};
}



#endif