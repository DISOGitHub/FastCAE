#ifndef _COMMANDLINE_H__
#define _COMMANDLINE_H__

#include <QString>

namespace GUI
{
	class MainWindow;
}

class CommandPara
{
public:
	CommandPara(int argc, char* argv[]);
	~CommandPara() = default;

	bool isHelp();
	bool exec(GUI::MainWindow* window);

private:
	void fileWarning(QString f, GUI::MainWindow* w);
	void connectToConsole();

private:
	bool _showGUI{ true };
	bool _help{ false };
	QString _scriptFile{};
	QString _projectFile{};

};



#endif
