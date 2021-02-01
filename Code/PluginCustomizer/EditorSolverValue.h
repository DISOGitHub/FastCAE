#ifndef EditorSolverValue_H
#define EditorSolverValue_H

#include <QDialog>
#include <QHash>
#include <QString>

namespace Ui
{
	class EditorSolverValue;
}

namespace GUI
{
	class MainWindow;
}

namespace ConfigOption
{
	class SolverInfo;
	class SolverOption;
}

namespace FastCAEDesigner
{
	class EditorSolverManager;

	class EditorSolverValue :public QDialog
	{
		Q_OBJECT
	public:
		EditorSolverValue(GUI::MainWindow* mainwin, EditorSolverManager* parent, ConfigOption::SolverInfo* solver);
		~EditorSolverValue();


	private:
		void init();
		void initCombox();
		void initDependFiles(bool visable = false);

	private slots:
		void on_out_OkButton_clicked();
		void on_out_CancelButton_clicked();
		void on_fileSelect_clicked();
		void on_fileSelect_1_clicked();
		void OnDepenFileSelectClicked();
		void OnRadioBtnClicked();

	private:
		Ui::EditorSolverValue* _ui{};
		EditorSolverManager* _parent{};

		ConfigOption::SolverOption* _solverOption{};
		ConfigOption::SolverInfo*  _modifySolver{};
		bool _isEdit{ false };

		QHash<QString, QString> _inpTextData{};
		QHash<QString, QString> _transferTextData{};

		//QString _dependFiles{};
		
	};
}


#endif
