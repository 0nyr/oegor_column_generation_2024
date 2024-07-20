# Instructions

## Setup guide (for Windows users)

1. Download and install the free Community Version of Visual Studio. Make sure to select “Desktop development with C++” for installation. https://visualstudio.microsoft.com/de/downloads/
2. Download and install CPLEX (free academic license). https://community.ibm.com/community/user/ai-datascience/blogs/xavier-nodet1/2020/07/09/cplex-free-for-students
3. Locally save the folder “cg_testproject” from Seafile. Open the file “CMakeLists.txt” and adapt CPLEX_VERSION and CPLEX_ROOT_DIR according to your installation.
4. Start Visual Studio. Use the "Open a local folder" option and navigate to your local “cg_testproject” folder.
5. Select “TestProject.exe” as Startup Item. You should now be ready to compile and run the code. Try by hitting F5.

### Install logs from CPLEX 2210

```shell
 ❮onyr ★ onyr-kenzae❯ ❮cplex2210❯❯ ./cplex_studio2210.linux_x86_64.copy.bin 
Preparing to install
Extracting the JRE from the installer archive...
Unpacking the JRE...
Extracting the installation resources from the installer archive...
Configuring the installer for this system's environment...

Launching installer...

===============================================================================
Choose Locale...
----------------

    1- Deutsch
  ->2- English
    3- Español
    4- Français
    5- Português  (Brasil)

CHOOSE LOCALE BY NUMBER: 2
===============================================================================
IBM ILOG CPLEX Optimization Studio 22.1.0        (created with InstallAnywhere)
-------------------------------------------------------------------------------

Preparing CONSOLE Mode Installation...




===============================================================================
Introduction
------------

InstallAnywhere will guide you through the installation of IBM ILOG CPLEX 
Optimization Studio 22.1.0.

It is strongly recommended that you quit all programs before continuing with 
this installation.

Respond to each prompt to proceed to the next step in the installation.  If 
you want to change something on a previous step, type 'back'.

You may cancel this installation at any time by typing 'quit'.

PRESS <ENTER> TO CONTINUE: 



===============================================================================


 
 
    LICENSE INFORMATION
    
    The Programs listed below are licensed under the following License
    Information terms and conditions in addition to the Program license
    terms previously agreed to by Client and IBM. If Client does not have
    previously agreed to license terms in effect for the Program, the
    International Program License Agreement (i125-3301-15) applies.
    
    Program Name (Program Number):
    IBM ILOG CPLEX Optimization Studio Developer Edition 22.1 (5725-A06)
    IBM ILOG CPLEX Optimizer Deployment Edition 22.1 (5725-A06)
    IBM ILOG CPLEX Optimizer for Non-Production 22.1 (5725-A06)
    IBM ILOG CPLEX Optimizer Single User Edition 22.1 (5725-A06)
    IBM CPLEX Optimizer Deployment Entry Edition 22.1 (5725-A06)
    IBM ILOG CPLEX MILP add-on Developer Edition 22.1 (5724-Y48)
    IBM ILOG CPLEX Developer Edition 22.1 (5724-Y48)
    IBM ILOG CPLEX LP Developer Edition 22.1 (5724-Y48)
    IBM ILOG CP Optimizer Developer Edition 22.1 (5724-Y49)
    IBM ILOG OPL CPLEX Developer Edition 22.1 (5724-Y54)
 
Press Enter to continue viewing the license agreement, or enter "1" to 
   accept the agreement, "2" to decline it, "3" to print it, or "99" to go back
   to the previous screen.: 1
 



===============================================================================
Choose Install Folder
---------------------

Where would you like to install?

  Default Install Folder: /opt/ibm/ILOG/CPLEX_Studio221

ENTER AN ABSOLUTE PATH, OR PRESS <ENTER> TO ACCEPT THE DEFAULT
      : /home/onyr/cplex2210/CPLEX_Studio221

INSTALL FOLDER IS: /home/onyr/cplex2210/CPLEX_Studio221
   IS THIS CORRECT? (Y/N): Y



===============================================================================
Ready To Install
----------------

InstallAnywhere is now ready to install IBM ILOG CPLEX Optimization Studio 
22.1.0 onto your system at the following location:

   /home/onyr/cplex2210/CPLEX_Studio221

PRESS <ENTER> TO INSTALL: 



===============================================================================
Pre-Installation Summary
------------------------

Please Review the Following Before Continuing:

Product Name:
    IBM ILOG CPLEX Optimization Studio 22.1.0

Install Folder:
    /home/onyr/cplex2210/CPLEX_Studio221

Product Version
    22.1.0

Disk Space Information (for Installation Target): 
    Required:    1,989,295,285 Bytes
    Available: 215,363,928,064 Bytes

PRESS <ENTER> TO CONTINUE: 



===============================================================================
Installing...
-------------

 [==================|==================|==================|==================]
 [------------------|------------------|------------------|-----------------


===============================================================================
Prepare env

Please Wait
-----------
-]



===============================================================================


Please Wait
-----------



===============================================================================


Please Wait
-----------



===============================================================================
Installing the engines in your Python environment
-------------------------------------------------

If you want to use the CPLEX or CP Optimizer engines through their Python 
APIs, you need to tell Python where to find them.  To do so, enter the 
following command into a terminal :
<<<<<<
python /home/onyr/cplex2210/CPLEX_Studio221/python/setup.py install
>>>>>>

PRESS <ENTER> TO CONTINUE: 



===============================================================================
Improve the product
-------------------

Do you agree to let IBM collect product usage data (including installation, 
starting the IDE, creating a model, solving it) to improve your user 
experience?

No model content or proprietary data will be sent.

    1- Yes
    2- No

ENTER THE NUMBER OF THE DESIRED CHOICE: 2




===============================================================================
Installation Complete
---------------------

IBM ILOG CPLEX Optimization Studio 22.1.0 has been successfully installed to:

   /home/onyr/cplex2210/CPLEX_Studio221

PRESS <ENTER> TO EXIT THE INSTALLER: 

```