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
		EditorDependencyFiles(QString fileName, QWidget *parent = 0);
		~EditorDependencyFiles();

		QString getFiles();

	private slots:
		void OnSelectPBtnClicked();
		void OnDeletePBtnClicked();
		void OnClearAllPBtnClicked();
		void OnOkPBtnClicked();

	private:
		void init();
		void UpdateFileNames();//Added xvdongming 2020-02-14 刷新选择的依赖文件列表

	private:
		Ui::EditorDependencyFiles *ui;
		QStringList _selectedFiles{};
		QString _fileNames{};
	};
}
#endif // EDITORDEPENDENCYFILES_H
