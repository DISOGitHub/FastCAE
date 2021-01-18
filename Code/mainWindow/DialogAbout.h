#ifndef _DIALOGABOUT_H_
#define _DIALOGABOUT_H_

#include <QDialog>

namespace Ui
{
	class AboutDialog;
}

namespace GUI
{
	class AboutDialog :public QDialog
	{
		Q_OBJECT
	public:
		AboutDialog();
		~AboutDialog();

	private:
		void init();

	private:
		Ui::AboutDialog* _ui{};
	};
}


#endif