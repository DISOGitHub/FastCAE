#include "GeoCommandList.h"
#include "GeoCommandBase.h"

namespace Command
{
	GeoComandList* GeoComandList::_instance = nullptr;

	GeoComandList* GeoComandList::getInstance()
	{
		if (_instance == nullptr)
			_instance = new GeoComandList;
		return _instance;
	}

	void GeoComandList::undo()
	{
		if (_sketchMode)
		{
			if (_sketchUndoList.size() < 1) return;
			GeoCommandBase* command = _sketchUndoList.last();
			command->undo();
			_sketchUndoList.removeLast();
			_sketchRedoList.append(command);
		}
		else
		{
			if (_undoList.size() < 1) return;
			GeoCommandBase* command = _undoList.last();
			command->undo();
			_undoList.removeLast();
			_redoList.append(command);
		}
		
	}

	void GeoComandList::redo()
	{
		if (_sketchMode)
		{
			if (_sketchRedoList.size() < 1) return;
			GeoCommandBase* command = _sketchRedoList.last();
			command->redo();
			_sketchRedoList.removeLast();
			_sketchUndoList.append(command);
		}
		else
		{
			if (_redoList.size() < 1) return;
			GeoCommandBase* command = _redoList.last();
			command->redo();
			_redoList.removeLast();
			_undoList.append(command);
		}
		
	}

	bool GeoComandList::executeCommand(GeoCommandBase* c)
	{
		 bool success = c->execute();
		 
		 if (success)
		 {
			 if (c->getComamndType() == D3Feature) _undoList.append(c);
			 else _sketchUndoList.append(c);
		 }
		 else
			 delete c;
		 return success;
	}

	void GeoComandList::clearRedoCommands()
	{
		const int n = _redoList.size();
		for (int i = 0; i < n; ++i)
		{
			Command::GeoCommandBase* c = _redoList.at(i);
			c->releaseResult();
			delete c;
		}
		_redoList.clear();
	}

	void GeoComandList::clearAllCommands()
	{
		clearRedoCommands();
		clearSketchRedoCommands();
		int n = _undoList.size();
		for (int i = 0; i < n; ++i)
		{
			Command::GeoCommandBase* c = _undoList.at(i);
			delete c;
		}
		_undoList.clear();
		n = _sketchUndoList.size();
		for (int i = 0; i < n; ++i)
		{
			Command::GeoCommandBase* c = _sketchUndoList.at(i);
			c->releaseResult();
			delete c;
		}
		_sketchUndoList.clear();
	}

	void GeoComandList::startSketchMode(bool s)
	{
		_sketchMode = s;
	}

	void GeoComandList::clearSketchRedoCommands()
	{
		const int n = _sketchRedoList.size();
		for (int i = 0; i < n; ++i)
		{
			Command::GeoCommandBase* c = _sketchRedoList.at(i);
			c->releaseResult();
			delete c;
		}
		_sketchRedoList.clear();
	}

	void GeoComandList::clearSketchUndoCommands()
	{
		const int n = _sketchUndoList.size();
		for (int i = 0; i < n; ++i)
		{
			Command::GeoCommandBase* c = _sketchUndoList.at(i);
			c->releaseResult();
			delete c;
		}
		_sketchUndoList.clear();
	}

	QList<GeoCommandBase*> GeoComandList::getSketchUndoList()
	{
		return _sketchUndoList;
	}

}