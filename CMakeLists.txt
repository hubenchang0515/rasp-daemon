# Rasptool-Daemon

# cmake version
cmake_minimum_required ( VERSION   3.11 )

project ( raspd )

# 通过 pkg-config 获取并添加include目录和link目录
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/find")
find_package (PkgConfig REQUIRED)
pkg_check_modules (GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules (GTKMM REQUIRED gtkmm-2.4)

include_directories (${GLIBMM_INCLUDE_DIRS})
link_directories (${GLIBMM_LIBRARY_DIRS})

include_directories (${GTKMM_INCLUDE_DIRS})
link_directories (${GTKMM_LIBRARY_DIRS})


# 基础核心部分源码
include_directories (./src/raspd-dbus)
aux_source_directory( ./src/raspd-dbus DBUS_SRC)

# session进程及其模块
aux_source_directory( ./src/session SESSION_SRC)

# 编译目标
add_executable( ${PROJECT_NAME}-session 
                ${DBUS_SRC}
                ${SESSION_SRC}
                ${HELLO_SRC}
                )

# 链接参数
target_link_libraries (${PROJECT_NAME}-session 
                        ${GLIBMM_LIBRARIES} 
                        ${GTKMM_LIBRARIES})