################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/ports/drv_rw007.c 

OBJS += \
./board/ports/drv_rw007.o 

C_DEPS += \
./board/ports/drv_rw007.d 


# Each subdirectory must supply rules for building sources it contributes
board/ports/%.o: ../board/ports/%.c
	arm-none-eabi-gcc -I"D:\RT-ThreadStudio\workspace\RA6M4-uart" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\board\ports" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\board" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\libraries\HAL_Drivers\config" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\libraries\HAL_Drivers" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra\fsp\inc\api" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra\fsp\inc\instances" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra\fsp\inc" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra_cfg\fsp_cfg\bsp" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra_cfg\fsp_cfg" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\ra_gen" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\libc\compilers\common" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\libc\compilers\newlib" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\libc\posix\io\poll" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\libc\posix\io\stdio" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\components\libc\posix\ipc" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\RA6M4-uart\rt-thread\libcpu\arm\cortex-m4" -include"D:\RT-ThreadStudio\workspace\RA6M4-uart\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
