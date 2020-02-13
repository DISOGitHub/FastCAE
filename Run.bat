cd Code

call Copy_DLLs.bat
cd ../
cd ../
cd Code/

call Copy_Pys.bat

call Create_X64_Project.bat

cd ../

call Run_MSVC.bat
