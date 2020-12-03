#ifndef DIALOGSPHERE_H
#define DIALOGSPHERE_H

#include <QDialog>

namespace Ui {
class DialogSphere;
}
namespace Geant4 {
	class SolidShapeBase;
	class SolidSphere;

	class DialogSphere : public QDialog
	{
		Q_OBJECT

	public:
		DialogSphere(SolidShapeBase* data);
		~DialogSphere();

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;



	private:
		Ui::DialogSphere *m_ui;
		SolidSphere* m_sphereData;
	};
}
#endif // DIALOGSPHERE_H
