# Developing using Docker and VSCode

Use VSCode to create code, compile and flash it.

By using this configuration you don't need to install anything in your host system as every required dependency is included in the Docker container. As a bonus, you get a nice UI to debug using GDB!

This repo contains VSCode configurations for:
 - linking to libopencm3 code references from the robot source code.
 - run common development tasks like building, cleaning and flashing binaries and even debugging using GDB.

## Setup
### Dependencies
You just need Docker running, VSCode and git.

### Remote containers extension

You need to add [remote containers](https://code.visualstudio.com/docs/remote/containers) extension to VSCode. Open VSCode, pres ***Ctrl+p***, paste *ext install ms-vscode-remote.remote-containers* in the input field that appears and press ***enter***.

### Udev rules
In order to use an Stlink v2 or v2.1 programmer without admin premission you need to make the following configuration changes in the host OS:

* Create file `/etc/udev/rules.d/99-openocd.rules` in the host machine and set the following contents:

```bash
# Copy this file to /etc/udev/rules.d/

ACTION!="add|change", GOTO="openocd_rules_end"
SUBSYSTEM!="usb|tty|hidraw", GOTO="openocd_rules_end"

# Please keep this list sorted by VID:PID

# opendous and estick
ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="204f", MODE="664", GROUP="plugdev"

# Original FT232/FT245 VID:PID
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", MODE="664", GROUP="plugdev"

# Original FT2232 VID:PID
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="664", GROUP="plugdev"

# Original FT4232 VID:PID
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6011", MODE="664", GROUP="plugdev"

# Original FT232H VID:PID
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6014", MODE="664", GROUP="plugdev"

# DISTORTEC JTAG-lock-pick Tiny 2
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="8220", MODE="664", GROUP="plugdev"

# TUMPA, TUMPA Lite
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="8a98", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="8a99", MODE="664", GROUP="plugdev"

# XDS100v2
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="a6d0", MODE="664", GROUP="plugdev"

# Xverve Signalyzer Tool (DT-USB-ST), Signalyzer LITE (DT-USB-SLITE)
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="bca0", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="bca1", MODE="664", GROUP="plugdev"

# TI/Luminary Stellaris Evaluation Board FTDI (several)
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="bcd9", MODE="664", GROUP="plugdev"

# TI/Luminary Stellaris In-Circuit Debug Interface FTDI (ICDI) Board
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="bcda", MODE="664", GROUP="plugdev"

# egnite Turtelizer 2
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="bdc8", MODE="664", GROUP="plugdev"

# Section5 ICEbear
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="c140", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="c141", MODE="664", GROUP="plugdev"

# Amontec JTAGkey and JTAGkey-tiny
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="cff8", MODE="664", GROUP="plugdev"

# TI ICDI
ATTRS{idVendor}=="0451", ATTRS{idProduct}=="c32a", MODE="664", GROUP="plugdev"

# STLink v1
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3744", MODE="664", GROUP="plugdev"

# STLink v2
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="664", GROUP="plugdev"

# STLink v2-1
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="664", GROUP="plugdev"

# Hilscher NXHX Boards
ATTRS{idVendor}=="0640", ATTRS{idProduct}=="0028", MODE="664", GROUP="plugdev"

# Hitex STR9-comStick
ATTRS{idVendor}=="0640", ATTRS{idProduct}=="002c", MODE="664", GROUP="plugdev"

# Hitex STM32-PerformanceStick
ATTRS{idVendor}=="0640", ATTRS{idProduct}=="002d", MODE="664", GROUP="plugdev"

# Amontec JTAGkey-HiSpeed
ATTRS{idVendor}=="0fbb", ATTRS{idProduct}=="1000", MODE="664", GROUP="plugdev"

# IAR J-Link USB
ATTRS{idVendor}=="1366", ATTRS{idProduct}=="0101", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="1366", ATTRS{idProduct}=="0102", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="1366", ATTRS{idProduct}=="0103", MODE="664", GROUP="plugdev"
ATTRS{idVendor}=="1366", ATTRS{idProduct}=="0104", MODE="664", GROUP="plugdev"

# J-Link-OB (onboard)
ATTRS{idVendor}=="1366", ATTRS{idProduct}=="0105", MODE="664", GROUP="plugdev"

# Raisonance RLink
ATTRS{idVendor}=="138e", ATTRS{idProduct}=="9000", MODE="664", GROUP="plugdev"

# Debug Board for Neo1973
ATTRS{idVendor}=="1457", ATTRS{idProduct}=="5118", MODE="664", GROUP="plugdev"

# Olimex ARM-USB-OCD
ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="0003", MODE="664", GROUP="plugdev"

# Olimex ARM-USB-OCD-TINY
ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="0004", MODE="664", GROUP="plugdev"

# Olimex ARM-JTAG-EW
ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="001e", MODE="664", GROUP="plugdev"

# Olimex ARM-USB-OCD-TINY-H
ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="002a", MODE="664", GROUP="plugdev"

# Olimex ARM-USB-OCD-H
ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="002b", MODE="664", GROUP="plugdev"

# USBprog with OpenOCD firmware
ATTRS{idVendor}=="1781", ATTRS{idProduct}=="0c63", MODE="664", GROUP="plugdev"

# TI/Luminary Stellaris In-Circuit Debug Interface (ICDI) Board
ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", MODE="664", GROUP="plugdev"

# Marvell Sheevaplug
ATTRS{idVendor}=="9e88", ATTRS{idProduct}=="9e8f", MODE="664", GROUP="plugdev"

# CMSIS-DAP compatible adapters
ATTRS{product}=="*CMSIS-DAP*", MODE="664", GROUP="plugdev"

LABEL="openocd_rules_end"
```
* Restart udev:

```bash
$ sudo udevadm trigger
```

### Download and code setup
 - **First:** Clone this repo.
 - **Second:** Open the repository folder in VSCode
 - **Third:** Launch development container. In VSCode use ***Ctrl+Shift+p*** and ***Remote-Containers: Open Folder in Container...*** then choose the respository folder. After a few seconds VSCode window will reload pointing to the development container.
  - **Fourth:** Setup the project. In VSCode terminal run `scripts/setup_project.sh`. This will download git project submodules and setup some required dependencies.

## Build code
Open `main.c` file from the example you want to build and press ***Ctrl+Shift+b***. Alternatively you can navigate to the menu ***Terminal->Run Task...*** then select ***Build current example***

## Clean code
Open `main.c` file from the example you want to clean and navigate to the menu ***Terminal->Run Task...*** then select ***Clean current example***

## Flash binary
Open `main.c` file from the example you want to clean and navigate to the menu ***Terminal->Run Task...*** then select ***Flash current example***

## Debug using STLink and GDB

This repository contains configurations to allow VSCode to launch GDB and debug the firmware running in the microcontroller.

Open `main.c` file from the example you want to debug and press ***Ctrl+Shift+d***. In the new menu, select the appropriate debug configuration (Stlink 2, 2.1 or Black Magic probe) and press ***Start debugging*** (the green icon that looks like a play button)

> Note that the different tasks rely the currently focused file (opened in VSCode editor) to decide what example to build, clean, flash or debug.
