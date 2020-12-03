#ifndef _PREWINDOWMANAGER_H_
#define _PREWINDOWMANAGER_H_

#include <QObject>
#include <QMultiHash>

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
	class MeshViewProvider;
}

namespace MeshData
{
	class MeshKernal;
}

class vtkDataSet;

namespace Geant4
{
	class PreWindowManager : public QObject
	{
		Q_OBJECT
	public :
		PreWindowManager(GUI::MainWindow* mw);
		~PreWindowManager() = default;

		//��Ӽ��� type 1-���� 2 -���� 3-����
		int addTrack(int type, QString file);
		int addTrack(int type, vtkDataSet* dataset);
		bool removeTrack(int id);

	private slots :
		void preWindowOpened(MainWidget::PreWindow* p);
		void preWindowClosed();
//		void updataTrackProperty();

	private:
		GUI::MainWindow* _mainWindow{};
		MainWidget::PreWindow* _preWindow{};
		MainWidget::MeshViewProvider* _viewProvider{};

		QMultiHash<int,MeshData::MeshKernal*> _typeKernalHash{};
	};


}


#endif
