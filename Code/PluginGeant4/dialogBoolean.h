#ifndef DIALOGBOOLEAN_H
#define DIALOGBOOLEAN_H

#include <QDialog>

namespace Ui {
class DialogBoolean;
}

namespace Geant4 {
	class SolidShapeBase;
	class SolidBoolean;

	class DialogBoolean : public QDialog
	{
		Q_OBJECT

	public:
		DialogBoolean(SolidShapeBase* data);
		~DialogBoolean();

	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;


		void displayPositionData();
		void displayRotationData();
	private:
		Ui::DialogBoolean *m_ui;
		SolidBoolean* m_booleanData;
	};
}
#endif // DIALOGBOOLEAN_H
