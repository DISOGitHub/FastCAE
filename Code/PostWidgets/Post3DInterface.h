#ifndef _POST3DWINDOWINTERFACE_H_
#define _POST3DWINDOWINTERFACE_H_

#include "PostWindowBase.h"

namespace GUI
{
	class MainWindow;
}

namespace Ui
{
	class Post3DInterface;
}

namespace Post
{
	class Post3DWindow;

	class POSTAPI Post3DWindowInterface : public PostWindowBase
	{
	public:
		Post3DWindowInterface(GUI::MainWindow* m, int proid);
		~Post3DWindowInterface();
		//获取三维绘图窗口
		Post3DWindow* getWindow();
		virtual void reTranslate() override;
		void saveImage(QString fileName, int width, int heigh, bool showDlg) override;

	private:
		void init();

	private:
		Ui::Post3DInterface* _ui{};
		Post3DWindow* _post3DWindow{};

	};
}


#endif
