# ARTIK503-NodeSensors-ARIA


SAMSUNG Artik 503 settings


## Environtment Setup

1. Samsung ARTIK 503  x3
2. CryptoChip FPGA    x3
3. Temperature Sensor x1
4. IR Sensor          x1
5. Gas Sensor         X1

1. Connect all the Pins to the related Pins connection. in this case, we use SPI Connection to connect the Cryptochip FPGA to the ARTIK 503
  - Connect all the MISO, MOSI, CS, CLK, GND and 3.3V pin to each other to ensure the Cryptochip FPGA embed to the ARTIK
![SPI Connection](https://user-images.githubusercontent.com/32058105/210042054-09ae5474-cf9c-43e9-9a61-a97d7ae8d2a3.png)
     
```     
     Power             Power            Power
       |                |                 |
    ARTIK-1          ARTIK-2           ARTIK-3
       |                 |                |
  <SPI>|            <SPI>|          <SPI> |
       |                 |                |
Temperature Sensor    PIR Sensor       Gas Sensor
```   
    
   - Check the pins of the ARTIK 503 with this pinout
![MicrosoftTeams-image (5)](https://user-images.githubusercontent.com/32058105/210044112-6452a9c1-474f-4932-aa86-44ad76974e39.png)

2.  Connect the Connected CryptoChip and Artik to the PC to the PC trough USB
    
```    
    Samsung ARTIK 503 <-----> PC (USB)
```

## Open IDE (should to be open by LINUX)
    
1. go to the directory to open IDE as the administrator (using terminal)
  ![MicrosoftTeams-image (6)](https://user-images.githubusercontent.com/32058105/210038633-31a873bb-3e3d-460e-920e-d1cdcf7c1611.png)

2. open the main code (or program) --> sudo ./artik-IDE
3. change workspace to the destined directory
  ![MicrosoftTeams-image (7)](https://user-images.githubusercontent.com/32058105/210038672-ac082216-f06a-4833-9908-827dd4641e78.png)

4. change the key certificate, Wi-Fi SSID and password or etc as your need
  ![MicrosoftTeams-image (8)](https://user-images.githubusercontent.com/32058105/210038726-fc148488-09dc-46ee-a07a-3cdd599532a3.png)

5. move -nostartfiles -nodefaultlibs from makeFile  to the subdir.mk after -D__TINYARA__ in subdir.mk file line 34 and add python3 to the makefile line 57-58 in front of /media/iq/5C482E7D482E55D4/ARTIK/.... . so when the program is build by the IDE and make clean and make all at the terminal it will no cause error
  ![MicrosoftTeams-image (2)](https://user-images.githubusercontent.com/32058105/210038753-836e8970-8733-492a-8744-67e5ebf3450e.png)
6. to make sure that the program build correctly, please use terminal to build it using command at destined directory using command 'make clean' and then 'make all' (cd to the destination folder where the make file is destined, usually inside 'Release' folder)
7. make sure that all of the build from your OS and IDE are doing correctly, sometimes it told you to do the gcc or cc1 to be installed to for compiling and build the project that needs by the ARTIK IDE
8. after the Program build by by the terminal, flash the program using the Flash Program from the IDE, using flash system image.this should make the prorgram you use are flashed to the ARTIK 503
9. check the terminal to make sure your setting to the ARTIK 503 are running correctly using baudrate 115200

  ![MicrosoftTeams-image (3)](https://user-images.githubusercontent.com/32058105/210038788-b073d92f-c240-4b2a-9ab2-1a0bf5fc0f13.png)

10. after it connected, make sure it shows the log that it's been working with the ARIA crypto also sending it to the server which shows like this
  ![Screenshot 2022-12-30 151901](https://user-images.githubusercontent.com/32058105/210040642-b76b5585-e169-4836-8703-0a55384cbc13.png)

  ![screenshot success aria di node](https://user-images.githubusercontent.com/32058105/210040237-4f6a2744-1ec0-4350-909a-51dabb9b1dca.png)

  ![Screenshot 2022-12-30 151513](https://user-images.githubusercontent.com/32058105/210040409-72af6b0c-66db-4e73-b526-3de48ab8fdd1.png)
