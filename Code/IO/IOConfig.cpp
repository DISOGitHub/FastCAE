#include <IOConfig.h>

namespace IO
{
	QHash<QString, WRITEINPFILE> IOConfigure::_inpWriteFun = QHash<QString, WRITEINPFILE>();
	QHash<QString, TRANSFEROUTFILE> IOConfigure::_outFileTransfer = QHash<QString, TRANSFEROUTFILE>();
	QHash<QString, IMPORTMESHFUN> IOConfigure::_inputmeshFuns = QHash<QString, IMPORTMESHFUN>();
	QHash<QString, EXPORTMESHFUN> IOConfigure::_exportMeshFuns = QHash<QString, EXPORTMESHFUN>();

	QStringList IOConfigure::getInputFileFormat()
	{
		QList<QString> fs = _inpWriteFun.keys();
		return QStringList(fs);
	}

	QStringList IOConfigure::getOutputFileTransfers()
	{
		QList<QString> ts = _outFileTransfer.keys();
		return QStringList(ts);
	}

	void IOConfigure::RegisterInputFile(QString suffix, WRITEINPFILE fun)
	{
		_inpWriteFun.insert(suffix, fun);
	}

	void IOConfigure::RegisterOutputTransfer(QString name, TRANSFEROUTFILE fun)
	{
		_outFileTransfer.insert(name, fun);
	}

	void IOConfigure::RemoveInputFile(QString s)
	{
		_inpWriteFun.remove(s);
	}

	void IOConfigure::RemoveOutputTransfer(QString name)
	{
		_outFileTransfer.remove(name);
	}

	WRITEINPFILE IOConfigure::getInputFileWriter(QString format)
	{
		if (_inpWriteFun.contains(format))
			return _inpWriteFun.value(format);
		return nullptr; 
	}

	TRANSFEROUTFILE IOConfigure::getOutputTransfer(QString tras)
	{
		if (_outFileTransfer.contains(tras))
			return _outFileTransfer.value(tras);
		return nullptr;
	}

	void IOConfigure::RegisterMeshImporter(QString suffix, IMPORTMESHFUN fun)
	{
		_inputmeshFuns.insert(suffix, fun);
	}

	void IOConfigure::RemoveMeshImporter(QString suffix)
	{
		if (_inputmeshFuns.contains(suffix))
			_inputmeshFuns.remove(suffix);
	}

	QStringList IOConfigure::getMeshImporters()
	{
		QList<QString> ts = _inputmeshFuns.keys();
		return QStringList(ts);
	}

	IMPORTMESHFUN IOConfigure::getMeshImporter(QString name)
	{
		if (_inputmeshFuns.contains(name))
			return _inputmeshFuns.value(name);
		return nullptr;
	}

	void IOConfigure::RegisterMeshExporter(QString suffix, EXPORTMESHFUN fun)
	{
		_exportMeshFuns.insert(suffix, fun);
	}

	void IOConfigure::RemoveMeshExporter(QString suffix)
	{
		if (_exportMeshFuns.contains(suffix))
			_exportMeshFuns.remove(suffix);
	}

	QStringList IOConfigure::getMeshExporters()
	{
		QList<QString> ts = _exportMeshFuns.keys();
		return QStringList(ts);
	}

	EXPORTMESHFUN IOConfigure::getMeshExporter(QString suffix)
	{
		if (_exportMeshFuns.contains(suffix))
			return _exportMeshFuns.value(suffix);
		return nullptr;
	}
}



