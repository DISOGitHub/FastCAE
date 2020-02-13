#ifndef NUMERICSRCOBJECT_H
#define NUMERICSRCOBJECT_H
#include "global_define.h"
#include "PipelineObject.h"
#include "heudatasrcio_global.h"
#include <qstringlist.h>
#include "PipelineObject.h"
#include "PipelineObjProp.h"
#include "qmap.h"
#include "qobject.h"
#include "global_define.h"
#include "ReadDataIO.h"

#include <vtkAutoInit.h>
#include "vtkActor.h"
#include "vtkActorCollection.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkCellType.h"
#include "vtkCellData.h"
#include "vtkMapper.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "vtkDataSetMapper.h"
#include "vtkStructuredGrid.h"
#include "vtkGlyph3D.h"
#include "vtkReflectionFilter.h"
#include "vtkContourFilter.h"
#include "vtkDataArray.h"
#include "vtkProperty.h"
#include "vtkAxesActor.h"
#include "vtkCubeAxesActor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkArrowSource.h"
#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
class  HEUDATASRCIOSHARED_EXPORT NumericSrcObject :
	public PipelineObject
{
public:
	bool readFile(QString tep_filename);
	void delDataSet();
	void setColorList();
	NumericSrcObject(QStringList dataFileList,int fileXh);
	~NumericSrcObject();
	const QString& GetDataFilePath() const;
	const QStringList& GetAllDataFiles() const { return mDataFileList; }
	virtual Def_PipelineObjType GetObjectType() const { return dNumeric_DataSource; }
	void generate_actors();
	void  ChangeColumnColor();
	void ChangeColumnColorWithArray(vtkSmartPointer<vtkFloatArray> tep_data);
	void ChangeColumnColor_MooringLine();
	bool get_flag_generate_scalarBarWidget(){ return flag_generate_scalarBarWidget; };
	void set_flag_generate_scalarBarWidget(bool flag){ flag_generate_scalarBarWidget = flag; };
	ReadDataIO *readdata_io;
	
	double refrence_point_for_obj[3];
	vtkIdType refrence_point_for_objID;
	double objModel_pos[3];
	void addSeaBottom(double r, double g, double b);
public:
private:
	bool flag_generate_scalarBarWidget;
	Scalar_Vector_NameListData scalar_vector_list;
		
	std::string   mColorName;
	Def_ColorMode mColorMode;
	Def_DataAttributeType mColorFieldType;	
	Def_CellType mCellType;
	char * strDefaultColumnName;
	int curGridIndex = 0;
	int mFileXh;
	QStringList mDataFileList;
	QString mDataFile;
	bool scalarVisibility = false;	
	bool MakeParticles();
	//mooringLine
	bool MakeGlyphs();
	vtkSmartPointer<vtkPolyData> pointData;
	vtkSmartPointer<vtkArrowSource> mArrowSource;
	vtkSmartPointer<vtkConeSource> mConeSource;
	vtkSmartPointer<vtkGlyph3D> glyphFilter;
	vtkSmartPointer<vtkPolyDataMapper> glyph_map;
	vtkSmartPointer<vtkCylinderSource> mCylinderSource;
	vtkSmartPointer<vtkDoubleArray> mooringLine_normals;

	QVector<ScalarColor_Data> init_LookUpTable_Value(vtkFloatArray* tep_array);
	QVector<ScalarOpacity_Data> init_LookUpTable_Opacity(vtkFloatArray* tep_array);
	bool func_replace_vtkFloatArray(vtkSmartPointer<vtkFloatArray> src_data, vtkFloatArray* des_data);
	void setParticlesRadiusRange(vtkDataSet* tep_dataSet);
protected:
	
	
};
inline const QString& NumericSrcObject::GetDataFilePath() const
{
	return mDataFile;
};
#endif
