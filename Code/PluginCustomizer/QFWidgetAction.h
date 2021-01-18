#ifndef QMYWIDGETACTION_H
#define QMYWIDGETACTION_H

#include <QWidgetAction>
#include <QObject>
#include <QCheckBox>
#include <QWidget>
#include <QHBoxLayout>


namespace FastCAEDesigner
{

	class QFWidgetAction : public QWidgetAction
	{
		Q_OBJECT

	public:
		QFWidgetAction(QWidget *parent = 0);
		~QFWidgetAction();

	public:
		void SetText(QString text);
		QString GetText();
		void SetChechBoxChecked(bool on);

		bool getCheckBoxChecked();

	protected:
		virtual QWidget *createWidget(QWidget *parent);

		public slots:
		void OnCheckBoxStateChanged(int state);

	signals:
		void signal_CheckBoxStateChanged(int state);

	public:

	protected:

	private:
		QCheckBox* _checkBox;
		QWidget* _widget;
		QHBoxLayout *_hlayout;
		QString _text{};
		QString _name{};

	};

}

#endif // QMYWIDGETACTION_H
