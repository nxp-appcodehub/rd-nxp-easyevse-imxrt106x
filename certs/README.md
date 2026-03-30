# OCPP key provisioning
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## Table of Contents
1. [Overview](#step1)
2. [Offline Provisioning](#step2)

## 1. Overview <a name="step1"></a>
The EasyEVSE application comes configured to use the SE050 to store the OCPP client's private key. In order to secure the OCPP communication using security profile 3 (enabled by default) that leverages the TLS with mutual authentication, the SE050 needs to be provisioned with the OCPP key pair of the EVSE. There are two options available to provision the OCPP key:
* Option 1: using an external tool such as **ssscli**, part of the Plug and Trust middleware for the SE050. With this option, the SE050 needs to be provisioned before using it in the EasyEVSE setup. 
* Option 2: using the **EdgeLock2GO** provisioning service provided by NXP. With this option, the SE050 will be provisioned automatically by the EasyEVSE application, given that the board has an active Internet connection.

This document describes option 1. For option 2, we recommend you rely on the [EdgeLock2GO](https://edgelock2go.com) documentation for generating the OCPP client's certificate chain.

## 1. Offline Provisioning <a name="step2"></a>
This section describes the steps needed to provision the SE050 with the OCPP client’s private key needed by the EVSE. The rest of the OCPP client certificate chain does not need to be provisioned to the secure module and is handled in software. To provision the SE050 board, use the i.MX RT1060 EVK as a bridge between the PC and the SE050 board.

To provision the SE050 offline, follow the steps below:
1. Download [Plug and Trust middleware](https://www.nxp.com/pages/edgelock-se050-plug-and-trust-secure-element-family-enhanced-iot-security-with-high-flexibility:SE050) and extract the archive to the *SE05X_root_folder* folder. The *simw-top\binaries\PCWindows\ssscli* folder contains the ssscli.exe executable that we use to provision OCPP key.
2. Download the SE050 VCOM binary for the i.MX RT1060 EVK from *SE05X_root_folder\simw-top\binaries\MCU\se05x\se05x_vcom-T1oI2C-evkmimxrt1060.bin*.
3. On i.MX RT1060 EVK, set the SW4 DIP switch as 0001 and connect a USB cable to the J1 USB connector.
4. Open Windows File Explorer. The EVK is visible as a removable drive. Drag and drop the *se05x_vcom_T1oI2C_evkmimxrt1060.bin* file onto the EVK (Windows indicate the progress of the copy process). When completed, the EVK USB enumeration temporarily disappears and then reappears. <u>*Note: If this step does not work, the binary can be flashed using the debugger. For i.MX RT1060, flash the binary at address 0x60000000.*</u>
5. Power off the board and return DIP switch to the original position: 0010.
6. Mount the SE050 board on top of the i.MX RT1060 EVK, using the Arduino interface.
7. Add a second USB cable to i.MX RT1060 EVK J48 and PC. Power on the board.
8. Open the Windows command prompt and navigate to the *SE05X_root_folder>\simw-top\binaries\PCWindows\ssscli* folder.
9. Use **ssscli.exe connect se05x vcom COMxx** to open a session to the SE050.
10. Use **ssscli.exe se05x uid** to see the UID of the SE050. If the connection and bridge are operating correctly, the SE050 UID appears.
11. The EasyEVSE-Software/certs/ocpp_client includes the OCPP client’s private key in PEM format: OCPP_CLIENT_KEY.pem and use **ssscli.exe set ecc pair 0x00224401 OCPP_CLIENT_KEY.pem** to provision it into the SE050. <u>*Note: The identifier 0x00224401 matches the value of ocpp_client_leaf_key_filepath string used by the EasyEVSE application.*</u>
12. Close the SE050 session using the **ssscli.exe disconnect** command.
