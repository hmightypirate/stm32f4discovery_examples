# STM32F407 Discovery Board Examples

This repository contains basic examples that show how to use different STM32F407 functionalities using [LibOpenCM3](https://libopencm3.org/) framework.

The examples and configurations are targeted to the [STM32F4 Discovery Board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) but they can be easily tweaked for other boards featuring similar microcontrollers like STM32F405 series.

## Developing

We provide different ways to develop these examples:

* If you want to compile from the host read [development from host](docs/developing_from_host.md)

* We have a setup with VSCode integrated with a Docker container that includes all the required tools. For that we use [remote containers](https://code.visualstudio.com/docs/remote/containers), [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) extensions. [Here are some instructions for this setup](docs/developing_from_vscode.md).

## Examples

* [leds](leds_src/README.md)

## Adding new examples

[Here you can find information about what to do to add new examples](docs/add_new_examples.md)