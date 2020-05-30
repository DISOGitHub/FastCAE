/************************
输出信息框
************************/
#ifndef _MESSAGEWINDOWBASE_H_
#define _MESSAGEWINDOWBASE_H_

#include "moduleBaseAPI.h"
#include "dockWidgetBase.h"
#include <QMap>
#include <QMetaType>
#include "QDateTime"

namespace Ui
{
	class MessageWindow;
}
namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	typedef enum tag_MessaegType
	{
		Normal_Message =0,
		Warning_Message,
		Error_Message,
		Python,
	}MessageType;

	typedef enum tag_ShowType
	{
		Normal_Show = 0x01,
		Warning_Show = 0x02,
		Error_Show = 0x04,
	}ShowType;

	typedef struct tag_Message
	{
		tag_Message(){}
		tag_Message(MessageType _type, QString _message)
		{
			type = _type;
			message = _message;
			time = QTime::currentTime().toString("hh:mm:ss");
		}
		MessageType type;
		QString message;
		QString time;
	}Message;

	typedef struct tag_TypeMessageColor
	{
		tag_TypeMessageColor(){}
		tag_TypeMessageColor(MessageType _type, QColor _typeColor, QColor _itemColor)
		{
			type = _type;
			typeColor = _typeColor;
			itemColor = _itemColor;
		}
		MessageType type;
		QColor typeColor;
		QColor itemColor;
	}TypeMessageColor;

	class MODULEBASEAPI MessageWindowBase : public DockWidgetBase
	{
		Q_OBJECT
	public:
		MessageWindowBase(GUI::MainWindow* mainwindow, QWidget* parent = 0);
		virtual ~MessageWindowBase();
		//翻译
		virtual void reTranslate() override;
		
		//执行python命令
		bool executePyscript();
		// 
		protected slots:
		virtual void showMessage(QString message);
		virtual void showMessage(Message message,bool first = true);
		
	protected:
		Ui::MessageWindow* _ui{};

	protected:
		///<MG
		void setColor(const TypeMessageColor & color);
		
		TypeMessageColor color(MessageType type) const;

		void setShow(int mode);

		int show() const;

		void setShowType(bool _showType);

		bool showType() const;

		int font();

		void setFont(int _font);

		bool eventFilter(QObject *target, QEvent *event);//事件过滤器

		bool isParaComaand();
	private:
		void updateMessage();

		///<MG initialize message attribute by config file 
		void initialize();

		private slots:
		void slot_right_button_clicked_text_edit();

		void slot_clicked_menu_background_color_action();
		void slot_clicked_menu_show_normal_action();
		void slot_clicked_menu_hide_noraml_action();
		void slot_clicked_menu_show_warning_action();
		void slot_clicked_menu_hide_warning_action();
		void slot_clicked_menu_show_error_action();
		void slot_clicked_menu_hide_error_action();
		void slot_clicked_menu_show_header_action();
		void slot_clicked_menu_hide_header_action();
		void slot_clicked_menu_font_larger_action();
		void slot_clicked_menu_font_smaller_action();
		void slot_clicked_menu_save_as_action();
		void slot_clicked_menu_clear_all_action();
		void slot_clicked_menu_close_action();
	private:
		int m_showMode{ Normal_Show | Warning_Show | Error_Show };
		QMap<MessageType, TypeMessageColor> m_typeColors;
		bool m_showType{ true };
		bool m_showTime{ true };
		QVector<Message> m_messages;
		QMap<MessageType, QString> m_typeNames{};
		int m_fontPts{11};
		QColor m_bkColor{ QColor(0, 0, 0) };


	};

	

}

Q_DECLARE_METATYPE(ModuleBase::Message)
#endif
