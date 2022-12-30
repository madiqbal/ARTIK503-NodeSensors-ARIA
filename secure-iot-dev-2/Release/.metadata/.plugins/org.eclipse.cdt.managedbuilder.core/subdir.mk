################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.c 

OBJS += \
./.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.o 

C_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.d 


# Each subdirectory must supply rules for building sources it contributes
.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.o: ../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARTIK GCC C Compiler'
	arm-none-eabi-gcc  -D__TINYARA__ -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/include -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/include/apps -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/framework_include -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/apps_include -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/apps_include/netutils -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/base -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/connectivity -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/systemio -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/wifi -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/lwm2m -I/media/iq/5C482E7D482E55D4/ARTIK/SDK/A05x/18.05.00/libsdk/A053/extra_tls_2/artik-sdk_include/mqtt -O2 -ffunction-sections -fdata-sections -fno-builtin -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -Wstrict-prototypes -Wshadow -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-but-set-variable -DAPP_STACKSIZE=18432 -DAPP_PRIORITY=100 -DCONFIG_ARTIK_IDE_INTERGRATE -mcpu=cortex-r4 -mfpu=vfpv3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


