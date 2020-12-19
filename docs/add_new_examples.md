
# Adding new examples

In order to add new examples and keep everything working flawlessly you should follow these rules:
* Create a new folder for your example and call it something like `example_name_src`. Just keep the `_src` suffix so it is easier to keep everything nice and tidy.
* Include a README.md file in the folder with a basic explanation of what the example does or, if it is something complex, how to make it work.
* Copy the Makefile from one of the existing examples to the new folder. Keep the binary name as `main`.
* Create a base source file and call it `main.c`. Create the required source subfolders and include them in the Makefile. If your example has the following structure:
```
communications_src/
├── main.c
└── communications/
    ├── communications.c
    └── communications.h
```
You should apply the following changes to the new Makefile:
```diff
OBJS = $(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))
-OBJS += $(patsubst hal/%.c,hal/%.o,$(wildcard hal/*.c))
+OBJS += $(patsubst communications/%.c,communications/%.o,$(wildcard communications/*.c))
```
* Modify the file `.vscode/c_cpp_properties.json` to include the path to the new example. This step ensures that *IntelliSense* is able to find all the required source files. Following the previous folder structure, the changes would look as follows:
```diff
            "includePath": [
                "${workspaceFolder}/leds_src/*",
+               "${workspaceFolder}/communications_src/*",
                "${workspaceFolder}/.opencm3/libopencm3/include"
            ],
```
* Add a new entry in the main README.md examples list.