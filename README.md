![Linux badge](https://img.shields.io/badge/Linux-teal)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
![QT](https://img.shields.io/badge/QT-green)

# NativeInstall
Open source program for mounting .iso files from Native Instruments, as well as keeping track of installations.
This program does not handle the .exe installations themselves, which have to be run with wine inside the filesystem.

## Install & requirements
**Tested on**: Pop_Os! 22.04 

**QT Version**: 6.5 or higher

You also need the legacy 1.8 version of native access to work with wine. 
This will not work with the newer electron-based Native Access 2.0, as it does not currently work with wine.

1. Copy the repository to your folder of choice
2. Create a build folder inside the cloned folder using 
   `mkdir build`

3. `cd build`
   
4. `make .. `

5. `make install `

To run this program, as of now it will only function properly with sudo(unsafe, so use at your own risk).
Run:

`sudo ./NativeInstall`

This program will do the task of mounting and logging the mounted files. 
The user still has to install the files inside the mounted filesystem.

## Description

![image](https://github.com/constlo/NativeInstall/assets/79052688/a6b1840a-9816-4ee6-b565-0feaf18b7fa3)

NativeInstall is an application I created to keep help of tracking installations of the Native Instruments plugins.
When using wine with Native Access, the program fails to install plugins. Instead, it downloads the .iso image from the internet,
which the user has to then install manually using the terminal.

This application does the task of mounting the files and saving the installations to the user history, which will be implemented soon.
The task left to the user is to drive the .exe files from the mounted folder using wine.

![image](https://github.com/constlo/NativeInstall/assets/79052688/0f448dd3-b6ee-4b1a-9eb2-23abdcfbc6f5)
