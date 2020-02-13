/**************************
对话框基类，提供确认取消等按钮
会对输入的数据后进行检测
**************************/
#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include "moduleBaseAPI.h"
#include <QList>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QLabel>
#include "SelfDefObject/QFDialog.h"
#include "Frameless/FramelessHelper.h"

#define _XMG_DIALOG_NEAR_PIXELS_			5

class QVBoxLayout;
class QBoxLayout;

namespace SelfDefObj
{
	class SelfDefWidget;
}
namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	enum DialogType
	{
		DialogType_None = 0,
		DialogType_Hori,
		DialogType_Veri,
	};

	enum TitleButtonType
	{
		Button_Minimize = 0,
		Button_Maximize,
		Button_Restore,
		Button_Close,
	};

	class MODULEBASEAPI DialogBase : public QFDialog
	{
		Q_OBJECT

	public:
		DialogBase(GUI::MainWindow* m, DialogType type, QWidget *parent = 0, Qt::WindowFlags f = 0);
		~DialogBase();

	protected:
		void appendWidget(SelfDefObj::SelfDefWidget* widget);
		///清空Dialog内所有的Widgets
		void clearWidgets();


	private:
		void initMainLayout();
		bool apply();
		bool check();

	private slots:
		void on_OKButton();
		void on_applyButton();
		void on_cancelButton();

	protected:
		DialogType _type{ DialogType_None };
		QVBoxLayout* _mainLayout{};
		QBoxLayout* _layout{};
		QList<SelfDefObj::SelfDefWidget*> _widgetList{};

	private:
		///<MG hold the icon of the title
		QLabel * m_pIconLabel{};

		///<MG hold the text of the title
		QLabel * m_pTitleLabel{};

		///<MG hold the minimize button of the title
		QPushButton * m_pMinimizeButton{};

		///<MG hold the maximize button of the title
		QPushButton * m_pMaximizeButton{};

		///<MG hold the restore button of the title
		QPushButton * m_pRestoreButton{};

		///<MG hold the close button of the title
		QPushButton * m_pCloseButton{};

		///<MG title 's layout
		QHBoxLayout * m_pTitleLayout{};

		///<MG big small tool
		FramelessHelper *m_pHelper{};

		QPen m_titlePen{ QPen(QBrush(QColor(108, 138, 154)), 1) };
		QBrush m_titleBrush{ QBrush(QColor(108, 138, 154)) };
		QPen m_borderPen{ QPen(QBrush(QColor(128, 180, 255)), 1) };

		///<MG the default title height of the dialog is 20
		int m_titleHeight{ 27 };

		bool m_fixedSize{ true };

	private:
		///<MG initialize the title of the dialog
		void initializeTitle();

		private slots:
		///<MG slot to deal with the button clicked signal
		void slot_clicked_button();

	protected:
		///<MG re implement the mouse double clicked event to make sure dialog size changes with the title double clicked event
		virtual void mouseDoubleClickEvent(QMouseEvent * event) override;

		///<MG re draw the border of the dialog
		void paintEvent(QPaintEvent * event) override;

		///<MG set dialog fixed size state
		void setFixedSizeDialog(int width, int height, bool bFixed = true);

		///<MG set dialog fixed size state
		void setFixedSizeDialog(const QSize & min, const QSize & max, bool bFixed = true);

	//public:
		///<MG set the height of the title of the dialog 
		void setTitleHeight(int height);

		///<MG set the title of the dialog
		void setDialogTitle(QString title);

		///<MG set the button 's picture with different state
		void setButtonPicture(TitleButtonType type, const QString & normal, const QString & pressed);

		///<MG set title brush to draw the dialog title
		void setTitleBrush(const QBrush & brush);

		///<MG set the border pen to draw the dialog border
		void setBorderPen(const QPen & brush);

		///<MG set the icon of the dialog
		void setIcon(const QIcon & icon);
	};
}



#endif