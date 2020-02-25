# FastCAE源码下载与编译说明（源码编译必读）
源码下载：
两种方式可以下载源码，直接下载和使用git克隆
1. 直接下载，从GitHub直接一次性打包下载全部源码。
2. git克隆，使用git克隆代码仓库，后续可以不断拉取更新。

文件夹结构
1. Code    FastCAE源码
2. OCC     OpenCASCADE7.2.0编译的第三方库，适用于windows平台
3. VTK       VTK7.1.0编译的第三方库，适用于windows平台
4. output  程序的编译输出
5. cgns      cgns第三方库，适用于windows平台
6. quazip   压缩文件的第三方库

编译过程
1. 第一次编译需要运行 Run.bat，生成适应于本机环境的批处理脚本
2. 以后启动Visual Studio IDE运行Code文件夹下的Run_MSVC.bat

重要批处理说明：
1.
