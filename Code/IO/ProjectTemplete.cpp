#include "ProjectTemplete.h"
#include <QFileDialog>
#include <QTextCodec>
#include <ModelData/modelDataBaseExtend.h>
#include "ModelData/simulationSettingBase.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterString.h"
#include "DataProperty/ParameterBool.h"
#include "ModelData//modelDataSingleton.h"
#include <QMessageBox>
#include <QDebug>
#include <QObject>
namespace IO{
	ProjectTemplete::ProjectTemplete(){

	}
	QStringList ProjectTemplete::getTempletefromFile(){
		QStringList rtn;
		QString fileName = QFileDialog::getOpenFileName(nullptr,
			QObject::tr("Open File"), "/home/templete.dat", QObject::tr("Configuration Files (*.dat *.txt)"));
		if (fileName.isEmpty()){
			return rtn;
		}
		QFile file(fileName);
		if (!file.exists()){
			return rtn;
		}
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return rtn;
		}

		QStringList file_text;
		QTextCodec *codec;
		codec = QTextCodec::codecForName("GBK");
		while (!file.atEnd()){
			QString line_str = codec->toUnicode(file.readLine());
			if (line_str == "\n") continue;
			file_text.append(line_str);
		}

		for (int i = 0; i < file_text.size(); i++)
		{
			if (file_text.at(i).contains("!") || file_text.at(i) == "\n")
			{
				file_text.removeAt(i);
				i--;
			}
		}
		file.close();
		return file_text;
	}

	void ProjectTemplete::importTemplete(int itemID, QStringList file_text)
	{
		ModelData::ModelDataSingleton *mds = ModelData::ModelDataSingleton::getinstance();
		ModelData::ModelDataBaseExtend *mdbCase = dynamic_cast<ModelData::ModelDataBaseExtend*>(mds->getModelByID(itemID));
		ModelData::SimlutationSettingBase *mdSim = mdbCase->getSimlutationSetting();
		int sim_count = mdSim->getParameterCount();
		int node_count = mdbCase->getConfigDataCount();
		if (sim_count == 0 && node_count == 0) return;
		for (int i = 0; i < sim_count; i++){
			DataProperty::ParameterBase* dbID = mdSim->getParameterAt(i);
			for (int j = 0; j < file_text.count(); j++){
				QStringList para_temp = file_text.at(j).split(" ", QString::SkipEmptyParts);
				//qDebug() << para_temp;
				if (para_temp.count() != 3){
					QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("The format of the file that you import is wrong!"));
					return;
				}
				para_temp[0].remove("Name="), para_temp[1].remove("Type="), para_temp[2].remove("Value="), para_temp[2].remove("\n");
				if (!para_temp[0].contains("/"))
				{
					if (dbID->getDescribe() != para_temp[0]) continue;
					if (dbID->ParaTypeToString(dbID->getParaType()) == para_temp[1] && dbID->getParaType() == DataProperty::Para_Int){
						DataProperty::ParameterInt* pInt = dynamic_cast<DataProperty::ParameterInt*>(mdSim->getParameterAt(i));
						pInt->setValue(para_temp[2].toInt());
					}
					else if (dbID->ParaTypeToString(dbID->getParaType()) == para_temp[1] && dbID->getParaType() == DataProperty::Para_Double){
						DataProperty::ParameterDouble* pDouble = dynamic_cast<DataProperty::ParameterDouble*>(mdSim->getParameterAt(i));
						pDouble->setValue(para_temp[2].toDouble());
					}
					else if (dbID->ParaTypeToString(dbID->getParaType()) == para_temp[1] && dbID->getParaType() == DataProperty::Para_String){
						DataProperty::ParameterString* pString = dynamic_cast<DataProperty::ParameterString*>(mdSim->getParameterAt(i));
						pString->setValue(para_temp[2]);
					}
					else if (dbID->ParaTypeToString(dbID->getParaType()) == para_temp[1] && dbID->getParaType() == DataProperty::Para_Bool){
						DataProperty::ParameterBool* pBool = dynamic_cast<DataProperty::ParameterBool*>(mdSim->getParameterAt(i));
						if (para_temp.at(2) == "True"){
							pBool->setValue(true);
						}
						else if (para_temp.at(2) == "False"){
							pBool->setValue(false);
						}
					}
				}
				else{
					DataProperty::ParameterBase *p = mdbCase->getParameterByName(para_temp[0]);
					switch (p->getParaType())
					{
					case DataProperty::Para_Int:
					{
						DataProperty::ParameterInt* pInt = dynamic_cast<DataProperty::ParameterInt*>(p);
						pInt->setValue(para_temp[2].toInt());
						break;
					}
					case DataProperty::Para_Double:
					{
						DataProperty::ParameterDouble* pDouble = dynamic_cast<DataProperty::ParameterDouble*>(p);
						pDouble->setValue(para_temp[2].toDouble());
						break;
					}
					case DataProperty::Para_String:
					{
						DataProperty::ParameterString* pString = dynamic_cast<DataProperty::ParameterString*>(p);
						pString->setValue(para_temp[2]);
						break;
					}
					case DataProperty::Para_Bool:
					{
						DataProperty::ParameterBool* pBool = dynamic_cast<DataProperty::ParameterBool*>(p);
						if (para_temp.at(2) == "True"){
							pBool->setValue(true);
						}
						else if (para_temp.at(2) == "False"){
							pBool->setValue(false);
						}
						break;
					}
					}
				}
			}
		}
		for (int i = 0; i < node_count; i++){
			DataProperty::DataBase* dbID = mdbCase->getConfigData(i + 1);
			int count_Parameter = dbID->getParameterCount();
			for (int j = 0; j < count_Parameter; j++){
				for (int k = 0; k < file_text.count(); k++){
					QStringList para_temp = file_text.at(k).split(" ", QString::SkipEmptyParts);
					qDebug() << para_temp;
					if (para_temp.count() != 3){
						QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("The format of the file that you import is wrong!"));
						return;
					}
					para_temp[0].remove("Name="), para_temp[1].remove("Type="), para_temp[2].remove("Value="), para_temp[2].remove("\n");
					if (!para_temp[0].contains("/"))
					{
						if (dbID->getParameterAt(j)->getDescribe() != para_temp[0]) continue;
						if (dbID->getParameterAt(j)->ParaTypeToString(dbID->getParameterAt(j)->getParaType()) == para_temp[1] && dbID->getParameterAt(j)->getParaType() == DataProperty::Para_Int){
							DataProperty::ParameterInt* pInt = dynamic_cast<DataProperty::ParameterInt*>(dbID->getParameterAt(j));
							pInt->setValue(para_temp[2].toInt());
						}
						else if (dbID->getParameterAt(j)->ParaTypeToString(dbID->getParameterAt(j)->getParaType()) == para_temp[1] && dbID->getParameterAt(j)->getParaType() == DataProperty::Para_Double){
							DataProperty::ParameterDouble* pDouble = dynamic_cast<DataProperty::ParameterDouble*>(dbID->getParameterAt(j));
							pDouble->setValue(para_temp[2].toDouble());
						}
						else if (dbID->getParameterAt(j)->ParaTypeToString(dbID->getParameterAt(j)->getParaType()) == para_temp[1] && dbID->getParameterAt(j)->getParaType() == DataProperty::Para_String){
							DataProperty::ParameterString* pString = dynamic_cast<DataProperty::ParameterString*>(dbID->getParameterAt(j));
							pString->setValue(para_temp[2]);
						}
						else if (dbID->getParameterAt(j)->ParaTypeToString(dbID->getParameterAt(j)->getParaType()) == para_temp[1] && dbID->getParameterAt(j)->getParaType() == DataProperty::Para_Bool){
							DataProperty::ParameterBool* pBool = dynamic_cast<DataProperty::ParameterBool*>(dbID->getParameterAt(j));
							if (para_temp.at(2) == "True"){
								pBool->setValue(true);
							}
							else if (para_temp.at(2) == "False"){
								pBool->setValue(false);
							}
						}
					}
					else{
						DataProperty::ParameterBase *p = mdbCase->getParameterByName(para_temp[0]);
						switch (p->getParaType())
						{
						case DataProperty::Para_Int:
						{
							DataProperty::ParameterInt* pInt = dynamic_cast<DataProperty::ParameterInt*>(p);
							pInt->setValue(para_temp[2].toInt());
							break;
						}
						case DataProperty::Para_Double:
						{
							DataProperty::ParameterDouble* pDouble = dynamic_cast<DataProperty::ParameterDouble*>(p);
							pDouble->setValue(para_temp[2].toDouble());
							break;
						}
						case DataProperty::Para_String:
						{
							DataProperty::ParameterString* pString = dynamic_cast<DataProperty::ParameterString*>(p);
							pString->setValue(para_temp[2]);
							break;
						}
						case DataProperty::Para_Bool:
						{
							DataProperty::ParameterBool* pBool = dynamic_cast<DataProperty::ParameterBool*>(p);
							if (para_temp.at(2) == "True"){
								pBool->setValue(true);
							}
							else if (para_temp.at(2) == "False"){
								pBool->setValue(false);
							}
							break;
						}
						}
					}
				}
			}
		}
	}
}