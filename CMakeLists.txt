#指定最低版本
cmake_minimum_required(VERSION 3.30)

#定义项目的名称和指明支持的编程语言<C><CXX><汇编ASM>
project(stm32_cmake  C CXX ASM)
#等效于
#project(stm32_cmake)
#enable_language(C CXX ASM)

#设置项目中 C 标准
set(CMAKE_C_STANDARD 11)
#设置项目中 C++ 标准
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_SYSTEM_NAME Linux) #设置目标系统名字
set(CMAKE_SYSTEM_PROCESSOR arm) #设置目标处理器架构

# 设置安装路径
set(installl_dir ${CMAKE_CURRENT_LIST_DIR}/OUT)
message(STATUS ${CMAKE_CURRENT_LIST_DIR})
message(STATUS ${CMAKE_CURRENT_LIST_DIR}/OUT)
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

##指定项目的交叉编译器和工具，需要配置GCC工具链的环境变量
find_program(arm_gcc_file_path arm-none-eabi-gcc)
if(NOT IS_ABSOLUTE ${arm_gcc_file_path})
    message(FATAL_ERROR "arm-none-eabi-gcc is not found!")  #出错后，停止编译
endif()


#如果没有指定 CMAKE_FIND_ROOT_PATH，那么 CMAKE_FIND_ROOT_PATH_MODE_<KIND> 变量的设置不会有任何效果，
#因为这些模式变量是用于控制如何在 CMAKE_FIND_ROOT_PATH 指定的路径中查找文件的。
set(CMAKE_FIND_ROOT_PATH ${arm_gcc_file_path})
#从来不在指定目录(交叉编译)下查找工具程序。(编译时利用的是宿主的工具)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#只在指定目录(交叉编译)下查找库文件
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#只在指定目录(交叉编译)下查找头文件
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#只在指定的目录(交叉编译)下查找依赖包
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


#指定C编译器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#指定C++编译器
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
#指定汇编语言编译器
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
#指定创建存档或创建static库的程序名称。
set(CMAKE_AR arm-none-eabi-ar)
#指定生成二进制文件的工具
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
#指定生成汇编文件的工具
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
#指定生成大小报告的工具
set(SIZE arm-none-eabi-size)

set(NM arm-none-eabi-nm)

##添加编译选项
#[[
    add_compile_options 用于添加编译选项，
        -mcpu=cortex-m3 用于指定 CPU 架构，ARM内核有M0，M3，M4，M7，H7，A系列
        -mthumb 用于指定 Thumb 指令集，
        -mthumb-interwork 用于指定 Thumb 指令集兼容 ARM 指令集。
]]
add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
#[[
    有时我们的程序会定义一些暂时使用不上的功能和函数，虽然我们不使用这些功能和函数，
        但它们往往会浪费我们的ROM和RAM的空间。这在使用静态库时，体现的更为严重。
            有时，我们只使用了静态库仅有的几个功能，但是系统默认会自动把整个静态库全部链接到可执行程序中，
                造成可执行程序的大小大大增加。
    为了解决前面分析的问题，我们引入了标题中的几个参数。
        GCC链接操作是以section作为最小的处理单元，只要一个section中的某个符号被引用，
            该section就会被加入到可执行程序中去。
    因此，GCC在编译时可以使用 -ffunction-sections和 -fdata-sections 将每个函数或符号创建为一个sections，
        其中每个sections名与function或data名保持一致。
    而在链接阶段， -Wl,–gc-sections 指示链接器去掉不用的section
        （其中-wl, 表示后面的参数 -gc-sections 传递给链接器），这样就能减少最终的可执行程序的大小了。
]]
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

#[[
    # optimization    ## 选择优化等级，优化等级低->高，可参考Keil的设置，如果程序运行不理想，优化程序或降低优化等级
    #OPT = -O0        ## 没有优化。
    #OPT = -O1        ## 编译器试图减少代码大小和执行时间，而不执行任何花费大量编译时间的优化。
    #OPT = -Og        ## 参数 -Og 是在 -O1 的基础上，去掉了那些影响调试的优化告诉编译器，编译后的代码不要影响调试，但调试信息的生成还是靠 -g 参数的。
    #OPT = -O2        ## 更加优化。GCC执行几乎所有支持的优化，不涉及空间速度权衡。此选项既增加编译时间，提高了生成代码的性能。
    #OPT = -O3        ## 优化更多。
    #OPT = -Os        ## 优化大小。启用所有优化，除了那些经常增加代码大小
    #OPT = -Ofast     ## 不遵守严格的标准。使所有优化。它还支持并非对所有符合标准的程序都有效的优化。
]]
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()


