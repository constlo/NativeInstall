![Linux badge](https://img.shields.io/badge/Linux-teal)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
![QT](https://img.shields.io/badge/QT-green)

# NativeInstall
Open source program for mounting .iso files and keeping track of mounted filesystems.

![program](https://github.com/constlo/NativeInstall/assets/79052688/5e19b9bf-7653-459c-ba16-e7b5c0738ba4)


## Description
This program is intended for linux users who would like to use NI plugins with their music production software.

When you install plugins from Native Instruments using linux, most of the time the installation fails. 
What happens is the wine-run Native Access cannot mount the installed files to the linux system.
The installation does not fail completely, however. What remains of the install is a .iso file on the Downloads folder, which can be mounted with a few terminal commands:

1. Create a mount point for your system using
   `sudo mkdir mnt`

2. Mount the iso file to your mnt folder
    `sudo mnt -t udf {yourfilename.iso} -o unhide mnt`

3. Inside the mounted filesystem, run the installer using wine, if it exists.

This is fine for a few installs. However, when you have a large amount of installable libraries, the amount of terminal usage becomes increasingly large.
Keeping notes of the installed programs can also get quite daunting.

This program tries to solve the issue of installing many plugins by handling all of the installs inside a UI, making the progress of installing the libraries a bit faster.
It also saves info on mounted files inside a .json file.

This is a work-in-progress program and may become unusable in the future if the support for a legacy Native Access is ended.

## Install & requirements
**Tested on**: Pop_Os! 22.04 

**QT Version**: 6.5 or higher

### Build the program

1. Copy the repository to your folder of choice
2. Create a build folder inside the cloned folder using 
   `mkdir build`

3. `cd build`
   
4. `make .. `

5. `make install `

## Usage

1. Create your mount point as described in the Description:
   `sudo mkdir mnt`
2. Run the NativeInstall using
   `sudo ./NativeInstall`
3. Inside the program, choose your mount folder by pressing the button under "mount folder path"
   ![image](https://github.com/constlo/NativeInstall/assets/79052688/872a1950-09c2-46d8-8f3d-79408454e926)

4. Choose the folder your .iso files are downloaded into, and scan the folder for .iso files by pressing the "scan"-button.
   
   ![image](https://github.com/constlo/NativeInstall/assets/79052688/94fed914-8077-4bcd-99b4-d0c76e14a708)

5. You should see your plugins inside a scroll area below the "Select" and "Scan"-buttons.
   ![tuto](https://github.com/constlo/NativeInstall/assets/79052688/eeef6efe-6d3c-4fa6-9427-a95c62cc341a)
 
7. Mount the file by pressing the "Mount" - button. You will see the list on the right side of the program update as you mount your files. 

8. Open your file explorer(Nautilus, etc.) and drive the .exe installer inside the filesystem using wine.


## Development goals
See my trello board for future development:
https://trello.com/b/pVQlRQgF/n

