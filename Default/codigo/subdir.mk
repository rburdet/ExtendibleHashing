################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../codigo/fisica_Block.cpp \
../codigo/fisica_DirHandler.cpp \
../codigo/fisica_VarBuffer.cpp \
../codigo/interfaz_Menu.cpp \
../codigo/interfaz_main.cpp \
../codigo/logica_BlockTable.cpp \
../codigo/logica_Buscador.cpp \
../codigo/logica_FileHandler.cpp \
../codigo/logica_HashExtensible.cpp \
../codigo/logica_Indexer.cpp \
../codigo/logica_IndicePrimario.cpp \
../codigo/logica_Reg.cpp \
../codigo/logica_Utils.cpp \
../codigo/logica_Validator.cpp 

OBJS += \
./codigo/fisica_Block.o \
./codigo/fisica_DirHandler.o \
./codigo/fisica_VarBuffer.o \
./codigo/interfaz_Menu.o \
./codigo/interfaz_main.o \
./codigo/logica_BlockTable.o \
./codigo/logica_Buscador.o \
./codigo/logica_FileHandler.o \
./codigo/logica_HashExtensible.o \
./codigo/logica_Indexer.o \
./codigo/logica_IndicePrimario.o \
./codigo/logica_Reg.o \
./codigo/logica_Utils.o \
./codigo/logica_Validator.o 

CPP_DEPS += \
./codigo/fisica_Block.d \
./codigo/fisica_DirHandler.d \
./codigo/fisica_VarBuffer.d \
./codigo/interfaz_Menu.d \
./codigo/interfaz_main.d \
./codigo/logica_BlockTable.d \
./codigo/logica_Buscador.d \
./codigo/logica_FileHandler.d \
./codigo/logica_HashExtensible.d \
./codigo/logica_Indexer.d \
./codigo/logica_IndicePrimario.d \
./codigo/logica_Reg.d \
./codigo/logica_Utils.d \
./codigo/logica_Validator.d 


# Each subdirectory must supply rules for building sources it contributes
codigo/%.o: ../codigo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


