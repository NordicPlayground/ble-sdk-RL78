Bluetooth low energy SDK for Renesas RL78
=========================================

Port of the Bluetooth low energy SDK for Arduino to the Renesas RL78. 
This uses the Nordic Semiconductor nRF8001 chip which is embedded on the nRF2740 board. The nRF2740 board is a compact module that has all the required hardware for the nRF8001 chip. At the same time the nRF2740 board was tested using the nRF8001/nRF24L01+ shield adapter for Arduino. 
This adapter was connected to the Renesas demo board [YRPBRL78G13 (RL78/G13)](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/index.jsp). The programming IDE used is IAR Embedded Workbench for Renesas RL78 version 1.40.1. It can be downloaded from the [IAR website](http://www.iar.com/Products/IAR-Embedded-Workbench/Renesas-RL78/).


The nRF2740 board and the nRF8001/nRF24L01+ shield adapter for Arduino are part of the nRF8001 Development kit. Alternatively you can use an [nRF8001 breakout board](http://www.adafruit.com/products/1697) or [shield](http://www.seeedstudio.com/depot/bluetooth-40-low-energy-ble-shield-v20-p-1631.html). 
For further details about the nRF8001 chip, the nRF2740 board and the nRF8001/nRF24L01+ shield adapter for Arduino please refer to the [Nordic Semiconductor website](http://www.nordicsemi.com).

This port is backward compatible with Arduino, but the file `hal_platform.cpp` has to be deleted. In case you want to use the BLE library with an Arduino it is recommended to use the original repo: [ble-sdk-arduino](https://github.com/NordicSemiconductor/ble-sdk-arduino)

Contents of this repository
----------------------

The folder `libraries` contain three subfolders.

* [BLE](#BLE)
* [Examples](#Examples)
* [RL78](#RL78)

### BLE

This folder contains all the `*.cpp` and `*.h` files required for the BLE SDK to work

### Examples

This folder contains several subfolders, each subfolder corresponds to a different example for the BLE library. Initially each folder will only contain a `main.c` file but after configuring IAR some other folders will appear.

### RL78

This folder contains all the `*.cpp` and `*.h` files required for porting the BLE SDK to the RL78. The current files are configured for the Renesas demo board YRPBRL78G13 (RL78/G13), so in case another board will be used small changes should be done corresponding to that type of RL78 board.


Quick start guide
---------------

1. Connect the nRF8001/nRF24L01+ shield adapter to YRPBRL78G13

  | YRPBRL78G13 <br> (J2 and J1 headers)  | nRF24L01+  <br> (P10 and P11 headers) |
  |:-------------------------|:----------------------|
  |**J1 pin1**   GND         |**P11 pin2**  GND      |
  |**J1 pin2**   VDD         |**P11 pin4**  5V       |
  |**EXTERNAL**  3.3V        |**P11 pin5**  3.3V     |
  |**J1 pin3**   p10/SCK00   |**P10 pin8**  ACI_SCLK |
  |**J1 pin4**   p11/SI00    |**P10 pin7**  ACI_MISO |
  |**J1 pin5**   p12/SO00    |**P10 pin6**  ACI_MOSI |
  |**J2 pin3**   p141        |**P10 pin5**  ACI_REQN |
  |**J2 pin4**   p140        |**P10 pin3**  ACI_RESET|
  |**J2 pin26**  p147        |**P10 pin2**  ACI_RDYN |

  There should be connected an external 3.3V source to the nRF24L01+ as unfortunately the YRPBRL78G13 does not provide any 3.3V output. 
  The 3.3V voltage is needed so the shield can convert the 5V logic from the YRPBRL78G13 to the needed 3.3V logic of the nRF2740 and vice versa.
  Remember to interconnect all grounds from the nRF24L01+, YRPBRL78G13 and the external voltage source.

  A recommendation is to attach a 3.3V voltage regulator between the YRPBRL78G13 and the shield. The input of the voltage regulator has to be connected to any pin that has VDD(5V) from the YRPBRL78G13, and the output has to be connected to the 3.3V pin of the nRF24L01+ shield.

2. Connect the serial output of the YRPBRL78G13 to a computer/serial reader device.

  The YRPBRL78G13 has a serial output implemented for communicating with an external device. This serial output transmits the same debug and function information as the serial monitor for Arduino implementation of the BLE library.

  The characteristics of the Serial interface are
  * 115 200 bps
  * 8 data bits
  * 1 stop bit
  * No parity
  * Transmission of the LSB first

  The pins that have to be connected are the next

  | YRPBRL78G13 <br> (J1 header) |
  |:-----------------------:|
  |**J1 pin7**  P02/TxD1    |
  |**J1 pin8**  P02/RxD1    |

3. Jumper configuration on YRPBRL78G13.

  The demo board YRPBRL78G13 has several jumpers that have to be set on specific positions so the BLE examples work properly. For further details about the jumpers functions please refer to the [RL78/G13 Stick (YRPBRL78G13)](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/Documentation.jsp) document.

  | Jumpers | Configuration |
  |:-------:|:-------------:|
  |   J3    |  put a jumper |
  |   J6    |      1-2      |
  |   J7    |      1-2      |
  |   J8    |      1-2      |
  |   J9    |      1-2      |

4. IAR workspace creation for all examples.

  To create a new workspace where all the example projects can be added you have to do the following steps.
  * Open IAR Embedded Workbench for Renesas RL78
  * Go to menu `File` -> `New` -> `Workspace`. 

  Now a new workspace will be created and you can add the example projects to it. Having a workspace is quite handy as you can switch easily from one example to another example.

5. Adding a new example and configuring it.

  Each example should be contained on a separate project. To create a new project and configure it do the next steps.
  * In the recently created workspace in the previous step go to menu `Project` -> `Create new project...`. A new window will open where you have to select:
     * **Toolchain:** RL78
     * **Project Template:** Empty project
  * Click `OK`
  * A new windows will open where you have to save the project file. Go to the corresponding example file, from which you want to create the project and name the project as the name of the example.
  * Click `Save`

  For example if you are creating a new project for the example `ble_aci_transport_layer_verification` go to the location `libraries\Examples\ble_aci_transport_layer_verification` and save the project file with the name `ble_aci_transport_layer_verification.ewp`. This is just a recommendation but you can arrange the example project however is more convenient for you.

  A new project was created and now you can add the corresponding files to it. First you have to add the `main.c` file of the example.

  * On the project view (Left panel of the IDE) right click on the newly created project and select `Add` -> `Add Files..`
  * A new window should open where you can select the corresponding `main.c` file.
  * Click `Open` and now the file should be opened on the project.

  Now you have to add the BLE files and the RL78 files. 
  * Right click on the project name and select `Add` -> `Add Group`.
  * A new window should open where you can write the group name. 
  * Once you have written the group name click on `OK`.
  
  We should create two groups, **BLE** and **RL78_common**. The first group will contain all the files of the BLE library and the second one will contain all the hardware specific files for the RL78. After creating both groups you have to add the corresponding files to each one.

  * Right click on the group **BLE** and select `Add` -> `Add Files..`.
  * A new window should open where you select all the files that should be added. You can add both `*.cpp` and `*.h` files or just `*.cpp` files. Probably the project will look less messy if you only add `*.cpp`.
  * Go to the folder `\libraries\BLE` and select all the files if you want to add them all or just the `*.cpp` files.
  * Click `Open`.
  * Now right click on the group **RL78_common** and select `Add` -> `Add Files..`.
  * A new window should open where you select all the files that should be added. You can add both `*.cpp` and `*.h` files or just `*.cpp` files. Probably the project will look less messy if you only add `*.cpp`.
  * Go to the folder `\libraries\RL78` and select all the files if you want to add them all or just the `*.c` files.
  * Click `Open`.

  Now you have to change the properties of the project so it can compile correctly.

  * Right click on the project name and select `Options..`. A new window should open where all the options of the project will appear.
  * On the Category `General Options` select:
     * Tab `Target`:
         * `Device` Browse to select `RL78 G13` -> `RL78 - R5F100LE`
     * Tab `Library Configuration`:
         * `Library` Select `Full DLib`
     * Tab `Stack/Heap`:
         * `Stack size (bytes)` : 256
  * On the Category `C/C++ Compiler` select:
     * Tab `Optmizations`:
         * `Level` : Low.
     * Tab `Preprocessor`:
        * `Additional include directories` add two lines, `$PROJ_DIR$\..\..\RL78\` and `$PROJ_DIR$\..\..\BLE\`
  * On the Category `Linker` select:
     * Tab `List`:
         * Tick `Generate Linker list`.
  * On the Category `Debugger` select:
     * Tab `Setup`:
         * `Driver` : TK.
  
  Now that all the options have been configured go to menu `Project` -> `Make` or you can simply press F7. The project will be build.

  In case IAR ask you to save the workspace go to the folder `\libraries\Examples` and save the workspace as `RL78_Examples` or whatever other name you wish.

6. Debugging an example.

  For debugging a project go to menu `Project` -> `Download and Debug` or click on the menu button with a green arrow that says `Download and debug`. The first time that you debug a project it will appear a window saying:

  > Emulator has to be configured before downloading a new application.

  > Press `OK` to enter emulator Hardware Setup.

  You have to click `OK` and a window specifying all the emulator configuration parameters will appear. Do not modify any parameter and just click `OK`. Now the program will be downloaded to the YRPBRL78G13 board and the debugging session will start.

  Once on the debugging session you can see the code running. The different actions for executing the code can be found on the menu `Debug`.

  Other features are the use of breakpoints and the monitoring of variables. For further details please refer to the [IAR documentation](http://www.iar.com/Products/IAR-Embedded-Workbench/Renesas-RL78/User-guides/) about it.

7. Flash the YRPBRL78G13 and use it as a standalone board

  In case you want to program the YRPBRL78G13 and let it run at will without the need of having it connected to IAR, first you have to program the board using the previous [jumper configuration](Jumper-configuration-on-YRPBRL78G13) and then remove jumpers J6, J7, J8 and J9 completely.

  For programming the board from IAR first you have to `Make` [F7 key] the project and the go to Menu `Project` -> `Download` -> `Download active application`. Once the board was programmed then you have to remove the jumpers.

  Now you can use the board as a standalone device without needing it to be connected to IAR. Remember that the YRPBRL78G13 needs a 5V power source to work and the nRF24L01 shield needs 5V and 3.3V to work.



Tools
-----

[nRFgo Studio for developing GATT Clients and Services for nRF8001](http://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRFgo-Studio)



References
----------

[RL78/G13 Stick(YRPBRL78G13) manual](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/Documentation.jsp)

[RL78/G13 Stick(YRPBRL78G13) quick guide](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/Documentation.jsp)

[RL78/G13 User's Manual: Hardware](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/Documentation.jsp)

[RL78/G13 Datasheet](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/Documentation.jsp)

[Promotion software installation package for YRPBRL78G13](http://am.renesas.com/products/tools/introductory_evaluation_tools/renesas_promo_board/yrpbrl78g13/downloads.jsp)

[Renesas design support website](http://renesasrulz.com/app_kits_and_demo_boards/rpbrl78g13/)

[IAR Embedded Workbench IDE Project Management and Building Guide](http://www.iar.com/Products/IAR-Embedded-Workbench/Renesas-RL78/User-guides/)

[IAR C/C++ Compiler Reference Guide for the Renesas RL78 Microcontroller Family](http://www.iar.com/Products/IAR-Embedded-Workbench/Renesas-RL78/User-guides/)

[Porting the nRF8001 SDK for Arduino (0.9.0) to your chosen microcontroller](https://github.com/NordicSemiconductor/ble-sdk-arduino/blob/master/documentation/libraries/BLE/nRF8001-Porting-ACI-Library.md)

[nRF8001 datasheet](https://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF8001)
    
[Webinars and FAQ when getting started](https://devzone.nordicsemi.com/question/4933/write-services-to-nrf-8001/?answer=4974#post-id-4974)
    
