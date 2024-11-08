set(WINDOWS_ST_CLT_PATH "D:/IDEs/ArmelStudio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin")
# set(MAC_ST_CLT_PATH "/opt/ST/STM32CubeCLT/GNU-tools-for-STM32/bin/")
if(EXISTS "${WINDOWS_ST_CLT_PATH}")
    set(TOOLCHAIN_DIRECTORIES ${WINDOWS_ST_CLT_PATH})
# elseif(EXISTS "${MAC_ST_CLT_PATH}")
#     set(TOOLCHAIN_DIRECTORIES ${MAC_ST_CLT_PATH})
# else()
#     # Try to find an STM32CubeIDE installation to use for the toolchain.
#     file(GLOB TOOLCHAIN_DIRECTORIES
#         "D:/IDEs/CubeIDE/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*/tools/bin/"
#         "/opt/st/stm32cubeide_*/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*/tools/bin/"
#         "/Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*/tools/bin/"
#     )
endif()
list(LENGTH TOOLCHAIN_DIRECTORIES TOOLCHAIN_DIRECTORIES_COUNT)

if(TOOLCHAIN_DIRECTORIES_COUNT LESS 1)
    message(WARNING "Could not find an avr installation. Falling back to tools available on PATH.")
else()
    list(GET TOOLCHAIN_DIRECTORIES 0 TOOLCHAIN_DIRECTORY)
    if (TOOLCHAIN_DIRECTORIES_COUNT GREATER 1)
        message(STATUS "Found multiple avr installations. Using \"${TOOLCHAIN_DIRECTORY}\".")
    endif()
endif()

if(WIN32)
    set(TOOLCHAIN_SUFFIX ".exe")
endif()

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          avr)

set(TOOLCHAIN_PREFIX                "avr-")
if(DEFINED TOOLCHAIN_DIRECTORY)
    set(TOOLCHAIN_PREFIX            "${TOOLCHAIN_DIRECTORY}/${TOOLCHAIN_PREFIX}")
endif()


# -x c -funsigned-char -funsigned-bitfields -DDEBUG  
# -I"D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\include" 
# -I"c:/AVR/_AVR" -I"../_I"  
# -Os 
# -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=avr128db48 
# -B "D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\gcc\dev\avr128db48" 
# -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" 
# set(FLAGS                           "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
set(C_FLAGS                       "-x c")
set(FLAGS "-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=avr128db48 -B D:/IDEs/ArmelStudio/7.0/Packs/atmel/AVR-Dx_DFP/1.10.114/gcc/dev/avr128db48") 
# -I D:/IDEs/ArmelStudio/7.0/Packs/atmel/AVR-Dx_DFP/1.10.114/include")


# -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=avr128db48 
# -B "D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\gcc\dev\avr128db48" 
# -I "D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\include"  
# -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g 
set(ASM_FLAGS "-Wa,-gdwarf2 -x assembler-with-cpp -c -I D:/IDEs/ArmelStudio/7.0/Packs/atmel/AVR-Dx_DFP/1.10.114/include")


# -funsigned-char -funsigned-bitfields 
# -DF_CPU=16000000UL -DCLOCK=CLKHT,TA0,CLKNOOUT -DDIOD=PB,3,INV -DDEBUG  
# -I"D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\include" 
# -I"c:/AVR/_AVR" -I"../_I"  
# -Os 
# -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=avr128db48 
# -B "D:\IDEs\ArmelStudio\7.0\Packs\atmel\AVR-Dx_DFP\1.10.114\gcc\dev\avr128db48" 
# -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" 
set(CPP_FLAGS                       "")

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX} ${FLAGS} ${C_FLAGS})
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER} ${ASM_FLAGS})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_SUFFIX} ${FLAGS} ${CPP_FLAGS})
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy${TOOLCHAIN_SUFFIX})
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size${TOOLCHAIN_SUFFIX})

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)