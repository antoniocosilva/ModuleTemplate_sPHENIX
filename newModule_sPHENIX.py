import sys, os
from os import environ
import argparse
from git import Repo
import urllib.request

parser = argparse.ArgumentParser(description='Creates a new sPHENIX module')
parser.add_argument('name', type=str, default="NewModule", help='Name of the new module')

args = parser.parse_args()

def newModule_sPHENIX():
    print("Creating new module {} for sPHENIX software".format(args.name))
    #Repo.clone_from("https://github.com/antoniocosilva/ModuleTemplate_sPHENIX", args.name)

    #urllib.request.urlretrieve("https://github.com/antoniocosilva/ModuleTemplate_sPHENIX/blob/main/src/ModuleTemplate_sPHENIX.cc", "ModuleTemplate_sPHENIX.cc")
    myCurrentPath = os.getcwd()
    mainDir = "{}/{}".format(myCurrentPath, args.name)
    os.makedirs("{}".format(mainDir), exist_ok=True)
    os.makedirs("{}/src".format(mainDir), exist_ok=True)
    dlList = ["autogen.sh", "configure.ac", "Makefile.am", "ModuleTemplate_sPHENIX.cc", "ModuleTemplate_sPHENIX.h", "ModuleTemplate_sPHENIXLinkDef.h"]
    for f in range(len(dlList)):
        urllib.request.urlretrieve("https://github.com/antoniocosilva/ModuleTemplate_sPHENIX/blob/main/src/"+dlList[f], mainDir + "/src/" + dlList[f])
    filePath = myCurrentPath + "/" + args.name + "/src/"
    changeFiles = ["ModuleTemplate_sPHENIX.cc", "ModuleTemplate_sPHENIX.h", "ModuleTemplate_sPHENIXLinkDef.h"]
    newFiles = [args.name + ".cc", args.name+ ".h", args.name + "LinkDef.h"]
    for f in range(len(changeFiles)):
        os.rename(filePath+changeFiles[f], filePath+newFiles[f])
    search_text = "ModuleTemplate_sPHENIX"
    replace_text = args.name
    filePathAndName = args.name + "/src/configure.ac"
    fileList = ["configure.ac", "Makefile.am", args.name + ".cc", args.name+ ".h", args.name + "LinkDef.h"]
    for f in range(len(fileList)):
        with open(filePath+fileList[f], 'r') as file:
            data = file.read()
            data = data.replace(search_text, replace_text)
            data = data.replace(search_text.lower(), replace_text.lower())
            data = data.replace(search_text.upper(), replace_text.upper())
        with open(filePath+fileList[f], 'w') as file:
            file.write(data)
newModule_sPHENIX()