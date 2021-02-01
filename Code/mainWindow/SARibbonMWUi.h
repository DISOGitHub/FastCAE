#ifndef SARIBBONMWUI_H
#define SARIBBONMWUI_H

#include "ui_mainWindow.h"

class SARibbonCategory;
class SARibbonPannel;
class SARibbonContextCategory;
class SARibbonMainWindow;

namespace Ui
{
	class MainWindowRibbon : public MainWindow
	{
	public:
		//每个pannel相当于一个toolbar，pannel的命名方式是“第一个action_pannel”
		SARibbonCategory* home_page{};
		SARibbonPannel* new_pannel{};
		SARibbonPannel* solve_pannel{};
		SARibbonPannel* import_mesh_pannel{};
		SARibbonPannel* import_geometry_pannel{};
		SARibbonPannel* exec_script_pannel{};
		SARibbonPannel* adapt_window_pannel_home{};
		SARibbonPannel* save_picture_pannel{};
		SARibbonPannel* plugin_manager_pannel{};
		SARibbonPannel* drawing_option_pannel_home{};

		SARibbonCategory* geometry_page{};
		SARibbonPannel* sketch_pannel_geometry{};
		SARibbonPannel* cube_pannel{};
		SARibbonPannel* create_point_pannel{};
		SARibbonPannel* base_plate_pannel{};
		SARibbonPannel* sum_pannel{};
		SARibbonPannel* stretch_pannel{};
		SARibbonPannel* display_surface_pannel{};
		SARibbonPannel* clear_choice_pannel_geometry{};
		SARibbonPannel* segmentation_pannel{};
		SARibbonPannel* chamfer_pannel{};
		SARibbonPannel* measure_pannel{};
		SARibbonPannel* undo_redo_pannel_geometry{};

		SARibbonCategory* mesh_page{};
		SARibbonPannel* surface_mesh_subdivision_pannel{};
		SARibbonPannel* mesh_quailty_examine_pannel{};
		SARibbonPannel* clear_choice_pannel_mesh{};
		SARibbonPannel* display_node_pannel{};
		SARibbonPannel* create_set_pannel{};

		SARibbonCategory* solve_page{};
		SARibbonPannel* solver_management_pannel{};

		SARibbonCategory* window_page{};
		SARibbonPannel* start_page_pannel{};
		SARibbonPannel* adapt_window_pannel_window{};
		SARibbonPannel* drawing_option_pannel_window{};

		SARibbonCategory* help_page{};
		SARibbonPannel* user_guide_pannel{};

		SARibbonContextCategory* sketch_tool_context{};
		SARibbonCategory* sketch_tool_page{};
		SARibbonPannel* sketch_pannel_sketch_tool{};
		SARibbonPannel* draw_line_pannel{};
		SARibbonPannel* undo_redo_pannel_sketch{};
		
		void setupRibbonUi(QMainWindow *MainWindow);
		void retranslateRibbonUi(QMainWindow *MainWindow);
	};
}
#endif