#ifndef PARAEXPORTMESHSETUP_H
#define PARAEXPORTMESHSETUP_H

#include <QDialog>

namespace Ui {
class ParaExportMeshSetup;
}
namespace FastCAEDesigner{
	class ParaExportMeshSetup : public QDialog
	{
		Q_OBJECT

	public:
		explicit ParaExportMeshSetup(QWidget *parent = 0);
		~ParaExportMeshSetup();

		void initExportSuffix(QString s);
		QString getExportSuffix();

	private slots:
		void OnBtnOkClicked();
		void OnBtnCancelClicked();
		void OnChkAllClicked();
		void CancelCheckAll();

	private:
		Ui::ParaExportMeshSetup *ui;
		
		QString _suffix{};
	};
}
#endif // PARAEXPORTMESHSETUP_H
