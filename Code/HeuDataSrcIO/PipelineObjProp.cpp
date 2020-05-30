#include "PipelineObjProp.h"


PipelineObjProp::PipelineObjProp()
{	
	pipelineObj_base_propData.flag_grid_blocks = 0;
	pipelineObj_base_propData.flag_show_actors = true;
	pipelineObj_base_propData.count_total_child = 0;
	pipelineObj_base_propData.obj_id = 0;
	pipelineObj_base_propData.count_clip = 0;
	pipelineObj_base_propData.count_slice = 0;
	pipelineObj_base_propData.count_contour = 0;
	pipelineObj_base_propData.count_vector = 0;
	pipelineObj_base_propData.count_reflection = 0;
    pipelineObj_base_propData.count_calculator = 0;
	pipelineObj_base_propData.count_smooth = 0;
	pipelineObj_base_propData.count_streamLine = 0;

	pipelineObj_base_propData.flag_rectilinear = false;
	pipelineObj_base_propData.fileName = "";
	pipelineObj_base_propData.file_xh = 0;
	pipelineObj_base_propData.scalarColorRange[0] = 0.667;
	pipelineObj_base_propData.scalarColorRange[1] = 0;	
	pipelineObj_base_propData.mSolidColor.r = 195;
	pipelineObj_base_propData.mSolidColor.g = 195;
	pipelineObj_base_propData.mSolidColor.b = 195;
	pipelineObj_base_propData.mScalarBarVisible = false;
	pipelineObj_base_propData.m_enum_representationtype = ERepresentationType::kkSurface;
	pipelineObj_base_propData.mColorColumnStyle = "SolidColor";//传递过来选择列名
	pipelineObj_base_propData.mPointCount = 0;
	pipelineObj_base_propData.mCellCount = 0;
	pipelineObj_base_propData.mPointSize=2;
	pipelineObj_base_propData.mLineWidth=1;	 
	//颜色相关
	//edge color
	pipelineObj_base_propData.mEdgeColor.r = 0;
	pipelineObj_base_propData.mEdgeColor.g = 0;
	pipelineObj_base_propData.mEdgeColor.b = 255;
	pipelineObj_base_propData.m_enum_interpolationtype = EInterpolationType::kFlat;
	pipelineObj_base_propData.mSpecular=0;
	pipelineObj_base_propData.mSpecularColor.r=255;
	pipelineObj_base_propData.mSpecularColor.g = 255;
	pipelineObj_base_propData.mSpecularColor.b = 255;
	pipelineObj_base_propData.mSpecularPower=100;
	pipelineObj_base_propData.mAmbient=0;
	//scolor mAmbientColor;
	pipelineObj_base_propData.mDiffuse=1;
	//scolor mDiffuseColor;
	pipelineObj_base_propData.m_struct_edgecolor.r=0;
	pipelineObj_base_propData.m_struct_edgecolor.g = 0;
	pipelineObj_base_propData.m_struct_edgecolor.b = 128;
	pipelineObj_base_propData.m_struct_transforming.x = 0;
	pipelineObj_base_propData.m_struct_transforming.y = 0;
	pipelineObj_base_propData.m_struct_transforming.z = 0;
	pipelineObj_base_propData.m_struct_scale.x=1;
	pipelineObj_base_propData.m_struct_scale.y = 1;
	pipelineObj_base_propData.m_struct_scale.z = 1;
	pipelineObj_base_propData.m_struct_origin.x=0;
	pipelineObj_base_propData.m_struct_origin.y = 0;
	pipelineObj_base_propData.m_struct_origin.z = 0;
	pipelineObj_base_propData.m_struct_orientation.x=0;
	pipelineObj_base_propData.m_struct_orientation.y = 0;
	pipelineObj_base_propData.m_struct_orientation.z = 0;
	pipelineObj_base_propData.mOpacity=1;

	//add for animate displacement
	aniDisplace_propData.flag_AniDisplaceFile = false;
	aniDisplace_propData.path_AniDisplaceFile = " ";
	aniDisplace_propData.m_aniDisplaceList.clear();
	aniDisplace_propData.flag_exeDisplace = false;
	aniDisplace_propData.cur_pos = 0;
	aniDisplace_propData.start_pos = 0;
	aniDisplace_propData.interval = 1;
	aniDisplace_propData.zoom_factor = 1.0;
	aniDisplace_propData.dis_xh = 0;
	aniDisplace_propData.m_pre_displace.time = 0;
	aniDisplace_propData.m_pre_displace.Heave_GZ = 0;
	aniDisplace_propData.m_pre_displace.Pitch_Gyb = 0;
	aniDisplace_propData.m_pre_displace.Roll_Gxb = 0;
	aniDisplace_propData.m_pre_displace.Surge_GX = 0;
	aniDisplace_propData.m_pre_displace.Sway_GY = 0;
	aniDisplace_propData.m_pre_displace.Yaw_Gzb = 0;
	aniDisplace_propData.m_cur_displace.time = 0;
	aniDisplace_propData.m_cur_displace.Heave_GZ = 0;
	aniDisplace_propData.m_cur_displace.Pitch_Gyb = 0;
	aniDisplace_propData.m_cur_displace.Roll_Gxb = 0;
	aniDisplace_propData.m_cur_displace.Surge_GX = 0;
	aniDisplace_propData.m_cur_displace.Sway_GY = 0;
	aniDisplace_propData.m_cur_displace.Yaw_Gzb = 0;
	aniDisplace_propData.m_origin.x = 0;
	aniDisplace_propData.m_origin.y = 0;
	aniDisplace_propData.m_origin.z = 0;
	aniDisplace_propData.flag_inputGravity = false;
	//add for mooringLine
	mooringLine_propData.glyph_data.type = -1;
	mooringLine_propData.glyph_data.scaleFactor = 0.2;
	mooringLine_propData.glyph_data.arrow_data.tipRes = 6;
	mooringLine_propData.glyph_data.arrow_data.tipRad = 0.08;
	mooringLine_propData.glyph_data.arrow_data.tipLen = 0.35;
	mooringLine_propData.glyph_data.arrow_data.shaftRes = 6;
	mooringLine_propData.glyph_data.arrow_data.shaftRad = 0.08;

	mooringLine_propData.glyph_data.cone_data.Res = 6;
	mooringLine_propData.glyph_data.cone_data.Rad = 0.1;
	mooringLine_propData.glyph_data.cone_data.Hei = 0.6;
	mooringLine_propData.glyph_data.cone_data.glyph_center.x = 0;
	mooringLine_propData.glyph_data.cone_data.glyph_center.y = 0;
	mooringLine_propData.glyph_data.cone_data.glyph_center.z = 0;
	mooringLine_propData.glyph_data.cone_data.glyph_direction.x = 1;
	mooringLine_propData.glyph_data.cone_data.glyph_direction.y = 0;
	mooringLine_propData.glyph_data.cone_data.glyph_direction.z = 0;

	mooringLine_propData.glyph_data.cylinder_data.Res = 6;
	mooringLine_propData.glyph_data.cylinder_data.Rad = 0.1;
	mooringLine_propData.glyph_data.cylinder_data.Hei = 0.1;
	mooringLine_propData.glyph_data.cylinder_data.glyph_center.x = 0;
	mooringLine_propData.glyph_data.cylinder_data.glyph_center.y = 0;
	mooringLine_propData.glyph_data.cylinder_data.glyph_center.z = 0;
	mooringLine_propData.flag_seaBottom = true;
	mooringLine_propData.seaBottomColor.r = 88;
	mooringLine_propData.seaBottomColor.g = 255;
	mooringLine_propData.seaBottomColor.b = 255;
	//add for mooringLine

	//particles
	particles_propData.type = 0;
	particles_propData.radius = 0.1;
	particles_propData.max_radius = particles_propData.radius;
	particles_propData.min_radius = particles_propData.radius;
	particles_propData.resolution = 10;
	//combine model
	pipelineObj_base_propData.flag_comModel = false;
	pipelineObj_base_propData.comModel_fileName = "";
	//cube axes
	pipelineObj_base_propData.flag_cubeAxes = false;
}

