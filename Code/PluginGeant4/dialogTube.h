#ifndef DIALOGTUBE_H
#define DIALOGTUBE_H

#include <QDialog>

namespace Ui {
class DialogTube;
}

namespace Geant4 {
	class SolidShapeBase;
	class SolidTube;

	class DialogTube : public QDialog
	{
		Q_OBJECT

	public:
		DialogTube(SolidShapeBase* data);
		~DialogTube();

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogTube *m_ui;
		SolidTube* m_tubeData;
	};
}
#endif // DIALOGTUBE_H
