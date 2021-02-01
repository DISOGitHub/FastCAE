#ifndef GENERATEINSTALLATIONPACKAGE_H
#define GENERATEINSTALLATIONPACKAGE_H

#include <QDialog>
#include <QProcess>
#include <QDir>

namespace Ui {
class Generateinstallationpackage;
}

namespace FastCAEDesigner{
	class Generateinstallationpackage : public QDialog
	{
		Q_OBJECT

	public:
//		explicit Generateinstallationpackage(QWidget *parent = 0);
		Generateinstallationpackage(QString name, QString version, QWidget *parent = 0);
		~Generateinstallationpackage();

	private:
		void init();
		void run();
		void toConfigXML(const QString & filePath);
		void toConfigComponent(const QString & filePath);
		void toConfigQS(const QString & filePath);
		void toSoftComponent(const QString & filePath);
		void toSoftQS(const QString & filePath);
		bool copyGUIDirectory(const QDir & from, const QDir & to, bool cover);
		bool copyDirectory(const QDir & from, const QDir & to, bool cover);
		void displayInstallInfo(QString info);
		void deleteDir(const QString &path);
	

	private slots:
		void slot_console_finished(int code, QProcess::ExitStatus status);
		void slot_console_output();
		void slot_console_state_changed(QProcess::ProcessState state);
		void slot_console_process_error(QProcess::ProcessError);
		void OnPathPBtnClicked();
		void OnGeneratePBtnClicked();
		void OnTimeout();
		void OnTimeOver();
		
	private:
		QProcess* _process{};
		bool bGenerated{ false };
		QString _installPath;
		QString _softName;
		QString _version;

	private:
		Ui::Generateinstallationpackage *ui;
	};
}
#endif // GENERATEINSTALLATIONPACKAGE_H
