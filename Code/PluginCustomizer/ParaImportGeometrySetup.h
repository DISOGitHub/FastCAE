#ifndef PARAIMPORTGEOMETRYSETUP_H
#define PARAIMPORTGEOMETRYSETUP_H

#include <QDialog>

namespace Ui {
class ParaImportGeometrySetup;
}


namespace FastCAEDesigner
{
	class ParaImportGeometrySetup : public QDialog
	{
		Q_OBJECT

	public:
		explicit ParaImportGeometrySetup(bool isImport ,QWidget *parent = 0);
		~ParaImportGeometrySetup();

		QString getSuffix();

	private:
		void Init();
		void initImportAndExportSuffix(QString s);
	private slots:
		void OnBtnOkClicked();
		void OnBtnCancelClicked();
		void OnChkAllClicked();
		void CancelCheckAll();

	private:
		Ui::ParaImportGeometrySetup *ui;
		bool _isImport{ false };
		QString _suffix{};
	
	
	};
}

#endif // PARAIMPORTGEOMETRYSETUP_H
