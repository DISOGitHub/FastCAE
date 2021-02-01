#ifndef _SIMLUTATIONSETTINGBASE_H_
#define _SIMLUTATIONSETTINGBASE_H_

#include "modelDataAPI.h"
#include "DataProperty/DataBase.h"

namespace ModelData
{
	class ModelDataBase;

	class MODELDATAAPI SimlutationSettingBase : public DataProperty::DataBase
	{
	public:
		SimlutationSettingBase(ModelDataBase* m);
		~SimlutationSettingBase() = default;

		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		void readDataFromProjectFile(QDomElement* e) override;

		virtual void dataToStream(QDataStream* datas) override;
	protected:
		ModelDataBase* _modelData{};
	};


}


#endif