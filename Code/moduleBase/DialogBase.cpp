#include "DialogBase.h"
#include "QFDialog.h"
#include "SelfDefObject/SelfDefWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDesktopWidget>

///<MG 
#include <QPainter>
#include <QDebug>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif


namespace ModuleBase
{
	DialogBase::DialogBase(GUI::MainWindow* m, DialogType type, QWidget *parent /* = 0 */, Qt::WindowFlags f /* = 0 */)
		:QFDialog(m), _type(type)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
// 		this->setFocusPolicy(Qt::ClickFocus);
// 		this->grabKeyboard();

		m_pHelper = new FramelessHelper(this);
		m_pHelper->activateOn(this);									//激活当前窗体
		m_pHelper->setTitleHeight(30);									//设置窗体的标题栏高度
		m_pHelper->setWidgetMovable(true);								//设置窗体可移动
		m_pHelper->setWidgetResizable(true);							//设置窗体可缩放
		//pHelper->setRubberBandOnMove(true);						//设置橡皮筋效果-可移动
		//pHelper->setRubberBandOnResize(true);						//设置橡皮筋效果-可缩放

		initMainLayout();
		setIcon(QIcon(":/QUI/main.ico"));
	}
	DialogBase::~DialogBase()
	{
		clearWidgets();
		if (m_pTitleLayout != nullptr) delete m_pTitleLayout;

		if (_layout != nullptr) delete _layout;
		if (_mainLayout != nullptr)delete _mainLayout;

		if (m_pIconLabel != nullptr) delete m_pIconLabel;
		if (m_pTitleLabel != nullptr) delete m_pTitleLabel;
		if (m_pMinimizeButton != nullptr) delete m_pMinimizeButton;
		if (m_pMaximizeButton != nullptr) delete m_pMaximizeButton;
		if (m_pCloseButton != nullptr) delete m_pCloseButton;
		if (m_pHelper != nullptr) delete m_pHelper;
	}
	void DialogBase::initMainLayout()
	{
		_mainLayout = new QVBoxLayout;
		_mainLayout->setSpacing(0);

		_mainLayout->setContentsMargins(5, 0, 5, 0);
		switch (_type)   //根据类型创建layout，控件的插入位置
		{
		case ModuleBase::DialogType_Hori:
			_layout = new QHBoxLayout;
			break;
		case ModuleBase::DialogType_Veri:
			_layout = new QVBoxLayout;
			break;
		default:
			break;
		}

		initializeTitle();
		_mainLayout->addLayout(m_pTitleLayout);

		QHBoxLayout* buttonlayout = new QHBoxLayout;
		buttonlayout->setSpacing(6);
		buttonlayout->setContentsMargins(0, 0, 9, 9);
		QPushButton* okButton = new QPushButton(tr("OK"), this);
		QPushButton* applyButton = new QPushButton(tr("Apply"), this);
		QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);

		okButton->setObjectName("out_OkButton");
		applyButton->setObjectName("out_ApplyButton");
		cancelButton->setObjectName("out_CancelButton");

		connect(okButton, SIGNAL(clicked()), this, SLOT(on_OKButton()));
		connect(applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton()));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton()));
		buttonlayout->addStretch();
		buttonlayout->addWidget(okButton);
		buttonlayout->addWidget(applyButton);
		buttonlayout->addWidget(cancelButton);
		if (_layout)
		{
			_layout->setMargin(0);
			_layout->setSpacing(0);
			_mainLayout->addLayout(_layout);
		}
		_mainLayout->addLayout(buttonlayout);
		this->setLayout(_mainLayout);
	}
	void DialogBase::on_applyButton()
	{
		apply();
	}
	void DialogBase::on_OKButton()
	{
		if (apply())
			this->accept();
	}
	void DialogBase::on_cancelButton()
	{
		this->reject();
		this->close();
	}

	void DialogBase::appendWidget(SelfDefObj::SelfDefWidget* widget)
	{
		_layout->addWidget(widget);
		_widgetList.append(widget);
	}

	bool DialogBase::check()
	{
		const int n = _widgetList.size();
		for (int i = 0; i < n; ++i)
		{
			SelfDefObj::SelfDefWidget* w = _widgetList.at(i);
			if (!w->checkObjs())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Illegal inputs exists !"));
				return false;
			}
		}
		return true;
	}

	bool DialogBase::apply()
	{
		if (!check()) return false;
		const int n = _widgetList.size();
		for (int i = 0; i < n; ++i)
		{
			auto w = _widgetList.at(i);
			w->apply();
		}
		return true;
	}
	void DialogBase::clearWidgets()
	{
		const int n = _widgetList.size();
		for (int i = 0; i < n; ++i)
		{
			auto w = _widgetList.at(i);
			_layout->removeWidget(w);
			w->setParent(nullptr);
			if (w != nullptr) delete w;
		}
		_widgetList.clear();
	}

	void DialogBase::setFixedSizeDialog(int width, int height, bool bFixed /*= true*/)
	{
		m_fixedSize = bFixed;
		m_pMaximizeButton->setEnabled(!bFixed);
		m_pRestoreButton->setEnabled(!bFixed);
		setMinimumSize(width, height);
		setMaximumSize(width, height);
		if (!m_pHelper){ return; }
		m_pHelper->setWidgetResizable(false);
	}

	void DialogBase::setFixedSizeDialog(const QSize & min, const QSize & max, bool bFixed /*= true*/)
	{
		m_fixedSize = bFixed;
		m_pMaximizeButton->setEnabled(!bFixed);
		m_pRestoreButton->setEnabled(!bFixed);
		setMinimumSize(min);
		setMaximumSize(max);
		if (!m_pHelper){ return; }
		m_pHelper->setWidgetResizable(true);
	}

	void DialogBase::setDialogTitle( QString title)
	{
		qDebug() << title;
	//	title = QString::fromLocal8Bit(title.toLatin1());
		m_pTitleLabel->setText(title);
	}

	void DialogBase::setButtonPicture(TitleButtonType type, const QString & normal, const QString & pressed)
	{
		QPushButton * pButton = NULL;
		if (type == Button_Minimize)
			pButton = m_pMinimizeButton;
		else if (type == Button_Maximize)
			pButton = m_pMaximizeButton;
		else if (type == Button_Restore)
			pButton = m_pRestoreButton;
		else if (type == Button_Close)
			pButton = m_pCloseButton;
		else
			return;

		QString styleSheet = QString("QPushButton{ color: white; font: bold 10pt; border:none; min-height: 24px; min-width: 24px; background:url(%1) -60px 0px no-repeat;}QPushButton:hover{color: lightgray;background:url(%2) 0px 0px no-repeat;}QPushButton:pressed {padding-left: 5px;padding-top: 5px;background:url(%3) 0px 0px no-repeat;}").arg(normal).arg(pressed).arg(pressed);

		pButton->setStyleSheet(styleSheet);
	}

	void DialogBase::setTitleBrush(const QBrush & brush)
	{
		m_titleBrush = brush;
		update();
	}

	void DialogBase::setBorderPen(const QPen & pen)
	{
		m_borderPen = pen;
		update();
	}

	void DialogBase::setIcon(const QIcon & icon)
	{
		m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
	}

	void DialogBase::paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		painter.setBrush(m_titleBrush);
		painter.setPen(m_titlePen);
		painter.drawRect(QRect(0, 0, this->width(), m_titleHeight));

		painter.setPen(m_borderPen);
		QRect rect = this->rect();
		int left = rect.left(), top = rect.top(), right = rect.right(), bottom = rect.bottom();
		QPointF points[4] = { QPointF(left, top + m_titleHeight), QPointF(left, bottom), QPointF(right, bottom), QPointF(right, top + m_titleHeight) };
		painter.drawLine(points[0], points[1]);
		painter.drawLine(points[1], points[2]);
		painter.drawLine(points[2], points[3]);
		QDialog::paintEvent(event);
	}

	void DialogBase::setTitleHeight(int height)
	{
		m_titleHeight = height;
	}

	void DialogBase::initializeTitle()
	{
		m_pIconLabel = new QLabel(this);
		m_pTitleLabel = new QLabel(this);
		m_pTitleLabel->setObjectName("DialogBaseTitleLabel");
		m_pMinimizeButton = new QPushButton(this);
		m_pMaximizeButton = new QPushButton(this);
		m_pRestoreButton = new QPushButton(this);
		m_pCloseButton = new QPushButton(this);

	//	m_pTitleLabel->setObjectName("title_lable");

		m_pRestoreButton->hide();

		m_pMinimizeButton->setToolTip(tr("Minimize"));
		m_pMaximizeButton->setToolTip(tr("Maximize"));
		m_pRestoreButton->setToolTip(tr("Restore"));
		m_pCloseButton->setToolTip(tr("Close"));

		m_pTitleLayout = new QHBoxLayout();
		m_pTitleLayout->addWidget(m_pIconLabel);
		m_pTitleLayout->addSpacing(5);
		m_pTitleLayout->addWidget(m_pTitleLabel);
		m_pTitleLayout->addWidget(m_pMinimizeButton);
		m_pTitleLayout->addWidget(m_pMaximizeButton);
		m_pTitleLayout->addWidget(m_pRestoreButton);
		m_pTitleLayout->addWidget(m_pCloseButton);
		m_pTitleLayout->setSpacing(0);
		m_pTitleLayout->setContentsMargins(0, 0, 0, 0);

		m_pIconLabel->setFixedSize(/*m_titleHeight*/32, /*m_titleHeight*/32);
		m_pIconLabel->setScaledContents(true);

		m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		setButtonPicture(Button_Minimize, ":/Beauty/QUI/beauty/min_normal.png", ":/Beauty/QUI/beauty/min_pressed.png");
		setButtonPicture(Button_Maximize, ":/Beauty/QUI/beauty/max_normal.png", ":/Beauty/QUI/beauty/max_pressed.png");
		setButtonPicture(Button_Restore, ":/Beauty/QUI/beauty/restore_normal.png", ":/Beauty/QUI/beauty/restore_pressed.png");
		setButtonPicture(Button_Close, ":/Beauty/QUI/beauty/close_normal.png", ":/Beauty/QUI/beauty/close_pressed.png");

		connect(m_pMinimizeButton, SIGNAL(clicked()), SLOT(slot_clicked_button()));
		connect(m_pMaximizeButton, SIGNAL(clicked()), SLOT(slot_clicked_button()));
		connect(m_pRestoreButton, SIGNAL(clicked()), SLOT(slot_clicked_button()));
		connect(m_pCloseButton, SIGNAL(clicked()), SLOT(slot_clicked_button()));
	}

	void DialogBase::slot_clicked_button()
	{
		QPushButton * pButton = qobject_cast<QPushButton*>(sender());
		QWidget * pWindow = this->window();
		if (pWindow->isTopLevel())
		{
			if (pButton == m_pMinimizeButton)
			{
				pWindow->showMinimized();
			}
			else if (pButton == m_pMaximizeButton)
			{
				pWindow->showMaximized();
				m_pRestoreButton->show();
				m_pMaximizeButton->hide();
			}
			else if (pButton == m_pRestoreButton)
			{
				pWindow->showNormal();
				m_pMaximizeButton->show();
				m_pRestoreButton->hide();
			}
			else if (pButton == m_pCloseButton)
			{
				pWindow->close();
			}
		}
	}

	void DialogBase::mouseDoubleClickEvent(QMouseEvent * event)
	{
		Q_UNUSED(event);
		if (m_fixedSize){ return; }
		QWidget * pWindow = this->window();
		if (pWindow->isMaximized())
			emit m_pRestoreButton->clicked();
		else
			emit m_pMaximizeButton->clicked();
	}
}