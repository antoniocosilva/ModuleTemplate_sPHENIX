# ModuleTemplate_sPHENIX

__IMPORTANT__: You should not clone this repository. Read the instructions. At the end, if everything is working, you can create your own repository with your new module.

In your root directory, use wget to get the script that will create you new sPHENIX analysis module.

```
wget https://raw.githubusercontent.com/antoniocosilva/ModuleTemplate_sPHENIX/main/newModule_sPHENIX.py
```
If you still don't have a intallation directory, you need to create one and export a variable pointing to your directory. In this example we are going to create a directory called __myInstall__. This will be our installation directory.

```
mkdir myInstall
```

And exporte the path to this directory in a variable

```
export MYINSTALL=$PWD/myInstall
```

Now you must load the sPHENIX environment using

```
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source $OPT_SPHENIX/bin/setup_local.sh $MYINSTALL
```

Execute the script as showed below and replace &lt;MyNewModule&gt; by the name you want for your new module.

```
python newModule_sPHENIX.py <MyNewModule>
```

This script will download all the files you need and make some changes in the original files using the name of the module you want.

The next step is to move to the directory that was created and execute the script build_and_install.py.

```
cd <MyNewModule>
python build_and_install.py -i $MYINSTALL
```

This script does a couple of things:

* Modify your Fun4All macro
* Create a build directory inside src
* Run autogen.sh inside your build directory
* Do "make install" in your build directory

If everything went well, you can now run Fun4All to see if everything looks good.

```
root Fun4All_Template.C
```

It should run 100 events from a jet production (pp at 200 GeV) and create a root file in a directory called "Output". Inside the root file you should find a histogram with the transverse momentum of tracks.

Now it's time to change the code in your .cc and .h for your new module. To recompile your code, just go to your build directory and do "make install".

Have fun!
