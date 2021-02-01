#ifndef GEOCOMMANDLIST_H_
#define GEOCOMMANDLIST_H_

#include "geometryCommandAPI.h"
#include <QList>


namespace Command
{
	class GeoCommandBase;

	class GEOMETRYCOMMANDAPI GeoComandList
	{
	public:
		static GeoComandList* getInstance();
		void undo();
		void redo();

		bool executeCommand(GeoCommandBase* c);
		void startSketchMode(bool s);
		void clearRedoCommands();
		void clearSketchRedoCommands();
		void clearSketchUndoCommands();
		void clearAllCommands();
		QList<GeoCommandBase*> getSketchUndoList();


	private:
		GeoComandList() = default;
		~GeoComandList() = default;
	
	private:
		static GeoComandList* _instance;
		bool _sketchMode{ false };

		QList<GeoCommandBase*> _undoList{};
		QList<GeoCommandBase*> _redoList{};

		QList<GeoCommandBase*> _sketchUndoList{};
		QList<GeoCommandBase*> _sketchRedoList{};

	};


}


#endif