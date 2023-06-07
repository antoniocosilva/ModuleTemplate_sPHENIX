import sys, os, stat
from os import environ
import argparse
import subprocess

parser = argparse.ArgumentParser(description='Creates a new sPHENIX module')
parser.add_argument('-i', '--installDir', type=str, default="MyInstall", help='Name of the intallation directory')

args = parser.parse_args()

userRootDir = os.path.expanduser('~')
userInstallDir = "{}".format(args.installDir)
os.makedirs(userInstallDir, exist_ok=True)

ModulePath = os.getcwd()

moduleName = os.path.basename(os.path.normpath(ModulePath))

search_text = "ModuleTemplate_sPHENIX"
replace_text = moduleName

with open(ModulePath+"/Fun4All_Template.C", 'r') as file:
    data = file.read()
    data = data.replace(search_text, moduleName)
    data = data.replace(search_text.lower(), moduleName.lower())
    data = data.replace("/path_to_user_install", userInstallDir)
with open(ModulePath+"/Fun4All_Template.C", 'w') as file:
    file.write(data)

os.chdir("src")

os.makedirs("build", exist_ok=True)

os.chdir("build")

print(os.getcwd())

#subprocess.call(['sh', ModulePath + "/src/autogen.sh --prefix=" + userInstallDir])
chmod = "chmod +x " + ModulePath + "/src/autogen.sh"
autogen = ModulePath + "/src/autogen.sh --prefix=" + userInstallDir
os.system(chmod)
os.system(autogen)
os.system("make install")
