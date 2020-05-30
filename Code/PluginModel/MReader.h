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

namespace Plugins
{	
	class MReader
	{
	public:
		MReader(QString& fileName);
		~MReader();
		
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



	private:
//		QString _fileName{};

		QFile* _file{};
		MeshData::MeshData* _meshData{};
		QStringList _stringBuffer{};
		QTextCodec* _codec{};

		QHash<int,int> _nodeIDIndex{};
		QHash<int, int> _elementIDIndex{};
		int mm = 0;
	};
}


#endif