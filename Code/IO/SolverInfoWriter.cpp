#include "SolverInfoWriter.h"
#include "settings/busAPI.h"
//#include "settings/SolveOption.h"
#include <QTextStream>
#include <QDomDocument>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <sstream>
#include <fstream>
#include "xml2json.hpp"
#include <QFile>
#include "geometry/geometryData.h"
#include "meshData/meshSingleton.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "Material/MaterialSingletion.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace std;

namespace IO
{
	SolverInfoWriter::SolverInfoWriter(const QString& fileName) : IOBase(fileName)
	{
// 		_solveOption = Setting::BusAPI::instance()->getSolveOptions();
// 		if (!_solveOption->isJSON())
// 		{
// 			_isTextType = false;
// 		}

		_geoData = Geometry::GeometryData::getInstance();
		_meshData = MeshData::MeshData::getInstance();
		_modelData = ModelData::ModelDataSingleton::getinstance();
		_materialData = Material::MaterialSingleton::getInstance();
	}
	SolverInfoWriter::SolverInfoWriter()
	{
// 		_solveOption = Setting::BusAPI::instance()->getSolveOptions();
// 		if (!_solveOption->isJSON())
// 		{
// 			_isTextType = false;
// 		}
		_geoData = Geometry::GeometryData::getInstance();
		_meshData = MeshData::MeshData::getInstance();
		_modelData = ModelData::ModelDataSingleton::getinstance();
		_materialData = Material::MaterialSingleton::getInstance();
	}
// 	void SolverInfoWriter::setName(const QString filenam)
// 	{
// 		_name = filenam;
// 	}
	void SolverInfoWriter::appendModel(ModelData::ModelDataBase *model)
	{
		_modelList.append(model);
	}
// 	void SolverInfoWriter::setFileType(FileType t)
// 	{
// 		_type = t;
// 	}
	bool SolverInfoWriter::write(QString spath)
	{
//		bool isEmpty = true;
		QString filePath;
		ModelData::ModelDataBase* model = _modelList.at(0);
		if (spath.isEmpty())
		{
			QString workingdir = Setting::BusAPI::instance()->getWorkingDir();
//			isjson = Setting::BusAPI::instance()->getSolveOptions()->isJSON();
			if (workingdir.isEmpty()) return false;
			
//			QString path = model->getPath();
			filePath = model->getPath();
		}
		else
		{
			filePath = spath+"/"+model->getName();
//			isjson = true;
		}
//		qDebug() << filePath;
		
		QDir dir;
		if (!dir.exists(filePath))
			dir.mkdir(filePath);
		
		QString filename_model;
		QString filename_mesh;
		QString filename_geo;
		QString filename_material;

		filename_model = filePath + "/" + "model.xml";
		this->setFileName(filename_model);
//		qDebug() << filename_model;
		write_xml_model();
		//xml2json_model(filename_model);


		if (model->getMeshSetList().size() != 0)
		{
			_doc = nullptr;
			filename_mesh = filePath + "/" + "meshdata.xml";
			this->setFileName(filename_mesh);
			write_xml_meshdata();
			//xml2json_meshdata(filename_mesh);
		}

// 		_doc = nullptr;
// 		filename_geo = filePath + "/" + "geometry.xml";
// 		this->setFileName(filename_geo);
// 		write_xml_geometry();
		//xml2json_geometry(filename_geo);

		_doc = nullptr;
		filename_material = filePath + "/" + "material.xml";
		this->setFileName(filename_material);
		write_xml_material();
		//xml2json_material(filename_material);

		if (_modelList.size() == 1)
		{
			return writeSingleModel(); //writeSingleModel()内容暂时注释掉了
		}
		else
			return false;
	}
	void SolverInfoWriter::write_xml_model(){
		if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) return ;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
		_modelData->writeToProjectFile(_doc, &root);
		_stream = new QTextStream(&_file);
		_doc->save(*_stream, 4);
		if (isjson){
			xml2json_model(_filename);
			_file.remove();
		}
		QString filename_model_temp = _filename.remove("model.xml").toLocal8Bit();
		QString filename_model_json = filename_model_temp + "model.json";
		QFile temp_file(filename_model_json);
		if (temp_file.open(QIODevice::ReadOnly) && (!isjson)){
			temp_file.remove();
			temp_file.close();
		}
		_file.close();
	}
	void SolverInfoWriter::write_xml_material(){
		if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
		_materialData->writeToProjectFile(_doc, &root);
		_stream = new QTextStream(&_file);
		_doc->save(*_stream, 4);
		if (isjson){
			xml2json_material(_filename);
			_file.remove();
		}
		QString filename_model_temp = _filename.remove("material.xml").toLocal8Bit();
		QString filename_model_json = filename_model_temp + "material.json";
		QFile temp_file(filename_model_json);
		if (temp_file.open(QIODevice::ReadOnly) && (!isjson)){
			temp_file.remove();
			temp_file.close();
		}
		_file.close();
	}
	void SolverInfoWriter::write_xml_geometry(){
		if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
		_geoData->writeToProjectFile(_doc, &root);
		_stream = new QTextStream(&_file);
		_doc->save(*_stream, 4);
		if (isjson){
			xml2json_geometry(_filename);
			_file.remove();
		}
		QString filename_model_temp = _filename.remove("geometry.xml").toLocal8Bit();
		QString filename_model_json = filename_model_temp + "geometry.json";
		QFile temp_file(filename_model_json);
		if (temp_file.open(QIODevice::ReadOnly) && (!isjson)){
			temp_file.remove();
			temp_file.close();
		}
		_file.close();
	}
	void SolverInfoWriter::write_xml_meshdata(){
		if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
		_meshData->writeToProjectFile(_doc, &root);
		_stream = new QTextStream(&_file);
		_doc->save(*_stream, 4);
		if (isjson){
			xml2json_meshdata(_filename);
			_file.remove();
		}
		QString filename_model_temp = _filename.remove("meshdata.xml").toLocal8Bit();
		QString filename_model_json = filename_model_temp + "meshdata.json";
		QFile temp_file(filename_model_json);
		if (temp_file.open(QIODevice::ReadOnly) && (!isjson)){
			temp_file.remove();
			temp_file.close();
		}
		_file.close();
	}
	void SolverInfoWriter::xml2json_model(QString fileName){
		ifstream inf;
		std::string filename_model_xml, filename_model_temp, filename_model_json;
		QString temp;
		//从QString 到 std::string
		filename_model_xml = string((const char *)fileName.toLocal8Bit());
		filename_model_temp = string((const char *)fileName.remove("model.xml").toLocal8Bit());
		filename_model_json = filename_model_temp + "model.json";
		inf.open(filename_model_xml);
		//filename_model
		ofstream outf(filename_model_json);
		string xml_str;
		string temp_str;
		//vector<string> xml;
		while (getline(inf, xml_str)){
			//xml.push_back(xml_str);
			
			//if (xml_str.find("Accuracy") != -1) continue;
			//if (xml_str.find("Accuracy") != -1) continue;
			temp_str += xml_str;
		}
		const char* temp_xml = temp_str.c_str();
		string temp_xml2json = xml2json(temp_xml);
		const char* temp_json = temp_xml2json.c_str();

		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(temp_json, &error);
		QByteArray byteArray = document.toJson(QJsonDocument::Indented);
		QString strJson(byteArray);
		QStringList strJson_filter = strJson.split("\n");
		for (int i = 0; i < strJson_filter.size(); i++){
			if (strJson_filter.at(i).contains("Accuracy")) continue;
			if (strJson_filter.at(i).contains("Editable")) continue;
			if (strJson_filter.at(i).contains("Range")) continue;
			if (strJson_filter.at(i).contains("Type")) continue;
			if (strJson_filter.at(i).contains("Title")) continue;
			if (strJson_filter.at(i).contains("Number")) continue;
			outf << strJson_filter.at(i).toStdString().c_str() << "\n";
		}
		//outf << strJson.toStdString().c_str();
		//cout << temp_str << endl;
		inf.close();
		outf.close();
	}
	void SolverInfoWriter::xml2json_material(QString fileName){
		ifstream inf;
		std::string filename_material_xml, filename_material_temp, filename_material_json;
		QString temp;
		//从QString 到 std::string

		filename_material_xml = string((const char *)fileName.toLocal8Bit());
		filename_material_temp = string((const char *)fileName.remove("material.xml").toLocal8Bit());
		filename_material_json = filename_material_temp + "material.json";
		inf.open(filename_material_xml);
		//filename_material
		ofstream outf(filename_material_json);
		string xml_str;
		string temp_str;
		//vector<string> xml;
		while (getline(inf, xml_str)){
			//xml.push_back(xml_str);
			temp_str += xml_str;
		}
		const char* temp_xml = temp_str.c_str();
		string temp_xml2json = xml2json(temp_xml);
		const char* temp_json = temp_xml2json.c_str();

		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(temp_json, &error);
		QByteArray byteArray = document.toJson(QJsonDocument::Indented);
		QString strJson(byteArray);
		outf << strJson.toStdString().c_str();
		//cout << temp_str << endl;
		inf.close();
		outf.close();
	}
	void SolverInfoWriter::xml2json_geometry(QString fileName){
		ifstream inf;
		std::string filename_geometry_xml, filename_geometry_temp, filename_geometry_json;
		QString temp;
		//从QString 到 std::string

		filename_geometry_xml = string((const char *)fileName.toLocal8Bit());
		filename_geometry_temp = string((const char *)fileName.remove("geometry.xml").toLocal8Bit());
		filename_geometry_json = filename_geometry_temp + "geometry.json";
		inf.open(filename_geometry_xml);
		//filename_geometry
		ofstream outf(filename_geometry_json);
		string xml_str;
		string temp_str;
		//vector<string> xml;
		while (getline(inf, xml_str)){
			//xml.push_back(xml_str);
			temp_str += xml_str;
		}
		const char* temp_xml = temp_str.c_str();
		string temp_xml2json = xml2json(temp_xml);
		const char* temp_json = temp_xml2json.c_str();

		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(temp_json, &error);
		QByteArray byteArray = document.toJson(QJsonDocument::Indented);
		QString strJson(byteArray);
		outf << strJson.toStdString().c_str();
		//cout << temp_str << endl;
		inf.close();
		outf.close();
	}
	void SolverInfoWriter::xml2json_meshdata(QString fileName){
		ifstream inf;
		std::string filename_meshdata_xml, filename_meshdata_temp, filename_meshdata_json;
		QString temp;
		//从QString 到 std::string

		filename_meshdata_xml = string((const char *)fileName.toLocal8Bit());
		filename_meshdata_temp = string((const char *)fileName.remove("meshdata.xml").toLocal8Bit());
		filename_meshdata_json = filename_meshdata_temp + "meshdata.json";
		inf.open(filename_meshdata_xml);
		//filename_meshdata
		ofstream outf(filename_meshdata_json);
		string xml_str;
		string temp_str;
		//vector<string> xml;
		while (getline(inf, xml_str)){
			//xml.push_back(xml_str);
			temp_str += xml_str;
		}
		const char* temp_xml = temp_str.c_str();
		string temp_xml2json = xml2json(temp_xml);
		const char* temp_json = temp_xml2json.c_str();

		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(temp_json, &error);
		QByteArray byteArray = document.toJson(QJsonDocument::Indented);
		QString strJson(byteArray);
		outf << strJson.toStdString().c_str();
		//cout << temp_str << endl;
		inf.close();
		outf.close();
	}
	bool SolverInfoWriter::writeSingleModel()
	{
//		QString workingdir = Setting::BusAPI::instance()->getWorkingDir();
//		if (workingdir.isEmpty()) return false;
//
//		QString fileName;
//		ModelData::ModelDataBase* model = _modelList.at(0);
////		if (_solveOption->isProjectPath())
////		{
//		QString filePath = model->getPath();
//		QDir dir;
//		if (!dir.exists(filePath)) dir.mkpath(filePath);
////			QString name = _solveOption->getName();
//		QString name = "model.xml";
//		fileName = filePath + "/" + name;
////		}
//// 		else
//// 		{
//// 			QString filePath = _solveOption->getPath();
//// //			QString name = _solveOption->getName();
//// 			QString name = model->getOutputFileName();
//// 			fileName = filePath +"/" +name.remove("%");
//// 		}
//
//		this->setFileName(fileName);
//		qDebug() << fileName;
//		if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
//		_stream = new QTextStream(&_file);
//// 		if (_isTextType)
//// 		{
//// 			model->writeToSolverText(_stream);
//// 		}
//// 		else
////		{
//		_doc = new QDomDocument;
//		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
//		_doc->appendChild(instruction);
//		QDomElement root = _doc->createElement("DISO_FILE_1.0");
//		_doc->appendChild(root);
//		model->writeToSolverXML(_doc, &root);
//		_doc->save(*_stream, 4);
////		}
//
//		_file.close();
		
		return true;
	}


}