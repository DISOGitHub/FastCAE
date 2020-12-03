#ifndef _INPFREADER_H__
#define _INPFREADER_H__

#include <QString>
#include <QStringList>
#include <QHash>

class QFile;
class vtkPolyData;
class QTextCodec;

namespace MeshData
{
	class MeshData;
	class MeshSet;
}

namespace ThermalFluid
{	
	class TFCouplingModelData;
	class TFMaterial;
	class ThermalData;
	class BoundaryTabTypeData;

	class INPFReader
	{
	public:
		INPFReader(QString& fileName, TFCouplingModelData* data);
		~INPFReader();
		
		bool read();

	private:	
		static double Val( const QString &rStr );
		static QString Mid( const QString &rStr, const int position, const int size = -1 );
		static QString Str( QString &rStr, const int index );
		static int InStr( const int position, const QString& rStr, const QString &s ); 

		QString readLine();

		QString readPart1();
		QString readPart2();
		QString readPart3();
		//读取网格节点
		QString readPart4(vtkPolyData*);
		//读取网格单元
		QString readPart5(vtkPolyData*); 
		QString readPart6(); 
		QString readPart7(); 
		QString readPart8();
		QString readPart9(int kid);
		QString readPart10();

		void readMeshGroups(int kid);
		QString readMeshGroup(int kid, QString firstLine);
		void readRevGroup();
		
		void readThickness(QStringList linelist);
		void readMaterial(TFMaterial* tm, QString line);
		void readThermalBoundary(QStringList linelist);
		void readControlTemperatur(QStringList linelist, int id);
		void readTabTypeData(BoundaryTabTypeData* td);


	private:
//		QString _fileName{};
		TFCouplingModelData* _modelData{};
		ThermalData* _thermalData{};

		QFile* _file{};
		MeshData::MeshData* _meshData{};
		QStringList _stringBuffer{};
		QTextCodec* _codec{};

		QHash<int, int> _nodeIDIndex{};
		QHash<int, int> _elementIDIndex{};		
	};
}


#endif