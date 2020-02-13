#ifndef EDITORDEPENDENCYFILES_H
#define EDITORDEPENDENCYFILES_H

#include <QDialog>

namespace Ui {
class EditorDependencyFiles;
}

namespace FastCAEDesigner{
	class EditorDependencyFiles : public QDialog
	{
		Q_OBJECT

	public:
		explicit EditorDependencyFiles(QWidget *parent = 0);
		EditorDependencyFiles(QString filename, QWidget *parent = 0);
		~EditorDependencyFiles();

		QString getFiles();

	private slots:
		void OnSelectPBtnClicked();
		void OnDeletePBtnClicked();
		void OnClearAllPBtnClicked();
		void OnOkPBtnClicked();

	private:
		void init();

	private:
		Ui::EditorDependencyFiles *ui;
		QStringList _selectedFiles{};
		QString _fileNames{};
	};
}
#endif // EDITORDEPENDENCYFILES_H
