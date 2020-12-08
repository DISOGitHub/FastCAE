#include "messageWindowBase.h"
#include <ui_messageWindowBase.h>
#include "mainWindow/mainWindow.h"
#include <QMenu>
#include "QColorDialog"
#include "QFileDialog"
#include "settings/busAPI.h"
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>
#include "python/PyAgent.h"

namespace ModuleBase
{

	MessageWindowBase::MessageWindowBase(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) :
		DockWidgetBase(mainwindow, ModuleType::MessageWindows)
	{
		qRegisterMetaType<Message>("Message");

		_ui = new Ui::MessageWindow();
		_ui->setupUi(this);
		initialize();

		connect(mainwindow, SIGNAL(printMessageToMessageWindow(QString)), this, SLOT(showMessage(QString)));
		connect(mainwindow, SIGNAL(printMessageToMessageWindow(Message)), this, SLOT(showMessage(Message)));

		m_typeNames[Normal_Message] = tr("[Normal]:   ");
		m_typeNames[Warning_Message] = tr("[Warning]:  ");
		m_typeNames[Error_Message] = tr("[Error]:    ");
		m_typeNames[Python] = tr("[Python]:    ");

		m_typeColors[Normal_Message] = TypeMessageColor(Normal_Message, QColor(120, 240, 120), QColor(255, 0, 0));
		m_typeColors[Warning_Message] = TypeMessageColor(Warning_Message, QColor(240, 120, 120), QColor(255, 0, 255));
		m_typeColors[Warning_Message] = TypeMessageColor(Warning_Message, QColor(240, 60, 60), QColor(0, 255, 255));
		m_typeColors[Python] = TypeMessageColor(Python, QColor(248, 213, 55), QColor(59, 120, 167));

		_ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
		_ui->textEdit->installEventFilter(this);
		connect(_ui->textEdit, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slot_right_button_clicked_text_edit()));
	}
	MessageWindowBase::~MessageWindowBase()
	{
		delete _ui;
		_ui = nullptr;
	}
	void MessageWindowBase::reTranslate()
	{
		_ui->retranslateUi(this);
		DockWidgetBase::reTranslate();
	}
	void MessageWindowBase::showMessage(QString message)
	{
		if (_ui == nullptr) return;
		_ui->textEdit->append(message);
		QString str = _ui->textEdit->toPlainText();
		_ui->textEdit->update();
	}

	void MessageWindowBase::showMessage(Message message, bool first /*= true*/)
	{
		if (first)
		{
			if (message.time.isEmpty())
			{
				message.time = QTime::currentTime().toString("hh:mm:ss");
			}
			m_messages.push_back(message);
		}

		if (((message.type == Normal_Message) && (m_showMode & Normal_Show)) ||
			((message.type == Warning_Message) && (m_showMode & Warning_Show)) ||
			((message.type == Error_Message) && (m_showMode & Error_Show)) ||
			(message.type == Python))
		{
			QColor typeColor = m_typeColors[message.type].typeColor;
			QColor itemColor = m_typeColors[message.type].itemColor;
			QString newMessage;
			if (m_showType)
			{
				newMessage = QString("<p style=\" margin-top:1px; margin-bottom:2px; \"><span style=\" font-size:%1pt; font-weight:600; color:rgb(%2, %3, %4); \">%5</span><span style=\" font-size:%6pt; color:rgb(%7, %8, %9); \">%10</span></p>").arg(m_fontPts).arg(typeColor.red()).arg(typeColor.green()).arg(typeColor.blue()).arg(m_typeNames[message.type]).arg(m_fontPts).arg(itemColor.red()).arg(itemColor.green()).arg(itemColor.blue()).arg(message.message);
			}
			else
			{
				newMessage = QString("<p style=\" margin-top:1px; margin-bottom:2px; \"><span style=\" font-size:%1pt; color:rgb(%2, %3, %4); \">%5</span></p>").arg(m_fontPts).arg(itemColor.red()).arg(itemColor.green()).arg(itemColor.blue()).arg(message.message);
			}
			_ui->textEdit->append(newMessage);
			
			_ui->textEdit->update();
		}
	}

	bool MessageWindowBase::executePyscript()
	{
		bool ok = false;
		QStringList execodelist{};
		QString pyscript = _ui->textEdit->toPlainText();
		if (m_messages.size()>0)
		{
			for  (Message msg:m_messages)
			{
				QString removestr = m_typeNames[msg.type].remove(QRegExp("\\s"))+" " + msg.message;
				if (pyscript.contains(removestr))
					pyscript.remove(removestr);
			}

		}
		if (pyscript.contains("executed"))
		{
			QStringList executelist = pyscript.split("executed!");
			QString last = executelist.back();
			QStringList lastlist = last.split("\n");
			for (int i = 0; i < lastlist.size(); i++)
				if (!lastlist[i].isEmpty()) execodelist.append(lastlist[i]);
		}
		else
		{
			QStringList lastlist = pyscript.split("\n");
			for (int i = 0; i < lastlist.size(); i++)
				if (!lastlist[i].isEmpty()) execodelist.append(lastlist[i]);
		}
		
		for (int i = 0; i < execodelist.size(); i++)
			Py::PythonAagent::getInstance()->execMessWinCode(execodelist[i]);
		
		if (execodelist.size() > 0)
			ok = true;
		return ok;
	}

	void MessageWindowBase::setColor(const TypeMessageColor & color)
	{
		TypeMessageColor before = m_typeColors[color.type];
		if (before.itemColor != color.itemColor || before.typeColor != color.typeColor)
		{
			m_typeColors.insert(color.type, color); 
			updateMessage();
		}
	}
	
	ModuleBase::TypeMessageColor MessageWindowBase::color(MessageType type) const
	{
		return m_typeColors[type];
	}

	void MessageWindowBase::setShow(int mode)
	{
		if (m_showType != mode)
		{
			m_showMode = mode;
			updateMessage();
		}

	}

	int MessageWindowBase::show() const
	{
		return m_showMode;
	}

	void MessageWindowBase::setShowType(bool _showType)
	{
		if (m_showType != _showType)
		{
			m_showType = _showType;
			updateMessage();
		}
	}

	bool MessageWindowBase::showType() const
	{
		return m_showType;
	}

	int MessageWindowBase::font()
	{
		return m_fontPts;
	}

	void MessageWindowBase::setFont(int _font)
	{
		if (m_fontPts != _font)
		{
			m_fontPts = _font;
			updateMessage();
		}
	}

	bool MessageWindowBase::eventFilter(QObject *target, QEvent *event)
	{

		if (target == _ui->textEdit)
		{
			if (event->type() == QEvent::KeyPress)//»Ø³µ¼ü
			{
			
				QKeyEvent *k = static_cast<QKeyEvent *>(event);
				if (k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter)
				{
					bool iscommand = isParaComaand();
					if (iscommand == true)
					{
						bool ok = executePyscript();
						if (ok)
							_ui->textEdit->insertPlainText(QString("\nexecuted!"));
					}
					else
					{
						_ui->textEdit->insertPlainText(QString("\n"));
						_ui->textEdit->update();
					}
					return true;
				}
			}
		}
		return QWidget::eventFilter(target, event);
	}

	bool MessageWindowBase::isParaComaand()
	{
		bool ok = false;
		QString pyscript = _ui->textEdit->toPlainText();
		QStringList pylist = pyscript.split("\n");
		QString last = pylist.back();
		if (pyscript.endsWith("/n") == false && !(last.contains("executed") || last.contains("[Normal]") || last.contains("[Warning]") || last.contains("[Error]") || last.contains("[Python]")))
			return ok=true;
		return ok;
	}

	void MessageWindowBase::updateMessage()
	{
		_ui->textEdit->clear();
		for (Message msg : m_messages)
		{
			showMessage(msg, false);
		}
	
	}

	void MessageWindowBase::initialize()
	{
		//	m_fontPts
		m_fontPts = Setting::BusAPI::instance()->getMessageFontPts();
		m_showMode = Setting::BusAPI::instance()->getMessageShowMode();
		m_showType = Setting::BusAPI::instance()->getMessageShowType();
		m_showTime = Setting::BusAPI::instance()->getMessageShowTime();
		m_bkColor = Setting::BusAPI::instance()->getMessageBkColor();
		if (m_fontPts < 9) m_fontPts = 9;
		_ui->textEdit->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(m_bkColor.red()).arg(m_bkColor.green()).arg(m_bkColor.blue()));
	}

	void MessageWindowBase::slot_right_button_clicked_text_edit()
	{
		QMenu *menu = new QMenu(_ui->textEdit);
		menu->setStyleSheet("background-color: rgb(240,240,240)");

		QAction * actionBKColor = menu->addAction(tr("Background Color"));

		QMenu * menuNormal = menu->addMenu(tr("Normal"));
		QAction * actionNoramlShow = menuNormal->addAction(tr("Show Normal"));
		QAction * actionNormalHide = menuNormal->addAction(tr("Hide Normal"));

		QMenu * menuWarning = menu->addMenu(tr("Warning"));
		QAction * actionWarningShow = menuWarning->addAction(tr("Show Warning"));
		QAction * actionWarningHide = menuWarning->addAction(tr("Hide Warning"));

		QMenu * menuError = menu->addMenu(tr("Error"));
		QAction * actionErrorShow = menuError->addAction(tr("Show Error"));
		QAction * actionErrorHide = menuError->addAction(tr("Hide Error"));

		menu->addSeparator();
		QMenu * menuHeader = menu->addMenu(tr("Header"));
		QAction * actionHeaderShow = menuHeader->addAction(tr("Show Header"));
		QAction * actionHeaderHide = menuHeader->addAction(tr("Hide Header"));

		QMenu * menuFont = menu->addMenu(tr("Font"));
		QAction * actionFontLarger = menuFont->addAction(tr("Larger Font"));
		QAction * actionFontSmaller = menuFont->addAction(tr("Smaller Font"));

		menu->addSeparator();
		QAction * actionSaveAs = menu->addAction(tr("Save As..."));
		QAction * actionClear = menu->addAction(tr("Clear"));
		//	QAction * actionClose = menu->addAction("Close");

		connect(actionBKColor, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_background_color_action()));
		connect(actionNoramlShow, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_show_normal_action()));
		connect(actionNormalHide, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_hide_noraml_action()));
		connect(actionWarningShow, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_show_warning_action()));
		connect(actionWarningHide, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_hide_warning_action()));
		connect(actionErrorShow, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_show_error_action()));
		connect(actionErrorHide, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_hide_error_action()));
		connect(actionHeaderShow, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_show_header_action()));
		connect(actionHeaderHide, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_hide_header_action()));
		connect(actionFontLarger, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_font_larger_action()));
		connect(actionFontSmaller, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_font_smaller_action()));
		connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_save_as_action()));
		connect(actionClear, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_clear_all_action()));
		//	connect(actionClose, SIGNAL(triggered()), this, SLOT(slot_clicked_menu_close_action()));

		menu->exec(QCursor::pos());
	}

	void MessageWindowBase::slot_clicked_menu_background_color_action()
	{
		QColor oldColor = m_bkColor;
		m_bkColor = QColorDialog::getColor(Qt::blue);
		if (m_bkColor.isValid())
		{
			_ui->textEdit->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(m_bkColor.red()).arg(m_bkColor.green()).arg(m_bkColor.blue()));
			Setting::BusAPI::instance()->setMessageBkColor(m_bkColor);
		}
		else
			m_bkColor = oldColor;
	}

	void MessageWindowBase::slot_clicked_menu_show_normal_action()
	{
		setShow(m_showMode | Normal_Show);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_hide_noraml_action()
	{
		setShow(m_showMode & 0xFFFFFFFE);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_show_warning_action()
	{
		setShow(m_showMode | Warning_Show);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_hide_warning_action()
	{
		setShow(m_showMode & 0xFFFFFFFD);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_show_error_action()
	{
		setShow(m_showMode | Error_Show);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_hide_error_action()
	{
		setShow(m_showMode & 0xFFFFFFFB);
		Setting::BusAPI::instance()->setMessageShowMode(m_showMode);
	}

	void MessageWindowBase::slot_clicked_menu_show_header_action()
	{
		setShowType(true);
		Setting::BusAPI::instance()->setMessageShowType(true);
	}

	void MessageWindowBase::slot_clicked_menu_hide_header_action()
	{
		setShowType(false);
		//_ui->textEdit->insertPlainText(QString("\nexecuted!"));
		Setting::BusAPI::instance()->setMessageShowType(false);
	}

	void MessageWindowBase::slot_clicked_menu_font_larger_action()
	{
		m_fontPts += 1;
		updateMessage();
		Setting::BusAPI::instance()->setMessageFontPts(m_fontPts);
	}

	void MessageWindowBase::slot_clicked_menu_font_smaller_action()
	{
		if (m_fontPts > 9)
		{
			m_fontPts -= 1;
			updateMessage();
			Setting::BusAPI::instance()->setMessageFontPts(m_fontPts);
		}
	}

	void MessageWindowBase::slot_clicked_menu_save_as_action()
	{
		QString filePath = QFileDialog::getSaveFileName(this, tr("Save Message Info"), QString(), tr("*.txt"));
		if (!filePath.isEmpty())
		{
			if (!filePath.contains(".txt"))
			{
				filePath += ".txt";
			}

			QFile file(filePath);
			if (file.open(QIODevice::WriteOnly))
			{
				for (Message msg : m_messages)
				{
					QString strMsg = QString("%1 %2 [%3]\r\n").arg(m_typeNames[msg.type]).arg(msg.message).arg(msg.time);
					file.write(strMsg.toStdString().c_str());
				}
				file.close();
			}
		}
	}

	void MessageWindowBase::slot_clicked_menu_clear_all_action()
	{
		_ui->textEdit->clear();
		m_messages.clear();
	}

	void MessageWindowBase::slot_clicked_menu_close_action()
	{
		this->hide();///<MG
	}

}