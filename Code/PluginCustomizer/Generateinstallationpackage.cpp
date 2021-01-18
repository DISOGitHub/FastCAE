#include "Generateinstallationpackage.h"
#include "ui_Generateinstallationpackage.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QDir>

namespace FastCAEDesigner{
// 	Generateinstallationpackage::Generateinstallationpackage(QWidget *parent) :
// 		QDialog(parent),
// 		ui(new Ui::Generateinstallationpackage)
// 	{
// 		ui->setupUi(this);
// 
// 
// 		connect(ui->PathPBtn, SIGNAL(clicked()), this, SLOT(OnPathPBtnClicked()));
// 		connect(ui->GeneratePBtn, SIGNAL(clicked()), this, SLOT(OnGeneratePBtnClicked()));
// 		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
// 	}

	Generateinstallationpackage::Generateinstallationpackage(QString name, QString version, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::Generateinstallationpackage),
		_version(version),
		_softName(name)
	{
		ui->setupUi(this);

		ui->label->setVisible(false);

		connect(ui->PathPBtn, SIGNAL(clicked()), this, SLOT(OnPathPBtnClicked()));
		connect(ui->GeneratePBtn, SIGNAL(clicked()), this, SLOT(OnGeneratePBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));

		_process = new QProcess;
		connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_console_output()));
		connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slot_console_finished(int, QProcess::ExitStatus)));
