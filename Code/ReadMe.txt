/****************************************************
Author: Li Baojun
Time:   2017/08/24
Description: Some Info about develop envoirment
*****************************************************/

1.为保证不污染个人PC端的环境变量，所有的路径配置均采用*bat命令进行；

2.对于公共的开发环境，如VS Qt等同一项目组最好能够保征安装路径的一致，以便实现一键编译；

3.本目录下的Frame_Code文件夹未源代码文件夹，需要纳入SVN版本控制，其他为编译的临时文件，不需要版本控制；

4.本目录下的output文件夹为编译的输出结果，包含 moc obj lib dll exe等文件；

5.内部每个目录下的*.pro文件配置了该工程的属性，*.pri文件负责配置头文件、源文件和ui文件；

6.内部每个目录下的Create_Project.bat的作用是通过pro文件的信息创建*.vcxproj文件；

7.Create_All_Project.bat创建了所有的vc工程；

8.添加文件时通过修改pri文件添加，修改工程配置通过修改pro文件，修改完成之后重新运行bat即可生效；

9.本机的VS安装路径为 D:\Program Files (x86)\Microsoft Visual Studio 12.0\ 
			 D:\program\vs\	
Qt的安装路径为E:\Qt\Qt5.5.1\5.5\msvc2013_64\bin （qmake.exe所在路径）;
		 D:\Qt\Qt5.4.2\5.4\msvc2013_opengl\bin

10.如上述路径不一致，修改bat的相应路径即可，但是请不要上传和更新bat文件，以免影响他人；

11.启动VS时，请通过Run_MSVC.bat启动，否则可能编译出错。

12.以Linux开头命名的文件均为linux编译使用批处理，Linux版本为Ubuntu 14.0.4

/********************************************************************
  MG：该项目所使用库为：VTK7.0.1 

32位版本配置说明：

1.修改vtklib.pri 文件（当前projectDefineUI moduleBase mainWindow IOBase 中均使用了vtk，都有vtklib.pri文件，且包含文件当前相同）中：

1.1修改INCLUDEPATH中vtk的头文件路径

1.2修改Release:LIBS中所添加的vtk lib库路径。

2.运行Create_x86_Projects.bat批处理文件。

3.打开CAEFrame_x86.sln文件，进行编译。

如果想要同时配置32 / 64 位需在vs中手动配置，qmake当前不支持同时对windows下的32位及64位同时进行配置。

*********************************************************************/