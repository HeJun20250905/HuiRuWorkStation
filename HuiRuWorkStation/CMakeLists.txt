# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.16)

# 设置项目名称和编程语言
project(HuiRuWorkStation LANGUAGES CXX)

# 包含Qt的CMake模块 (Qt VS Tools )
include(qt.cmake)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 强制要求使用指定的C++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt库
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Sql
)

# 显式启用Qt代码生成
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 标准Qt项目设置
qt_standard_project_setup()

# 添加项目源文件
set(PROJECT_SOURCES
    main.cpp
    HuiRuWorkStation.ui
    HuiRuWorkStation.h
    HuiRuWorkStation.cpp
    TestDialog/QTestDialog.ui
    TestDialog/QTestDialog.cpp
    TestDialog/QTestDialog.h
    CustomToolButton/QCustomToolButton.cpp
    CustomToolButton/QCustomToolButton.h
    CustomStateMachine/QCustomStateMachine.cpp
    CustomStateMachine/QCustomStateMachine.h
    HuiRuWorkStation.qrc 
    CustomButton/QCustomButton.cpp
    CustomButton/QCustomButton.h
    CustomListView/QCustomListView.cpp
    CustomListView/QCustomListView.h
)

# 添加可执行文件目标
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# 设置 Windows GUI 应用程序
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# 链接Qt库到目标
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
)
