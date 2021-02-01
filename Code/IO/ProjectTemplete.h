#ifndef PROJECTTEMPLETE_H
#define PROJECTTEMPLETE_H
#include "IOAPI.h"
#include <QStringList>

namespace IO
{
	class IOAPI ProjectTemplete
	{
	public:
		ProjectTemplete();
		~ProjectTemplete() = default;
		QStringList getTempletefromFile();
		void importTemplete(int itemID, QStringList file_text);

	};
}


#endif