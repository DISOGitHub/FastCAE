#ifndef DATAMANGER_H
#define DATAMANGER_H

#include <QObject>
#include "ConfigOptions\ConfigOptions.h"
#include "ConfigOptions\GlobalConfig.h"

namespace FastCAEDesigner
{
	class DataManager : public QObject
	{
		Q_OBJECT
	public:
		explicit DataManager(QObject *parent = 0);

	private:
		static DataManager* _instance;
	public:
		static DataManager* getInstance();


	public:
		bool ReadInfoFromServerToLocal();
		bool WriteInfoToServerPath();

		ConfigOption::GlobalConfig getBasicInfo();
		void setBasicInfo(ConfigOption::GlobalConfig basic);

		/*
		GeometryInformation getGeometryInfo();
		void setGeometryInfo(GeometryInformation geometry);

		GridInformation getGridInfo();
		void setGridInfo(GridInformation grid);

		SolverInformation getSolverInfo();
		void setSolverInfo(SolverInformation solver);

		PostInformation getPostInfo();
		void setPostInfo(PostInformation post);
		*/
	private:
		
		ConfigOption::GlobalConfig _basic;
		//GeometryInformation _geometry;
		//GridInformation _grid;
		//SolverInformation _solver;
		//PostInformation _post;

		bool ReadBasicInfo();
	};
}

#endif // DATAMANGER_H
