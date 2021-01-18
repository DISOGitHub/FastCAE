#ifndef _SOLVERSETTINGBASE_H_
#define _SOLVERSETTINGBASE_H_

#include "modelDataAPI.h"
#include "DataProperty/DataBase.h"

namespace ModelData
{
	class ModelDataBase;

	class  MODELDATAAPI SolverSettingBase : public DataProperty::DataBase
	{
	public:
		SolverSettingBase(ModelDataBase* m);
		~SolverSettingBase() = default;

		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
//		void writeToProjectFile1(QDomDocument* doc, QDomElement* parent) override;

	
	protected:
		ModelDataBase* _modelData{};
	};

}


#endif