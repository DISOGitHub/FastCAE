# FastCAE源码下载与编译说明（源码下载编译必读）

#特别注意：
1. 源码绝对路径中不要出现中文字符;
2. 源码文件夹中不要出现空格(){}*/?|\等特殊字符
3. 要求Windows7以上的64位系统
4. 要求自行安装Visual Studio 2013（with update5）和Qt5.4.2 

#源码下载：
两种方式可以下载源码，直接下载和使用git克隆
1. 直接下载，从GitHub直接一次性打包下载全部源码。
2. git克隆，使用git克隆代码仓库，后续可以不断拉取更新。

#文件夹结构
1. Code    FastCAE源码
2. OCC     OpenCASCADE7.2.0编译的第三方库，适用于windows平台
3. VTK     VTK7.1.0编译的第三方库，适用于windows平台
4. output  程序的编译输出
5. cgns     cgns第三方库，适用于windows平台
6. quazip   压缩文件的第三方库

#编译过程
1. 第一次编译需要运行 Run.bat，生成适应于本机环境的批处理脚本
2. 以后启动Visual Studio IDE运行Code文件夹下的Run_MSVC.bat

#重要批处理说明：
1. Run.bat                 下载完源码之后在本机配置生成开发环境，在一台机器上正确运行一次即可，若中间出现错误可重复运行
2. Create_X64_Project.bat  将Qt管理的工程文件转化为VS的工程文件，当工程设置出现修改的时候（*.Pro*.pri修改）需要重新运行
3. Copy_DLLs.bat           将程序运行必需的dll拷贝到运行目录下
4. Copy_Pys.bat            将程序运行必需的python脚本拷贝到运行目录
5. Run_MSVC.bat            启动VS，加载工程文件
6. 每个模块的代码文件夹下都有一个_Create_Project.bat，作用与Create_X64_Project.bat相同，区别在于各模块只是更新各自的VS工程文件

#帮助文档
http://www.fastcae.com/document.html

#社区论坛
http://disc.fastcae.com/forum.php

#Linux版本地址
https://github.com/DISOGitHub/FastCAE_Linux