QString PipelineObjProp::GetRepresentationType()
{
	QString RepresentionStr;
	switch (pipelineObj_base_propData.m_enum_representationtype)
	{
	case ERepresentationType::k3DGlyphs:
		RepresentionStr = "3D Glyphs";
		break;
	case ERepresentationType::kOutLine:
		RepresentionStr = "OutLine";
		break;
	case ERepresentationType::kPointGaussian:
		RepresentionStr = "Point Gaussian";
		break;
	case ERepresentationType::kkPoints:
		RepresentionStr = "Points";
		break;
	case ERepresentationType::kkSurface:
		RepresentionStr = "Surface";
		break;
	case ERepresentationType::kkSurfaceWithEdges:
		RepresentionStr = "Surface With Edges";
		break;
	case ERepresentationType::kVolume:
		RepresentionStr = "Volume";
		break;
	case ERepresentationType::kWIreframe:
		RepresentionStr = "Wireframe";
		break;
	default:
		RepresentionStr = "Surface";
		break;
	}
	return RepresentionStr;
}

PipelineObjProp::~PipelineObjProp()
{
	
}

void PipelineObjProp::slt_set_FilterSlice_plane(double origins[3], double normals[3])
{	
	plane_propData.m_plane_origin.x = origins[0];
	plane_propData.m_plane_origin.y = origins[1];
	plane_propData.m_plane_origin.z = origins[2];
	plane_propData.m_plane_normal.x = normals[0];
	plane_propData.m_plane_normal.y = normals[1];
	plane_propData.m_plane_normal.z = normals[2];
}
