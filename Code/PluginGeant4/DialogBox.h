#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QDialog>


class QTableWidget;

namespace Ui {
class DialogBox;
}

namespace Geant4 {
	
	class SolidShapeBase;
	class SolidBox;

	class DialogBox : public QDialog
	{
		Q_OBJECT

	public:
		DialogBox(SolidShapeBase* solid);
		~DialogBox();


	private:
		void init();
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogBox* m_ui;

		SolidBox* m_boxData;
	};
}
#endif // DIALOGSOLIDS_H