# 设置编译时宏定义
add_compile_definitions(STM32F10X_HD USE_STDPERIPH_DRIVER)   
#add_definitions效果相似，相比于add_compile_definitions，会添加 -D 标志


# 链接脚本
set(LINK_SCRIPT 
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/STM32F103C8Tx_FLASH.ld
)
# 启动文件
set(STARTUP_SCRIPT
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/startup/gcc_ride7/startup_stm32f10x_md.s
)
# CMSIS 内核文件
set(CMSIS_CORE
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/core_cm3.c
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/system_stm32f10x.c
)

#list(APPEND xxx.c)
# 标准库外设源文件
file(GLOB STDPERIPH_DRIVER
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/*.c
)

# 用户程序的源文件（递归查找）
file(GLOB_RECURSE USER_SRC
    ${CMAKE_SOURCE_DIR}/User/*.c
)

set(OS_LIB
    ${CMAKE_SOURCE_DIR}/FreeRTOS/croutine.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/event_groups.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/list.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/queue.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/tasks.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/timers.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/portable/GCC/ARM_CM3/port.c
    ${CMAKE_SOURCE_DIR}/FreeRTOS/portable/MemMang/heap_4.c
)

set(MQTT_CLIENT
    ${CMAKE_SOURCE_DIR}/mqttclient/common/mqtt_list.c
    ${CMAKE_SOURCE_DIR}/mqttclient/common/random.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTConnectClient.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTDeserializePublish.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTFormat.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTPacket.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTSerializePublish.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTSubscribeClient.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt/MQTTUnsubscribeClient.c
    ${CMAKE_SOURCE_DIR}/mqttclient/mqttclient/mqttclient.c
    ${CMAKE_SOURCE_DIR}/mqttclient/network/nettype_tcp.c
    ${CMAKE_SOURCE_DIR}/mqttclient/network/network.c
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS/platform_memory.c
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS/platform_mutex.c
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS/platform_net_socket.c
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS/platform_thread.c
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS/platform_timer.c
)
# ELF 文件名称
set(ELF_TARGET ${CMAKE_PROJECT_NAME}.elf)


# 添加 ELF 目标文件
add_executable(
    ${ELF_TARGET}
    ${CMSIS_CORE}
    ${STARTUP_SCRIPT}
    ${STDPERIPH_DRIVER}
    ${USER_SRC}
    ${OS_LIB}
    ${MQTT_CLIENT}
)

# 头文件路径
target_include_directories(
    ${ELF_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F10x_StdPeriph_Driver/inc
    ${CMAKE_SOURCE_DIR}/User
    ${CMAKE_SOURCE_DIR}/User/led
    ${CMAKE_SOURCE_DIR}/User/submodule_config
    ${CMAKE_SOURCE_DIR}/FreeRTOS/include
    ${CMAKE_SOURCE_DIR}/FreeRTOS/portable/GCC/ARM_CM3
    ${CMAKE_SOURCE_DIR}/mqttclient/common
    ${CMAKE_SOURCE_DIR}/mqttclient/mqtt
    ${CMAKE_SOURCE_DIR}/mqttclient/mqttclient
    ${CMAKE_SOURCE_DIR}/mqttclient/network
    ${CMAKE_SOURCE_DIR}/mqttclient/platform/FreeRTOS
)
    

target_compile_options(${ELF_TARGET} PRIVATE
    ${CPU_FLAGS}
    -Wall
    -Wextra
    -g 
    -gdwarf-2 
    -mthumb 
    -Os 
    -fdata-sections 
    -ffunction-sections
    -fmessage-length=0
)
# 链接参数
target_link_options(${ELF_TARGET} PRIVATE
    -T${LINK_SCRIPT}
    ${CPU_FLAGS}
    --specs=nano.specs
    -Wl,--gc-sections #-Wl用于将选项传递给链接器（ld）。--gc-sections选项告诉链接器在生成的二进制文件中删除未使用的段。
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--cref
    -Wl,--print-memory-usage  #让链接器在编译过程中打印内存使用情况
)

# 添加自定义命令，用于生成 hex bin 格式文件，POST_BUILD 表示生成 TARGET 后执行命令
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${ELF_TARGET} ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${ELF_TARGET} ${CMAKE_PROJECT_NAME}.bin
    COMMAND ${SIZE}  ${CMAKE_PROJECT_NAME}.elf  #编译完成后打印text bss data段长度
   # COMMAND ${NM} -a ${ELF_TARGET} >> ${CMAKE_PROJECT_NAME}.map
)

############################设置安装目录###################################
install(PROGRAMS 
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map 
    DESTINATION ${installl_dir}
)
