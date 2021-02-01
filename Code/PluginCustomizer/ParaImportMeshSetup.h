#ifndef PARAIMPORTMESHSETUP_H
#define PARAIMPORTMESHSETUP_H

#include <QDialog>

namespace Ui {
class ParaImportMeshSetup;
}
namespace FastCAEDesigner{
	class ParaImportMeshSetup : public QDialog
	{
		Q_OBJECT

	public:
		explicit ParaImportMeshSetup(QWidget *parent = 0);
		~ParaImportMeshSetup();
	
		void initimportsuffix(QString s);
		QString getSuffix();

	private slots:
		void OnBtnOkClicked();
		void OnBtnCancelClicked();
		void OnChkAllClicked();
		void CancelCheckAll();

	private:
		Ui::ParaImportMeshSetup *ui;
		QString _sffix;
	};
}
#endif // PARAIMPORTMESHSETUP_H
