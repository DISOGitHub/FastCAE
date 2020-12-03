#include <QString>
#include "GenerateMacFile.h"
#include "DataManager.h"
#include "Geant4DataModel.h"
#include <QFile>

namespace Geant4
{
	GenerateMacFile::GenerateMacFile()
	{
//		generate();
	}

	GenerateMacFile::~GenerateMacFile()
	{

	}

	void GenerateMacFile::write(QString f)
	{
		generate(f);
	}

	void GenerateMacFile::generate(QString macFile)
	{
		auto dataModel = DataManager::getInstance()->getGeant4DataModel();
//		QString macFile = dir+ "/MyMac.mac";
		QString gdml = dataModel->getGDMLFile();

		_G4SourceType sourceType = dataModel->getSourceType();
		_G4AngleDistribution sourceAngele = dataModel->getAngleDistribution();
		_G4SourceParameters sourceParas = dataModel->getSourceParameters();
		_G4PhysicalPart physicalPart = dataModel->getPhysicalPart();

		/* Select GDML Geometry */
		QString contents;
		contents += ("/gras/geometry/type gdml\r\n");
		contents += ("/gdml/file " + gdml + "\r\n");
		contents += ("/gdml/setup Default\r\n");

		/* Source Define */
		contents += ("/gps/particle " + _G4_PARTICLE_TYPE_ENUM_STRINGLIST_.value(sourceParas.particleType) + "\r\n");
		contents += ("/gps/pos/type " + _G4_LOCATION_ENUM_STRINGLIST_.value(sourceType.location) + "\r\n");
		contents += ("/gps/pos/shape " + _G4_SHAPE_ENUM_STRINGLIST_.value(sourceType.shape) +"\r\n");
		contents += ("/gps/pos/radius " + QString::number(sourceType.radius) + "." + _G4_UNIT_ENUM_STRINGLIST_.value(sourceType.unit) + "\r\n");
		contents += ("/gps/pos/centre " + QString::number(sourceType.position.x) + " " + QString::number(sourceType.position.y) + " " + QString::number(sourceType.position.z) + " " + _G4_UNIT_ENUM_STRINGLIST_.value(sourceType.unit) + "\r\n");

		contents += ("/gps/ang/type " + _G4_DIRECTION_DISTRIBUTION_ENUM_STRINGLIST_.value(sourceAngele.directionDistribution) + "\r\n");
		contents += ("/gps/ang/mintheta " + QString::number(sourceAngele.anXitaMin) + " degree\r\n");
		contents += ("/gps/ang/maxtheta " + QString::number(sourceAngele.anXitaMax) + " degree\r\n");

		contents += ("/gps/ene/type " + _G4_UNKNOWN_ENUM_STRINGLIST_.value(sourceParas.inputList) + "\r\n");
		if (sourceParas.spectrumType == EeqE0)
		{
			contents += ("/gps/ene/ezero " + QString::number(sourceParas.E0) + "\r\n");
		}
		else
		{
			contents += ("/gps/ene/ezero " + QString::number(sourceParas.powerLow) + "\r\n");
		}

		contents += ("/gps/ene/min " + QString::number(sourceParas.Emin) + "MeV\r\n");
		contents += ("/gps/ene/max " + QString::number(sourceParas.Emax) + "MeV\r\n");

		/* Select Physical List */
		contents += ("/gras/physics/addPhysics " + _G4_ELECTROMAGNETIC_PHYSICS_ENUM_STRINGLIST_.value(physicalPart.ePhy) + "\r\n");
		contents += ("/gras/physics/addPhysics " + _G4_HADRONIC_PHYSICS_ENUM_STRINGLIST_.value(physicalPart.hPhy) + "\r\n");
		contents += ("/gras/physics/addPhysics " + _G4_OTHER_PHYSICS_ENUM_STRINGLIST_.value(physicalPart.oPhy) + "\r\n");
		contents += ("/gras/physics/addPhysics " + _G4_ION_ION_ENUM_STRINGLIST_.value(physicalPart.ion) + "\r\n");
		contents += ("/gras/physics/setCuts " + QString::number(physicalPart.eIgnore) + "mm\r\n");

		QFile file(macFile);
		if (!file.exists())
		{
			if (file.open(QIODevice::WriteOnly))
			{
				file.write(contents.toLocal8Bit().data());
			}
			file.close();
		}
	}
}

