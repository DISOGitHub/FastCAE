#ifndef _GRAPHWINDOWBASE_H_
#define _GRAPHWINDOWBASE_H_

#include "moduleBaseAPI.h"
#include "moduleBase/ModuleBase.h"
#include "moduleBase/ModuleType.h"
#include <QWidget>
#include "python/PyInterpreter.h"

class QMdiSubWindow;

namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	class MODULEBASEAPI GraphWindowBase: public QWidget, public CModuleBase
	{
		Q_OBJECT
	public:
		GraphWindowBase(GUI::MainWindow* mainwindow ,int proID,GraphWindowType type = GraphWindowType::UnDefined, bool connectToMainwindow = false);
		virtual ~GraphWindowBase() = 0;
		//��ȡ����
		GraphWindowType getGraphWindowType();
		//����ͼƬ
		virtual void saveImage(QString fileName, int width, int heigh, bool showDlg);
		//��ȡID
		int getID();
		//�����ӽ�
		virtual void setView(QString view);
		virtual void setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//;liu
		//virtual void setViewPreValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//;liu
		//���ñ�����ɫ
		virtual void setBackGround(QColor color1, QColor color2);
		//�ַ�����ʽ��ȡ��������
		QString getStringGraphWindowType();

	signals:
		void showGraphRange(double, double);

	private:
		void connectSignals();
		virtual void fitView();
		virtual void setViewXPlus();
		virtual void setViewXMiuns();
		virtual void setViewYPlus();
		virtual void setViewYMiuns();
		virtual void setViewZPlus();
		virtual void setViewZMiuns();
		

		virtual void keyPressEvent(QKeyEvent *e) override;
		virtual void keyReleaseEvent(QKeyEvent *e) override;


	private slots:
		virtual void setDisplay(QString m);
		//virtual void setGeometryDisplay(QString m);
		virtual void setSelectModel(int mode);
		virtual void updateGraphOption();
	


	protected:
		int _projectId{ -1 };
		GraphWindowType _graphWindowType{ GraphWindowType::UnDefined };
		

	};


}


#endif
