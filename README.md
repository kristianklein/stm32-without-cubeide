# STM32 without CubeIDE

Code for the blog post series "STM32 without CubeIDE" at https://kleinembedded.com

- [Part 1: The bare necessities](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities)
- [Part 2: CMSIS, make and clock configuration](https://kleinembedded.com/stm32-without-cubeide-part-2-cmsis-make-and-clock-configuration)
- [Part 3: The C Standard Library and printf](https://kleinembedded.com/stm32-without-cubeide-part-3-the-c-standard-library-and-printf)

## Initializing submodules
From part 4 of this series, the official STM32CubeF4 package is added as a git submodule. This makes it easy to get the newest updates and bugfixes whenever a new version is published.

After cloning the stm32-without-cubeide repository, initialize the top-level submodule:

`git submodule update --init`

This submodule itself contains several submodules. You can list them with:

`git submodule status --recursive`

For now, we will only use the CMSIS package and the HAL/LL driver. Initialize them like this:

`git submodule update --init vendor/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx vendor/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver`

To update STM32CubeF4 to the newest version, simply navigate to the submodule's root directory, pull from remote and update submodules:

```shell
cd vendor/STM32CubeF4
git pull origin master
git submodule update --recursive
```