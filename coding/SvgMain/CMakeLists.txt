cmake_minimum_required (VERSION 3.0)
#cmake 最低版本要求
project(SvgMain)
#工程名字

set(QT_PATH "C:/Qt/Qt5.9.3/5.9.3/msvc2017_64/bin")
SET(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)


add_executable(SvgMain  WIN32
	src/main.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}  SvgEditor)