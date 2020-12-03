#include <QDialog>

namespace Ui {
class DialogAddVolume;
}

namespace Geant4 {

	class DialogAddVolume : public QDialog
	{
		Q_OBJECT

	public:
		DialogAddVolume(QWidget * parent = nullptr,QString anaName = QString());
		~DialogAddVolume();

		QString getSelectedVolumeName();

	private:
		void closeEvent(QCloseEvent *) override;
		void reject() override;
		void accept() override;

	private:
		Ui::DialogAddVolume *m_ui;
		QString m_selectedVolumeName;
	};
}
