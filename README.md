# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## NXP EasyEVSE iMXRT106x 


EasyEVSE is a simulated electric vehicle charging station, J1772 and ISO 15118-2/-20 compliant, connected to a CitrineOS OCPP server over Ethernet or Wi-Fi communication. The provided application is available in two flavors. One is the basic enablement, available on GitHub, while the full enablement can be found on [nxp.com](https://www.nxp.com/design/design-center/development-boards-and-designs/easyevse-ev-charging-station-development-platform-mcu-rtos:CONNECTED-EV-CHARGING-STATION). <br />Basic enablement provides:<ul><li>Graphical UI using LVGL and NXP GUI Guider tool</li><li>Network connectivity using either Ethernet or Wi-Fi<br /></li><li>OCPP server connectivity using security profile 1, 2 and 3</li><li>EVSE-SIG-BRD2X/SIGBRD-HPGP (on EV side) and EVSE-EMETER & SIGBRD-HPGP (on EVSE side) add-on development boards communication for `J1772` vehicle charger standard</li></ul>Full enablement provides the following additional functionalities:<br /><ul><li>Integration with Sevenstax `ISO15118-2` and `ISO15118-20` high level charging protocol using the HomePlug Green PHY (HPGP) transceiver (Lumissil IS32CG5317)</li><li>Integration with NXP CLRC663 NFC for external identification method, including MIFARE DUOX support</li></ul>

The EasyEVSE purpose is to deliver both EV and EVSE reference applications based on NXP kits.

![plot](./images/aaa-065241.svg)


Documentation, videos and more can be found at [EasyEVSE](https://www.nxp.com/design/design-center/development-boards-and-designs/easyevse-ev-charging-station-development-platform-mcu-rtos:CONNECTED-EV-CHARGING-STATION).
#### Boards: MIMXRT1064-EVK, MIMXRT1060-EVK, MIMXRT1060-EVKB, MIMXRT1060-EVKC, EVSE-EMETER, SIGBRD-HPGP, EVSE-SIG-BRD2X
#### Categories: Industrial, User Interface, Networking, Cloud Connected Devices, HMI, RTOS, Wireless Connectivity
#### Peripherals: DISPLAY, DMA, ETHERNET, FLASH, GPIO, I2C, SDIO, SDMMC, SPI, UART, USB
#### Toolchains: MCUXpresso IDE, ARM GCC/MCUXpresso for VS Code

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4) 
5. [Limitations](#step5)
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software<a name="step1"></a>
* [MCUXpresso IDE V25.6 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE) or VS Code with MCUXpresso extension
* SDK_2_16_000_EVK-MIMXRT1064 for the i.MXRT1064 project
* SDK_2_16_100_EVK-MIMXRT1060 for the i.MXRT1060 project

![plot](./images/aaa-065086.svg)

## 2. Hardware<a name="step2"></a>
Mandatory hardware:
* [MIMXRT1064-EVK](https://www.nxp.com/part/MIMXRT1064-EVK) or [MIMXRT1060-EVK](https://www.nxp.com/part/MIMXRT1060-EVK) or [MIMXRT1060-EVKB](https://www.nxp.com/part/MIMXRT1060-EVKB) or [MIMXRT1060-EVKC](https://www.nxp.com/part/MIMXRT1060-EVKC) on EV side
* [MIMXRT1060-EVK](https://www.nxp.com/part/MIMXRT1060-EVK) or [MIMXRT1060-EVKB](https://www.nxp.com/part/MIMXRT1060-EVKB) or [MIMXRT1060-EVKC](https://www.nxp.com/part/MIMXRT1060-EVKC) on EVSE side
* [OM-SE050ARD-E](https://www.nxp.com/part/OM-SE050ARD-E)
* [EVSE-EMETER](https://www.nxp.com/design/design-center/development-boards-and-designs/EVSE-EMETER?tid=vanEVSE-EMETER) and [SIGBRD-HPGP](https://www.nxp.com/design/design-center/development-boards-and-designs/EVSE-SIG-BRD) on EVSE side
* [EVSE-SIG-BRD2X](https://www.nxp.com/part/EVSE-SIG-BRD2X) or [SIGBRD-HPGP](https://www.nxp.com/design/design-center/development-boards-and-designs/EVSE-SIG-BRD) on EV side
* [RK043FN66HS-CTG](https://www.nxp.com/part/RK043FN66HS-CTG) or [RK043FN02H](https://www.nxp.com/design/design-center/development-boards-and-designs/RK043FN02H-CT) display

Optional hardware boards:
* [Murata Type 1XK: 802.11a/b/g/n Wi-Fi + Bluetooth 5.2](https://www.nxp.com/webapp/connect/displayPartnerProfile.sp?partnerId=8220&offeringId=31602) Module
* [CLEV6630ARD](https://www.nxp.com/part/CLEV6630ARD) NFC Reader Module

Full list of evaluation kits and miscellaneous needed can be found in the [User Guide](https://www.nxp.com/doc/CCEVCPGSUG).

## 3. Setup<a name="step3"></a>


### 3.1 Step 0: Prerequisites

#### Setup EVSE-EMETER

Before starting to use the EVSE software, clone the repositories for [KM35 metrology MCU](https://github.com/nxp-appcodehub/dm-nxp-km-metrology) and [MCXN947 MCU](https://github.com/nxp-appcodehub/rd-evse-emeter) on EVSE-EMETER and setup the device.

#### Setup EVSE-SIG-BRD2X or SIGBRD-HPGP
Before starting to use the EV software, clone the [EVSE-SIG-BRD2X software](https://github.com/nxp-appcodehub/dm-evse-sigbrd1x-ev-software-on-lpc5536) or [SIGBRD-HPGP software](https://github.com/nxp-appcodehub/dm-evse-sigbrd-hpgp), depending on available hardware, and then setup the device.

#### Setup CitrineOS OCPP server (CSMS)

EasyEVSE is a development platform that enables you to develop and run a simulated electric vehicle charging station managed by an open source Charging Station Management System (CSMS), CitrineOS. To set up your CitrineOS CSMS, follow the instructions in the *Charging Station Management System (CSMS) Installation and Configuration User Guide* (document [UG10362](https://www.nxp.com/doc/UG10362)).

##### CitrineOS Web UI

To open the CitrineOS Web UI, follow the steps below:
1. In your browser, go to http://\<machine-ip\>:3000, where machine-ip is the IP address of the machine that hosts CitrineOS core.
1. Use administrator credentials to log in.

##### Charge point registration

Before exchanging information with CitrineOS, the charging station must be registered with the CSMS. Follow the instructions in the *Charging Station Management System (CSMS) Installation and Configuration User Guide* (document [UG10362](https://www.nxp.com/doc/UG10362)) to register the EasyEVSE to the CitrineOS instance. 

When authenticating with the CSMS, the EasyEVSE application uses one of the following security profiles, depending on the settings in EVSE_ConnectivityConfig.h:
1. **Security Profile 1**: HTTP Basic Authentication with password
2. **Security Profile 2**: HTTP Basic Authentication with password and TLS 1.2 (server authentication only)
3. **Security Profile 3**: TLS 1.2 with mutual authentication (client and server certificates)

By default, the EasyEVSE application uses security level 3 leveraging TLS with mutual authentication.

When registering your charging station, choose a unique name to identify your charging station (e.g. “cp001”). Remember this registration ID as it is needed by the EasyEVSE application when connecting to the CitrineOS. An HTTP authentication password is also required for security profiles 1 and 2. By default, the registering script uses the password "DEADBEEFDEADBEEF" when adding a charging station to CitrineOS.

After completing the registration process, open your CitrineOS interface, go to the “Charging Stations” window and you should see your charging station with the corresponding ID and the “Offline” status. Click on your specific charging station and go to "Add Connector". Use 'ConnectorID' = 1 and 'ConnectorIDType' = 1, when prompted (the EasyEVSE application is configured to run as EVSE: 1, connector: 1).

![plot](./images/aaa-063701.jpg)

##### Manage authorization list

To add your authorization tokens in the authorization list, follow the steps below:
1. In your CitrineOS interface, in the left panel, go to the “Authorizations” window.
2. Click the “Add New Authorization” button.

![plot](./images/aaa-063702.jpg)

3. In the “Authorization Id Token” field, add your authorization token Id. In the “IdToken Type” dropdown list, select your token type. The three authorization types available with the EasyEVSE application (full enablement version) are described below:
- If you want to register an NFC card, select ‘ISO14443’ as the token type and your card’s UID in the “Authorization Id Token” field.

**Note**: You can get the card UID from the EasyEVSE logs (search for 'Card UID' after tapping the card), the EasyEVSE GUI or by using an NFC reader application.

   - If you want to register a contract certificate for Plug and Charge, select ‘eMAID’ as the token type and add your eMAID value in the “Authorization Id Token” field.

**Note**: You can get the eMAID from the EasyEVSE logs (search for 'emaid value') or extract the CN field from the certificate using a crypto tool/library. The eMAID value is different for ISO15118-2 and ISO15118-20.

   - If you want to register a local token, select ‘Local’ as the token type and add the string “AUTH_FROM_CLI” in the “Authorization Id Token” field. In the EasyEVSE application, you can do local authorizations using the ‘auth true’ command in the cli.

![plot](./images/AddAuth.jpg)

#### Provision the SE050 with the OCPP client's key
In order to secure the OCPP communication using security profile 3 (enabled by default) that leverages the TLS with mutual authentication and SE050 support, the SE050 needs to be provisioned with the OCPP key of the EVSE. Follow the instructions in the [provisioning README](certs/README.md) to do the provisioning.

### 3.2 Step 1: Software setup
Clone the [APP-CODE-HUB/rd-nxp-easyevse-imxrt106x](https://github.com/nxp-appcodehub/rd-nxp-easyevse-imxrt106x) repository.

#### Import the project in MCUXpresso IDE or VS Code

To import the project in MCUXpresso IDE:
1. Proceed to import the project from filesystem
2. Choose the previously cloned archive
3. Go to project selection
4. MCUXpresso IDE or VS Code will automatically detect 3 projects:
* LVGL project. We removed the LVGL component from the main project because LVGL is usually generated automatically using GUI Guider and it is built only once.
* OCPP project with the OCPP stack implementation for the EVSE application.
* EasyEVSE project with multiple build configurations: EV, EV_UI, EVSE and EVSE_ETH on i.MXRT1060 and only EV and EV_UI on i.MXRT1064. EVSE and EVSE_ETH configurations link both LVGL and OCPP libraries, while EV_UI configuration only links the LVGL library.

The EVSE project offers support for both i.MXRT1060 and i.MXRT1064 MCUs. Select the folders corresponding to the existing EVK. The steps are the same for both MCUs.

5. Finish

![plot](./images/importHost.jpg)

To import the project in VS Code:
1. Press Ctrl+Shift+P to open a search bar in VS Code
2. Search _MCUXpresso for VS Code: Import Multiple Projects_
3. Browse the project corresponding to your board (RT1060 or RT1064)
4. Select RT106x, EasyEVSE_LVGL and EasyEVSE_OCPP projects then click OK.

![plot](./images/selectProjectsVSC.jpg)

#### Configure the EasyEVSE application to connect to your CSMS

In the EasyEVSE application code, to set up your connection to CitrineOS CSMS, go to the common/source/config/EVSE_ConnectivityConfig.h file and:

1. Set the EVSE_OCPP_SERVER_URL macro to the URL of the CitrineOS instance that you are using. The URL string has the following format: url="ws://\<CSMS IP or hostname\>:\<CSMS port\>":
 * Security level 1: url="ws://\<CSMS IP or hostname\>:8082"
 * Security level 2: url="wss://\<CSMS IP or hostname\>:8443"
 * Security level 3: url="wss://\<CSMS IP or hostname\>:8444"
2. Set the CHARGE_POINT_ID macro to the ID of the charge point as registered in CitrineOS (e.g. "cp001").
3. Set the CHARGE_POINT_AUTH_PASSWORD macro to the HTPP authentication password as registered in CitrineOS (e.g. "DEADBEEFDEADBEEF"). For security profile 3, this password is not needed.
4. Set the EVSE_OCPP_SECURITY_LEVEL macro to the security level you are using to connect to the CitrineOS CSMS.

![plot](./images/evse_config_ocpp.jpg)

#### Flash the boards

To flash the EVKs, follow these steps:
1. Build the LVGL project (EVSE build configuration)

![plot](./images/HostBuildLVGL.png)

2. Build the OCPP project

![plot](./images/HostBuildOCPP.jpg)

3. Build the EasyEVSE project in EVSE or EVSE_ETH configuration, depending on your hardware setup

4. Flash the i.MXRT1060 which will be used as EVSE
5. Build the EasyEVSE project in EV or EV_UI configuration
>**Note**: Make sure to build EV_UI configuration of LVGL project before building EV_UI configuration of EasyEVSE project.
6. Flash the RT106x which will be used as EV

>**Note**: In MCUXpresso, hover the hammer icon to check and change the build configuration. 

![plot](./images/HostCheckBuildConfig.png)

>**Note**: In VS Code, expand _Build Configurations_ menu and select the desired build configuration by checking the tick on the right. 

![plot](./images/aaa-065073.jpg)


---

### 3.2 Step 2: Hardware Assembly

There are multiple options available for setting up the system.
The assembly procedure is different based on the connectivity method, power supply or communication protocol chosen for the SIGBRD-HPGP board.

For instance, choosing the Wi-Fi connectivity will result in SIGBRD-HPGP to not be connected in the Arduino interface. The SDIO interface pins are shared with the only SPI interface from the Arduino header.

> **_NOTE:_** If SIGBRD-HPGP board is not connected via Ethernet and it shares the same SPI interface as the NFC frontend, then SPI must be reinitialized on the fly, as SIGBRD-HPGP board and NFC frontend are configured to operate in different SPI modes. The EasyEVSE application already deals with this scenario.

The three main hardware setups are:
1. SIGBRD-HPGP connected to the Arduino interface [EVSE build configuration with BOARD_SIGBOARD_ARDUINO_HEADER set to 1 and Ethernet connectivity (ENABLE_ETH set to 1)]

<img src="./images/Arduino_HPGP_connection.jpg" width="80%" height="80%">

2. SIGBRD-HPGP connected to camera interface [EVSE build configuration with BOARD_SIGBOARD_ARDUINO_HEADER set to 0 and Wi-Fi connectivity (ENABLE_WIFI set to 1)]

<img src="./images/CSI_HPGP_connection.jpg" width="80%" height="80%">

3. SIGBRD-HPGP connected via Ethernet [EVSE_ETH build configuration with BOARD_SIGBOARD_ARDUINO_HEADER set to 0 and Wi-Fi connectivity (ENABLE_WIFI set to 1)]

<img src="./images/ETH_HPGP_connection.jpg" width="80%" height="80%">

For more details, check `Board setup, wiring and assembly` chapter from the User Guide based on your existing hardware.


### 3.3 Step 3: Power the boards
1. Power the i.MXRT106x board using the external power supply (recommended) J40: 1-2
2. Power the EVSE-EMETER board using 12V power supply
3. Power the SIGBRD-HPGP board from host's Arduino interface (J25: 2-3) or external 5V power supply (J25: 1-2). We strongly recommend using the external power supply when SIGBRD-HPGP is not connected directly to the host's Arduino interface.

## 4. Results<a name="step4"></a>
1. EVSE project

Once the i.MXRT106x is powered the screen will light up and it will display the UI.

![plot](./images/GUI_MainMenu.jpg)

In Linking screen popup, please check that the SIGBRD-HPGP connection matches with the expected hardware configuration.

![plot](./images/LinkingScreenPopup.jpg)

2. EV project

Currently, the EV project has some basic UI capabilities. If the UI is enabled and the setup is correct, you will see the UI on the display. In order to check if the project is working, if UI is disabled, connect an USB cable in the J1 USB interface (connected to the LinkServer) to see the logs. Using a terminal emulator (230400 bps), logs should appear on the serial terminal connected to the board.

![plot](./images/EV_BootLogs.png)

The log from above is mandatory.

3. Start charging session

Once both boards have booted, connect the CP pins together. You can connect the SIGBRD-HPGP on EVSE side to a SIGBRD-HPGP or to an EVSE-SIG-BRD2X on EV side.

Before the charging actually starts, the charging session needs to be authorized. This can be done in three ways:
1. When OCPP is enabled, the server authorizes the charging session only if the card used for authentication has already been registered into the server.
2. When OCPP is disabled and NFC is enabled, the authorization is done using a whitelist of RFID cards stored locally on the EVSE board. Each list item is described as a pair of card UID + card type. If the list is left empty, the charging session is authorized by default. If the list contains at least one card, then the card presented to the NFC reader when authentication is requested is compared to each item of the list and it is accepted only if both the UID and type match with the registered cards. If a card is MIFARE DUOX, it also needs to pass all the security checks in order to be accepted.
3. If both OCPP and NFC are disabled, CLI authorization must be used, and the charging session starts afterwards.

![plot](./images/chargingMode.jpg)

On the Linking screen popup or Charging screen popup you can spot the charging state (Mode) switch from A -> B -> C.

![plot](./images/EV_ChargingLogs.png)

On the EV side, in the logs, you will spot messages like above. The PWM dictates the charge speed and the current battery level steadily increases.

## 5. Limitations<a name="step5"></a>

Limitations with the current implementation:
1. In case of a disconnect from the CSMS, the EVSE will try to reconnect for approximately 15 minutes. After that time, you will need to reset the board to reconnect.
2. The remote start and stop functionalities have not been validated with CitrineOS CSMS.
3. Due to bad SD contacts, the Wi-Fi initialization might fail and an EVSE reboot is needed.
4. When the CP line remains connected for extended durations, the EVSE may need a system restart.
5. Rare instances of EVSE-EMETER becoming unresponsive to host commands.

## 6. Support<a name="step6"></a>

Questions regarding the content/correctness of this example can be entered as Issues within this [GitHub repository](https://github.com/nxp-appcodehub/rd-nxp-easyevse-imxrt106x/issues).

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected functionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

## Related / Similar Platforms
Looking for related platforms or alternative EVSE architectures?

- [**KM35**](https://github.com/nxp-appcodehub/dm-nxp-km-metrology): implementation for KM3x metrology
- [**EVSE-EMETER**](https://github.com/nxp-appcodehub/rd-evse-emeter): implementation for EVSE-EMETER board
- [**SIGBRD-HPGP**](https://github.com/nxp-appcodehub/dm-evse-sigbrd-hpgp): implementation for SIGBRD-HPGP board
- [**EVSE-SIG-BRD2X**](https://github.com/nxp-appcodehub/dm-lpc5536-evse-sigbrd): implementation for EVSE-SIG-BRD2X board
- [**EVSE-RW612-OCPP**](https://github.com/nxp-appcodehub/dm-ocpp-evse-rw612): RW612-based EVSE with OCPP support

#### Project Metadata

<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-EVK&ndash;MIMXRT1060-blue)](https://www.nxp.com/pip/MIMXRT1060-EVK)
[![Board badge](https://img.shields.io/badge/Board-EVK&ndash;MIMXRT1064-blue)](https://www.nxp.com/pip/MIMXRT1064-EVK)


<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-INDUSTRIAL-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+industrial+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-USER%20INTERFACE-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+ui+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-NETWORKING-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+networking+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-CLOUD%20CONNECTED%20DEVICES-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+cc_devices+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-HMI-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+hmi+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-RTOS-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+rtos+in%3Areadme&type=Repositories)
[![Category badge](https://img.shields.io/badge/Category-WIRELESS%20CONNECTIVITY-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+wireless_connectivity+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-DISPLAY-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+display+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-DMA-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+dma+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-ETHERNET-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+ethernet+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-FLASH-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+flash+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-GPIO-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+gpio+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-I2C-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+i2c+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SDIO-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+sdio+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SDMMC-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+sdmmc+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SPI-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+spi+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-UART-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+uart+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-USB-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+usb+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected functionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/X-Follow%20us%20on%20X-black.svg)](https://x.com/NXP)

## 7. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
|5.1.2     | <p>Added EVSE-EM-HPGP support</p> <p>Enabled OCPP security profiles 2 and 3</p> <p>Enabled OCPP security profile 3 with client key stored in SE050</p> <p>Fixed a bug in which OCPP security profile 1 was used with incorrect password settings</p> <p>Included OCPP source code in the application</p> | March 27<sup>th</sup> 2026 |
|5.0.8     | <p>Enabled OCPP communication</p> <p>Added MIFARE DUOX support</p> <p>Updated SDK version to 2.16</p> <p>Added VS Code support</p> <p>Updated UI starting from GUI Guider example</p> <p>Dropped Azure Cloud support</p> <p>Dropped EVSE support on i.MXRT1064 </p> | December 15<sup>th</sup> 2025 |
|4.0.4     | <p>Added ISO15118-20 on the MCU</p> <p>Updated UI to address the ISO15118-20</p> <p>Updated SDK version to 2.15</p> <p>Added new build configuration for EVSE-SIG-BRD connection using the ETH</p> | April 25<sup>th</sup> 2025 |
|3.0.6     | Replaced Baltimore CA certificate with DigiCert Global Root G2 certificate for DPS registration        | October 15<sup>th</sup> 2024 |
|3.0.4     | Initial release on Application Code Hub        | August 20<sup>th</sup> 2024 |
