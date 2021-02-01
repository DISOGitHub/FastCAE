#ifndef PARABASICSETUP_H
#define PARABASICSETUP_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class QMainWindow;
//xuxinwei 20200305
namespace GUI{
	class SubWindowManager;
}


namespace ConfigOption
{
	class GlobalConfig;
}

namespace Ui
{
	class ParaBasicSetup;
}

namespace FastCAEDesigner
{
	class ParaBasicSetup : public QDialog
	{
		Q_OBJECT

	public:
		ParaBasicSetup(ConfigOption::GlobalConfig* globalConfig, QMainWindow* mw);
		ParaBasicSetup(ConfigOption::GlobalConfig* globalConfig, QMainWindow* mw, GUI::SubWindowManager* sub);
		~ParaBasicSetup();

	private:
		Ui::ParaBasicSetup *ui;
	
	public:
		QString GetLogoFileName();
		void SetLogoFileName(QString fileName);
		QString GetWelcomeFileName();
		void SetWelcomeFileName(QString fileName);

		virtual void showEvent(QShowEvent *);

	private:
		enum InputErrorCode
		{
			ChnNameIsEmpty = 1,
			EngNameIsEmpty,
			EmailNameInvalid,
			WebsiteNameInvalid,
			ChnNameIsError,
			EngNameIsError
		};

		void Init();
		void InitErrorList();
		void UpdateDataToUi();
		void UpdateUiToData();
		void ShowImage(QLabel *widget, QString fileName);
		int IsDataOk();
		void SetControlValidStyle(QLineEdit *edit, bool b);

	private slots:
		void OnBtnOkClicked();
		void OnBtnCancelClicked();
		void OnBtnOpenLogoClicked();
		void OnBtnOpenWelcomeClicked();
		void OnTxtEmailTextChanged(QString text);
		void OnTxtWebSiteTextChanged(QString text);
		void OnTimeout();


	private:
		ConfigOption::GlobalConfig* _globalConfig{};
		QString _logoFileName{};
		QString _welcomeFileName{};
		bool _emailIsValid{true};
		bool _websiteisValid{true};
		QHash<int, QString> _errorList{};
		QString _styleOk{};
		QString _styleError{};

		QMainWindow* _mainWindow{};

		GUI::SubWindowManager* _subWindow{};
		//20200324
		//QList<QString> _iconNameList{};
	};
}
#endif // PARABASICSETUP_H