//		connect(_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(slot_console_state_changed(QProcess::ProcessState)));

	}

	Generateinstallationpackage::~Generateinstallationpackage()
	{
		_process->kill();
		delete _process;
		delete ui;
	}

	void Generateinstallationpackage::OnGeneratePBtnClicked()
	{
		_installPath = ui->PathLE->text().trimmed();

		if (_installPath.isEmpty())
		{
			QString errorMsg = QString("Install package path is empty.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		ui->label->setVisible(true);

		ui->GeneratePBtn->setEnabled(false);
		ui->CancelPBtn->setEnabled(false);

		run();

	}

	void Generateinstallationpackage::OnPathPBtnClicked()
	{
		QString selectpath = QFileDialog::getExistingDirectory(this, QString(tr("Select install path")));
		ui->PathLE->setText(selectpath);
	}

	void Generateinstallationpackage::deleteDir(const QString &path)
	{ 
		QDir dir(path);

		if (!dir.exists())
			return;

		dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
		QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
		foreach(QFileInfo file, fileList){ //遍历文件信息
			if (file.isFile()){ // 是文件，删除
				file.dir().remove(file.fileName());
			}
			else{ // 递归删除
				deleteDir(file.absoluteFilePath());
			}
		}
		dir.rmpath(dir.absolutePath()); // 删除文件夹
	}

	void Generateinstallationpackage::run()
	{
		/*
		QFile file(_installPath + "/" + _softName + ".exe");
		if (file.exists())
		{
			file.remove();
		}

		QString path = qApp->applicationDirPath();

		qDebug() << path;

		QDir dir(path);
		dir.cdUp();
		//dir.cdUp();
		//dir.cdUp();
		QString caePath = dir.path();

		qDebug() << caePath;

		QString install = caePath + "/Install/";

		deleteDir(install);
		

		//emit sig_print_info(QString::fromLocal8Bit("正在写入:整体配置信息配置！"));
		displayInstallInfo(QString("Writing: overall configuration information configuration!"));
		dir.mkpath(caePath + "/Install/config/");
		QString configFile = caePath + "/Install/config/config.xml";
		toConfigXML(configFile);

		//emit sig_print_info(QString::fromLocal8Bit("正在写入：配置组件信息配置！"));
		displayInstallInfo(QString("Writing: configuration component information configuration!"));
		QString configComponentPackage = caePath + "/Install/packages/org.diso.configfiles/meta/package.xml";
		dir.mkpath(caePath + "/Install/packages/org.diso.configfiles/meta/");
		toConfigComponent(configComponentPackage);

		//emit sig_print_info(QString::fromLocal8Bit("正在写入：配置组件信息脚本！"));
		displayInstallInfo(QString("Writing: configure component information script!"));
		QString configComponentQS = caePath + "/Install/packages/org.diso.configfiles/meta/installscript.qs";
		toConfigQS(configComponentQS);

		//emit sig_print_info(QString::fromLocal8Bit("正在写入：核心组件信息配置！"));
		displayInstallInfo(QString("Writing: core component information configuration!"));
		QString coreComponentPackage = caePath + "/Install/packages/org.diso.fastcae/meta/package.xml";
		dir.mkpath(caePath + "/Install/packages/org.diso.fastcae/meta/");
		toSoftComponent(coreComponentPackage);

		//emit sig_print_info(QString::fromLocal8Bit("正在写入：核心组件信息脚本！"));
		displayInstallInfo(QString("Writing: core component information script!"));
		QString coreComponentQS = caePath + "/Install/packages/org.diso.fastcae/meta/installscript.qs";
		toSoftQS(coreComponentQS);

		///<MG 拷贝配置
		QString config_dir_from = caePath + "/ConfigFiles";
		if (!dir.exists(config_dir_from))
		{
			dir.mkpath(config_dir_from);
		}
		QString config_dir_to = caePath + "/Install/packages/org.diso.configfiles/data/ConfigFiles";
		dir.mkpath(config_dir_to);
		bool result = copyDirectory(config_dir_from, config_dir_to, true);
		if (!result){
			displayInstallInfo(QString("Copy ConfilgFiles failed!"));
			qDebug() << "Copy ConfilgFiles failed!";
			//emit sig_generate_result(false, QString::fromLocal8Bit("拷贝ConfilgFiles失败！"));
		}

		///<MG 拷贝求解器
		QString solver_dir_from = caePath + "/Solver";
		QString solver_dir_to = caePath + "/Install/packages/org.diso.configfiles/data/Solver";
		dir.mkpath(solver_dir_to);
		result = copyDirectory(solver_dir_from, solver_dir_to, true);
		if (!result){
			displayInstallInfo(QString("Copy Solvers failed!"));
			qDebug() << "Copy ConfilgFiles failed!";
			//emit sig_generate_result(false, QString::fromLocal8Bit("拷贝Solvers失败！"));
		}

		///<MG 拷贝应用
		//QString core_dir_from = caePath + "/GUI";
		QString core_dir_from = path;
		QString core_dir_to = caePath + "/Install/packages/org.diso.fastcae/data/GUI";
		dir.mkpath(core_dir_to);
		result = copyGUIDirectory(core_dir_from, core_dir_to, true);
		if (!result){
			displayInstallInfo(QString("Copy GUI failed!"));
			qDebug() << "Copy ConfilgFiles failed!";
			//emit sig_generate_result(false, QString::fromLocal8Bit("拷贝GUI失败！"));
		}

		///<MG 拷贝Python环境
		QString python_dir_from = caePath + "/python37";
		QString python_dir_to = caePath + "/Install/packages/org.diso.fastcae/data/python37";
		dir.mkpath(python_dir_to);
		result = copyDirectory(python_dir_from, python_dir_to, true);
		if (!result){ displayInstallInfo(QString("Copy python37 failed!")); 
			qDebug() << "Copy ConfilgFiles failed!";
		}
			 

		///<MG 改变exe名字
		QString filePath = caePath + "/Install/packages/org.diso.fastcae/data/GUI/FastCAE.exe";
		QFile exe(filePath);
		if (exe.exists())
		{
			exe.rename(caePath + "/Install/packages/org.diso.fastcae/data/GUI/" + _softName + ".exe");
		}

		///<MG 运行脚本
 		QDir ctorDir(path);
 		ctorDir.cdUp();
// 		ctorDir.cdUp();

		QString shell = ctorDir.path() + "/binarycreator.exe";

		qDebug() << shell;

		QStringList args;
		args.append("-c");
		args.append(configFile);
		args.append("-p");
		args.append(caePath + "/Install/packages");
		args.append(_installPath + "/" + _softName + ".exe");
		args.append("-v");
		displayInstallInfo(QString("Writing to the installation package"));
		//emit sig_print_info(QString::fromLocal8Bit("正在写入安装包！"));

//		_process = new QProcess();
		qDebug() << args;

		_process->start(shell, args);
		*/

	QString sysPath = "/Install/packages/org.qianfan.";

	QFile file(_installPath + "/" + _softName + ".exe");
	if (file.exists())
	{
		file.remove();
	}

	QString path = qApp->applicationDirPath();

	qDebug() << path;

	QDir dir(path);
	dir.cdUp();
	QString caePath = dir.path();

//	qDebug() << caePath;

	QString install = caePath + "/Install/";

	deleteDir(install);


	displayInstallInfo(QString("Writing: overall configuration information configuration!"));
	dir.mkpath(caePath + "/Install/config/");
	QString configFile = caePath + "/Install/config/config.xml";
	toConfigXML(configFile);

	displayInstallInfo(QString("Writing: configuration component information configuration!"));
	QString configComponentPackage = caePath + sysPath + "configfiles/meta/package.xml";
	dir.mkpath(caePath + sysPath + "configfiles/meta/");
	toConfigComponent(configComponentPackage);

	displayInstallInfo(QString("Writing: configure component information script!"));
	QString configComponentQS = caePath + sysPath + "configfiles/meta/installscript.qs";
	toConfigQS(configComponentQS);

	displayInstallInfo(QString("Writing: core component information configuration!"));
	QString coreComponentPackage = caePath + sysPath + "fastcae/meta/package.xml";
	dir.mkpath(caePath + sysPath + "fastcae/meta/");
	toSoftComponent(coreComponentPackage);

	displayInstallInfo(QString("Writing: core component information script!"));
	QString coreComponentQS = caePath + sysPath + "fastcae/meta/installscript.qs";
	toSoftQS(coreComponentQS);

	///<MG 拷贝配置
	QString config_dir_from = caePath + "/ConfigFiles";
	if (!dir.exists(config_dir_from))
	{
		dir.mkpath(config_dir_from);
	}
	QString config_dir_to = caePath + sysPath + "configfiles/data/ConfigFiles";
	dir.mkpath(config_dir_to);
	bool result = copyDirectory(config_dir_from, config_dir_to, true);
	if (!result){
		displayInstallInfo(QString("Copy ConfilgFiles failed!"));
	}

	///<MG 拷贝求解器
	QString solver_dir_from = caePath + "/Solver";
	QString solver_dir_to = caePath + sysPath + "configfiles/data/Solver";
	dir.mkpath(solver_dir_to);
	result = copyDirectory(solver_dir_from, solver_dir_to, true);
	if (!result){
		displayInstallInfo(QString("Copy Solvers failed!"));
	}

	///<MG 拷贝用户说明文档
	QString doc_dir_from = caePath + "/Doc";
	QString doc_dir_to = caePath + sysPath + "configfiles/data/Doc";
	dir.mkpath(doc_dir_to);
	result = copyDirectory(doc_dir_from, doc_dir_to, true);
	if (!result){
		displayInstallInfo(QString("Copy Doc failed!"));
	}

	///<MG 拷贝应用
	//QString core_dir_from = caePath + "/GUI";
	QString core_dir_from = path;
	QString core_dir_to = caePath + sysPath + "fastcae/data/GUI";
	dir.mkpath(core_dir_to);
	result = copyGUIDirectory(core_dir_from, core_dir_to, true);
	if (!result){
		displayInstallInfo(QString("Copy GUI failed!"));
	}

	///<MG 拷贝Python环境
	QString python_dir_from = caePath + "/python37";
	QString python_dir_to = caePath + sysPath + "fastcae/data/python37";
	dir.mkpath(python_dir_to);
	result = copyDirectory(python_dir_from, python_dir_to, true);
	if (!result){
		displayInstallInfo(QString("Copy python37 failed!"));
	}


	///<MG 改变exe名字
	QString filePath = caePath + sysPath + "fastcae/data/GUI/FastCAE.exe";
	QFile exe(filePath);
	if (exe.exists())
	{
		exe.rename(caePath + sysPath + "fastcae/data/GUI/" + _softName + ".exe");
	}

	///<MG 运行脚本
	//QDir ctorDir(path);
	//ctorDir.cdUp();
	// 		ctorDir.cdUp();

//	QString shell = ctorDir.path() + "/binarycreator.exe";
	QString shell = path + "/binarycreator.exe";

	//qDebug() << shell;

	QStringList args;
	args.append("-c");
	args.append(configFile);
	args.append("-p");
	args.append(caePath + "/Install/packages");
	args.append(_installPath + "/" + _softName + ".exe");
	args.append("-v");
	displayInstallInfo(QString("Writing to the installation package"));


	_process->start(shell, args);

	}
		 
	void Generateinstallationpackage::slot_console_finished(int code, QProcess::ExitStatus status)
	{
		Q_UNUSED(status);
		Q_UNUSED(code);
		bGenerated = true;

		this->close();

	}
		 
	void Generateinstallationpackage::slot_console_output()
	{
		if (_process)
		{
			QString buffer = QString::fromLocal8Bit(_process->readAll());
			QStringList infos = buffer.split("\n", QString::SkipEmptyParts);
			for (int i = 0; i < infos.count(); i++)
			{
				QString info = infos.at(i);
				displayInstallInfo(info);
				QTimer::singleShot(200, this, SLOT(OnTimeOver()));
			}
		}
	}
		 
	void Generateinstallationpackage::slot_console_state_changed(QProcess::ProcessState state)
	{
		Q_UNUSED(state);
		if (state == QProcess::ProcessState::NotRunning)
		{
			QProcess * _process = qobject_cast<QProcess *>(sender());
			bGenerated = true;
			if (_process)
			{
			//	emit sig_generate_result(true, QString::fromLocal8Bit("生成成功"));
				_process->kill();
				delete _process;
				_process = nullptr;
			}
		}
	}
		 
	void Generateinstallationpackage::toConfigXML(const QString & filePath)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}

		QDomDocument doc;
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);

		QDomElement root = doc.createElement("Installer");
		doc.appendChild(root);

		QDomElement name = doc.createElement("Name");
		root.appendChild(name);
		QDomText nameText = doc.createTextNode(_softName);
		name.appendChild(nameText);

		QDomElement version = doc.createElement("Version");
		root.appendChild(version);
		QDomText versionText = doc.createTextNode(_version);
		version.appendChild(versionText);

		QDomElement title = doc.createElement("Title");
		root.appendChild(title);
		QDomText titleText = doc.createTextNode(_softName + " Installation Guide");
		title.appendChild(titleText);

		QDomElement publisher = doc.createElement("Publisher");
		root.appendChild(publisher);
		//QDomText pubText = doc.createTextNode("Diso");
		QDomText pubText = doc.createTextNode("Qianfan");
		publisher.appendChild(pubText);

		QDomElement start = doc.createElement("StartMenuDir");
		root.appendChild(start);
		QDomText startText = doc.createTextNode(_softName);
		start.appendChild(startText);

		QDomElement target = doc.createElement("TargetDir");
		root.appendChild(target);
		QDomText targetText = doc.createTextNode("@rootDir@/Program Files (x86)/" + _softName);
		target.appendChild(targetText);

		file.write(doc.toString().toStdString().c_str());
		file.close();
	}
		 
	void Generateinstallationpackage::toConfigComponent(const QString & filePath)
	{

		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}

		QDomDocument doc;
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);

		QDomElement root = doc.createElement("Package");
		doc.appendChild(root);

		QDomElement name = doc.createElement("DisplayName");
		root.appendChild(name);
		QDomText nameText = doc.createTextNode(_softName + " Configure Files , Solvers Package And User Documentation");
		name.appendChild(nameText);

		QDomElement desc = doc.createElement("Description");
		root.appendChild(desc);
		QDomText descText = doc.createTextNode(_softName + QString::fromLocal8Bit("	配置文件包是一个包含项目所有必须配置环境及依赖文件,用户求解器环境的安装包和用户说明文档，该安装包默认必须安装！"));
		desc.appendChild(descText);

		QDomElement version = doc.createElement("Version");
		root.appendChild(version);
		QDomText versionText = doc.createTextNode(_version + "-1");
		version.appendChild(versionText);

		QDomElement dataE = doc.createElement("ReleaseDate");
		root.appendChild(dataE);
		QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd");
		QDomText dataText = doc.createTextNode(time);
		dataE.appendChild(dataText);

		QDomElement defaultE = doc.createElement("Default");
		root.appendChild(defaultE);
		QDomText defaultText = doc.createTextNode("FastCAE");//script
		defaultE.appendChild(defaultText);

		QDomElement script = doc.createElement("Script");
		root.appendChild(script);
		QDomText scriptText = doc.createTextNode("installscript.qs");
		script.appendChild(scriptText);

		QDomElement force = doc.createElement("ForcedInstallation");
		root.appendChild(force);
		QDomText forceText = doc.createTextNode("true");
		force.appendChild(forceText);

		file.write(doc.toString().toStdString().c_str());
		file.close();
	}
		 
	void Generateinstallationpackage::toConfigQS(const QString & filePath)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}

		file.write("function Component() \n");
		file.write("{\n");
		file.write("}\n");

		file.write("Component.prototype.createOperations = function()\n");
		file.write("{\n");
		file.write("    component.createOperations();\n");
		file.write("}\n");
		file.close();
	}
		 
	void Generateinstallationpackage::toSoftComponent(const QString & filePath)
	{

		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}

		QDomDocument doc;
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);

		QDomElement root = doc.createElement("Package");
		doc.appendChild(root);

		QDomElement name = doc.createElement("DisplayName");
		root.appendChild(name);
		QDomText nameText = doc.createTextNode(_softName + " Soft Core Package");
		name.appendChild(nameText);

		QDomElement desc = doc.createElement("Description");
		root.appendChild(desc);
		QDomText descText = doc.createTextNode(_softName + QString::fromLocal8Bit("	软件核心安装包中包含软件的核心应用程序及其相应的dll文件，该安装包默认必须进行安装！"));
		desc.appendChild(descText);

		QDomElement version = doc.createElement("Version");
		root.appendChild(version);
		QDomText versionText = doc.createTextNode(_version + "-1");
		version.appendChild(versionText);

		QDomElement dataE = doc.createElement("ReleaseDate");
		root.appendChild(dataE);
		QDomText dataText = doc.createTextNode(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
		dataE.appendChild(dataText);

		QDomElement defaultE = doc.createElement("Default");
		root.appendChild(defaultE);
		QDomText defaultText = doc.createTextNode("script");
		defaultE.appendChild(defaultText);

		QDomElement script = doc.createElement("Script");
		root.appendChild(script);
		QDomText scriptText = doc.createTextNode("installscript.qs");
		script.appendChild(scriptText);

		QDomElement force = doc.createElement("ForcedInstallation");
		root.appendChild(force);
		QDomText forceText = doc.createTextNode("true");
		force.appendChild(forceText);

		file.write(doc.toString().toStdString().c_str());
		file.close();
	}
		 
	void Generateinstallationpackage::toSoftQS(const QString & filePath)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}

		file.write("function Component() \n");
		file.write("{\n");
		file.write("}\n");
		file.write("Component.prototype.createOperations = function()\n");
		file.write("{\n");
		file.write("try {\n");
		file.write("       component.createOperations();\n");
		file.write("            if(systemInfo.productType === \"windows\") {\n");
		file.write(QString("               component.addOperation(\"CreateShortcut\",\"@TargetDir@/GUI/%1.exe\",\"@StartMenuDir@/%1.lnk\");\n").arg(_softName).toStdString().c_str());
		file.write(QString("               component.addOperation(\"CreateShortcut\",\"@TargetDir@/GUI/%1.exe\",\"@HomeDir@/Desktop/%1.lnk\");\n").arg(_softName).toStdString().c_str());
		file.write("           }\n");
		file.write("    } catch(e) {\n");
		file.write("       console.log(e);\n");
		file.write("    }\n");
		file.write("}\n");
		file.close();
	}
		 
	bool Generateinstallationpackage::copyDirectory(const QDir & from, const QDir & to, bool cover)
	{
		QDir formDir_ = from;
		QDir toDir_ = to;
		if (!toDir_.exists())
		{
			if (!toDir_.mkdir(to.absolutePath()))
				return false;
		}
		QFileInfoList fileInfoList = formDir_.entryInfoList();
		for (QFileInfo fileInfo : fileInfoList)
		{
			if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
				continue;

			if (fileInfo.isDir())
			{
				if (!copyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), cover))
					return false;
			}
			else
			{
				if (cover && toDir_.exists(fileInfo.fileName()))
				{
					toDir_.remove(fileInfo.fileName());
				}
			//	emit sig_copy_file(toDir_.filePath(fileInfo.fileName()));
				displayInstallInfo(toDir_.filePath(fileInfo.fileName()));
				if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Generateinstallationpackage::copyGUIDirectory(const QDir & from, const QDir & to, bool cover)
	{
		QDir formDir_ = from;
		QDir toDir_ = to;
		if (!toDir_.exists())
		{
			if (!toDir_.mkdir(to.absolutePath()))
				return false;
		}
		QFileInfoList fileInfoList = formDir_.entryInfoList();
		for (QFileInfo fileInfo : fileInfoList)
		{
			if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
				continue;

			if (fileInfo.isDir())
			{
				if (!copyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), cover))
					return false;
			}
			else
			{
				QString fileSuffix = fileInfo.fileName().split(".").last();
				if ((fileSuffix != "dll") && (fileSuffix != "exe") && (fileSuffix != "ini") && (fileSuffix != "py"))
					continue;

				if (cover && toDir_.exists(fileInfo.fileName()))
				{
					toDir_.remove(fileInfo.fileName());
				}
				//	emit sig_copy_file(toDir_.filePath(fileInfo.fileName()));
				if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
				{
					return false;
				}
			}
		}
		return true;
	}
		 
	void Generateinstallationpackage::slot_console_process_error(QProcess::ProcessError)
	{
		//emit sig_generate_result(false, QString::fromLocal8Bit("发生错误！"));
		QProcess * _process = qobject_cast<QProcess *>(sender());
		if (_process)
		{
			_process->kill();
			delete _process;
			_process = nullptr;
		}
	}

	void Generateinstallationpackage::displayInstallInfo(QString info)
	{
		QString text;
		if (info.size() < 60)
			text = info;
		else
			text = "..." + info.right(57);

		ui->OutputInfomation->setText(text);
		ui->OutputInfomation->show();
		//QTimer::singleShot(200, this, SLOT(OnTimeOver()));
	}

	void Generateinstallationpackage::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}

	void Generateinstallationpackage::OnTimeOver()
	{
		ui->OutputInfomation->setText("");
		ui->OutputInfomation->hide();
	}
}