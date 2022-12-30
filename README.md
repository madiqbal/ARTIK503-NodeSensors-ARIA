# ARTIK503-NodeSensors-ARIA


SAMSUNG Artik 503 settings

Open IDE (should to be open by LINUX)



1. go to the directory to open IDE as the administrator (using terminal)
2. open the main code (or program) --> sudo ./artik-IDE
3. change workspace to the destined directory
4. change the key certificate, Wi-Fi SSID and password or etc as your need
5. move -nostartfiles -nodefaultlibs from makeFile  to the subdir.mk after -D__TINYARA__ in subdir.mk file line 34 and add python3 to the makefile line 57-58 in front of /media/iq/5C482E7D482E55D4/ARTIK/.... . so when the program is build by the IDE and make clean and make all at the terminal it will no cause error
6. to make sure that the program build correctly, please use terminal to build it using command at destined directory using command 'make clean' and then 'make all' (cd to the destination folder where the make file is destined, usually inside 'Release' folder)
7. make sure that all of the build from your OS and IDE are doing correctly, sometimes it told you to do the gcc or cc1 to be installed to for compiling and build the project that needs by the ARTIK IDE
8. after the Program build by by the terminal, flash the program using the Flash Program from the IDE, using flash system image.this should make the prorgram you use are flashed to the ARTIK 503
9. check the terminal to make sure your setting to the ARTIK 503 are running correctly using baudrate 115200
