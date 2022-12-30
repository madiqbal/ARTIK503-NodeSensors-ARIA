################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../._main.c \
../lwm2m_client.c \
../lwm2m_common.c \
../mqtt-api.c \
../mqtt_main.c \
../wifi-auto.c 

OBJS += \
./._main.o \
./lwm2m_client.o \
./lwm2m_common.o \
./mqtt-api.o \
./mqtt_main.o \
./wifi-auto.o 

C_DEPS += \
./._main.d \
./lwm2m_client.d \
./lwm2m_common.d \
./mqtt-api.d \
./mqtt_main.d \
./wifi-auto.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARTIK GCC C Compiler'
	arm-none-eabi-gcc  -D__TINYARA__ -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/include -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/include/apps -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/framework_include -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/apps_include -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/apps_include/netutils -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/base -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/connectivity -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/systemio -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/wifi -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/lwm2m -I/workspace/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/mqtt -O2 -ffunction-sections -fdata-sections -fno-builtin -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -Wstrict-prototypes -Wshadow -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-but-set-variable -DAPP_STACKSIZE=18432 -DAPP_PRIORITY=100 -DCONFIG_ARTIK_IDE_INTERGRATE -mcpu=cortex-r4 -mfpu=vfpv3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


