import shutil
MainWindow.clearData()
examplePath = "%examplesPath%"
dirPath = examplePath + "/../../"

desC = dirPath + "ConfigFiles"
dess = dirPath + "solver"

if os.path.exists(desC): shutil.rmtree(desC)
if os.path.exists(dess): shutil.rmtree(dess)

shutil.copytree(examplePath + "/ConfigFiles/", dirPath + "ConfigFiles/")
shutil.copytree(examplePath + "/solver/", dirPath + "solver/")

MainWindow.updateInterface()

MainWindow.openProjectFile("%examplesPath%/M99.diso")
MainWindow.solveProject(0,0) 
