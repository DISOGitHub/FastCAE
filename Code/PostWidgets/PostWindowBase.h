#ifndef _POSTWINDOWBASE_H_
#define _POSTWINDOWBASE_H_

#include "PostWidgetsAPI.h"
#include "moduleBase/graphWindowBase.h"
#include <QWidget>
#include <QList>

class QToolBar;

namespace GUI
{
	class MainWindow;
}


namespace Post
{
	enum PostWindowType
	{
		UnDefined = 0,
		D2,
		D3,
		D2D3,
	};

	class POSTAPI PostWindowBase : public ModuleBase::GraphWindowBase
	{
		Q_OBJECT
	public:
		PostWindowBase(GUI::MainWindow* mainwindow, int proID, ModuleBase::GraphWindowType gt, PostWindowType type);
		~PostWindowBase();
		//��������
		void setWindowType(PostWindowType type);
		//��ȡ����
		PostWindowType getPostWindowType();
		//��ӻ�ͼ����
		void appendGraphWindow(ModuleBase::GraphWindowBase* g);
		//�Ƴ�����
		void removeGraphWindow(ModuleBase::GraphWindowBase* g);
		//�Ƴ���index����ͼ����
		void removeGraphWindow(int index);
		//���ȫ����ͼ����
		void removeAllGraphWindow();
		//�������δ���
		void setTreeWidget(QWidget* w);
		//��ȡ���δ���
		QWidget* getTreeWidget();
		//�������Դ���
		void setPropWidget(QWidget* w);
		//��ȡ���Դ���
		QWidget* getPropWidget();
		//��ӹ�����
		void appendToolBar(QToolBar* toolbar);
		//�Ƴ�������
		void removeToolBar(QToolBar* toolbar);
		//��ȡȫ��������
		QList<QToolBar*> getToolBarList();
		virtual void setView(QString view) override;
		virtual void setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3) override;
	signals:
		//�رմ����ź�
		void closeWindowSignal(Post::PostWindowBase* w);

	protected:
		void closeEvent(QCloseEvent *e) override;
	
	protected:
		PostWindowType _windowType{ UnDefined };
		QList<ModuleBase::GraphWindowBase*> _graphWidgetList{};

		QWidget* _treeWidget{};
		QWidget* _propWidget{};
		QList<QToolBar*> _toolBarList{};

	};
}

#endif
