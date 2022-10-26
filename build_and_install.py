import sys, os
from os import environ
import argparse
import subprocess

parser = argparse.ArgumentParser(description='Creates a new sPHENIX module')
parser.add_argument('-i', '--installDir', type=str, default="MyInstall", help='Name of the intallation directory')

args = parser.parse_args()

userRootDir = os.path.expanduser('~')
userInstallDir = "{}/{}".format(userRootDir,args.installDir)
os.makedirs(userInstallDir, exist_ok=True)

myCurrentPath = os.getcwd()

moduleName = os.path.basename(os.path.normpath(myCurrentPath))

search_text = "ModuleTemplate_sPHENIX"
replace_text = moduleName

with open(myCurrentPath+"/Fun4All_Template.C", 'r') as file:
    data = file.read()
    data = data.replace(search_text, moduleName)
    data = data.replace(search_text.lower(), moduleName.lower())
    data = data.replace("/path_to_user_install", userInstallDir)
with open(myCurrentPath+"/Fun4All_Template.C", 'w') as file:
    file.write(data)

buildDir = myCurrentPath + "/src/build"

print(buildDir)

os.makedirs(buildDir, exist_ok=True)

os.chdir(buildDir)

print(myCurrentPath)

#subprocess.call(['sh', myCurrentPath + "/src/autogen.sh --prefix=" + userInstallDir])
autogen = myCurrentPath + "/src/autogen.sh --prefix=" + userInstallDir
os.system(autogen)
os.system("make install")
