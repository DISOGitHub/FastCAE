#ifndef DIALOGTUBE_H
#define DIALOGTUBE_H

#include <QDialog>

namespace Ui {
class DialogAddAnalysis;
}

namespace Geant4 {

	class DialogAddAnalysis : public QDialog
	{
		Q_OBJECT

	public:
		DialogAddAnalysis(QWidget * parent = nullptr);
		~DialogAddAnalysis();

		void renameWindow(QString name);
		QString getAnalysisName() const;
		int getAnalysisType() const;

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogAddAnalysis *m_ui;
	};
}
#endif // DIALOGTUBE_H
