# STM32 without CubeIDE

Code for the blog post series "STM32 without CubeIDE" at https://kleinembedded.com

- [Part 1: The bare necessities](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities)
- [Part 2: CMSIS, make and clock configuration](https://kleinembedded.com/stm32-without-cubeide-part-2-cmsis-make-and-clock-configuration)
- [Part 3: The C Standard Library and printf](https://kleinembedded.com/stm32-without-cubeide-part-3-the-c-standard-library-and-printf)
- [Part 4: CMake, FPU and STM32 libraries](https://kleinembedded.com/stm32-without-cubeide-part-4-cmake-fpu-and-stm32-libraries)

## Initializing submodules
From part 4 of this series, the official STM32CubeF4 package is added as a git submodule. This makes it easy to get the newest updates and bugfixes whenever a new version is published.

After cloning the stm32-without-cubeide repository, initialize the top-level submodule:

`git submodule update --init`

Enter the submodule and initialize the CMSIS device driver and the HAL/LL library:

```shell
cd vendor/STM32CubeF4
git submodule update --init Drivers/CMSIS/Device/ST/STM32F4xx/ Drivers/STM32F4xx_HAL_Driver/
```

In order to update STM32CubeF4 to the newest version, simply navigate to the submodule's root directory, pull from remote and update the 
(already initialized) submodules:

```shell
cd vendor/STM32CubeF4
git pull origin master
git submodule update --recursive
```

## Building with CMake
After cloning the repository and initializing the submodules as described
above, you can configure and build the project with CMake from the root
project directory:

```shell
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/stm32f410rb.cmake
cmake --build build
```

Then flash the program to the target with:

```shell
cmake --build build --target flash
```
