# Developing natively

## Setting up the project

Required dependencies:

* git
* arm-gcc toolchain. You can download it from [ARM developers page](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* openocd (you can install it with apt install openocd in Ubuntu)

To install the dependencies and configure the project execute the following command

```bash
$ script/setup_project.sh
```

## Compilation

Navigate to the folder of the example you want to build and type this commands:

```bash
$ make clean
$ make
```

## Programming the micromouse

Navigate to the folder of the example you want to flash and run the following script that uses *openocd*

```bash
$ scripts/program_ratibroom.sh
```