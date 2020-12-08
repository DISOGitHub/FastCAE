#ifndef EXAMPLEWIDGET_H__
#define EXAMPLEWIDGET_H__

#include <QWidget>
#include <QString>

namespace Ui
{
	class ExampleWidget;
}

namespace Py
{
	class PythonAagent;
}

namespace Guidence
{
	class ExampleWidget : public QWidget
	{
		Q_OBJECT
	public:
		ExampleWidget(QString path, QWidget* parent);
		~ExampleWidget();

	private slots:
		void on_detailsButton_clicked();
		void on_viewButton_clicked();
		void on_openButton_clicked();

	private:
		void init();
		void showEvent(QShowEvent *e) override;

	private:
		Ui::ExampleWidget* _ui{};
		QWidget* _parent{};

		QString _path{};
		Py::PythonAagent* _agent{};
	

	};
}



#endif