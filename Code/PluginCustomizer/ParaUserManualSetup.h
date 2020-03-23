#ifndef PARAUSERMANUALSETUP_H
#define PARAUSERMANUALSETUP_H

#include <QDialog>
#include <QShowEvent>
#include <QResizeEvent>

#ifdef Q_OS_WIN
#include <QAxWidget>
#endif

namespace Ui {
class ParaUserManualSetup;
}

namespace ConfigOption
{
	class GlobalConfig;
}

namespace FastCAEDesigner
{
	class ParaUserManualSetup : public QDialog
	{
		Q_OBJECT

	public:
		ParaUserManualSetup(ConfigOption::GlobalConfig* globalConfig, QWidget *parent = 0);
		~ParaUserManualSetup();
		void showEvent(QShowEvent *) override;
		void resizeEvent(QResizeEvent *e) override;

	private slots:
		void OnBtnLoadDocClicked();
		void OnBtnOkClicked();
		void OnBtnCancelClicked();
		void OnBtnPreviewClicked();
	
	private:
		void OpenExcel(QString &fileName);
		void OpenWord(QString &fileName);
		void OpenPdf(QString &fileName);
		void CloseOffice();
		void Sleep(int msec);
		void UpdataDlg();
		void OpenFile(QString fileName);

	private:
		Ui::ParaUserManualSetup *ui{};
		ConfigOption::GlobalConfig* _globalConfig{};

#ifdef Q_OS_WIN

		QAxWidget* _officeContent{nullptr};

#endif
		bool _updateDlg{nullptr};
		QString _fileName;

	};
}

#endif // PARAUSERMANUALSETUP_H
