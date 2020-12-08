#ifndef DIALOGFILTERMESH_H
#define DIALOGFILTERMESH_H

#include <QDialog>
#include <vtkCellType.h>
#include <QList>
#include "mainWidgetsAPI.h"

class vtkDataSet;

namespace MeshData{
	class MeshKernal;
	class MeshData;
}

namespace Ui {
class FilterMeshDialog;
}

namespace GUI{
	class MainWindow;
}

namespace MainWidget{
	class PreWindow;

	class MAINWIDGETSAPI FilterMeshDialog : public QDialog
	{
		Q_OBJECT

	public:
		FilterMeshDialog(GUI::MainWindow* m, PreWindow* prewindow);
		~FilterMeshDialog();


	/*private slots:*/
	signals:
		void updateMeshTree();
		void updateSetTree();
		void updateActions();
		void updateMeshActor();

	private:
		void accept() override;
		//��ʼ��
		void init();
		//��������
		vtkDataSet* filterMesh(vtkDataSet* dataset);
		//�Ƿ�Ϊָ������
		bool isSpecifiedCell(VTKCellType type);
		//���ù�������
		void setCellFilters();
		//���ù�������
		void setFilterMeshs();

	private:
		Ui::FilterMeshDialog *_ui;

		QList<int> _cellTypeList{};

		QList<MeshData::MeshKernal*> _kernalList{};
		MeshData::MeshData* _data;
	};
}

#endif // DIALOGFILTERMESH_H
