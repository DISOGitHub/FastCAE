#ifndef _TEMPLATEWRITER_H_
#define _TEMPLATEWRITER_H_

#include "IOAPI.h"
#include <QString>

namespace ModelData
{
	class ModelDataBaseExtend;
}

namespace IO
{
	class IOAPI TemplateWriter
	{
	public:
		TemplateWriter(ModelData::ModelDataBaseExtend* m);
		~TemplateWriter();
		void write();

	private:
		void writeMonitorFile();
		void writePost2D();
		void writePost3D();

	private:
		ModelData::ModelDataBaseExtend* _data{};
		QString _casePath{};

	};
}


#endif
