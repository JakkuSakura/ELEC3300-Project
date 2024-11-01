cmake_minimum_required(VERSION 3.29)
project(ELEC3300_Lab2 C)

set(CMAKE_C_STANDARD 11)

include_directories(Core/Inc)
include_directories(Drivers/CMSIS)
include_directories(Drivers/CMSIS/Device)
include_directories(Drivers/CMSIS/Device/ST)
include_directories(Drivers/CMSIS/Device/ST/STM32F1xx)
include_directories(Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(Drivers/CMSIS/Include)
include_directories(Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(Drivers/STM32F1xx_HAL_Driver/Inc/Legacy)

add_executable(ELEC3300_Lab2
        Core/Src/main.c
        Core/Src/state.c
        Core/Src/stm32f1xx_hal_msp.c
        Core/Src/stm32f1xx_it.c
        Core/Src/syscalls.c
        Core/Src/sysmem.c
        Core/Src/system_stm32f1xx.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
        Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
)
