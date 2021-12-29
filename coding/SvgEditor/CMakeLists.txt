cmake_minimum_required (VERSION 3.0)
#cmake 最低版本要求
project(SvgEditor)
#工程名字

set(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON) #编译rcc开关
SET(CMAKE_AUTOMOC ON) #编译moc开关
SET(CMAKE_BUILE_TYPE DEBUG) #指定编译类型

set(QT_PATH "E:/TOOL/QT/5.9.8/msvc2017_64/bin")
SET(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

MESSAGE(STATUS "Project Directory:${PROJECT_SOURCE_DIR}")
find_package(Qt5
	COMPONENTS  
	Core 
	Gui 
	Widgets 
	Svg 
	REQUIRED
)

add_library( SvgEditor  STATIC 
	Coding/shapebase.h
	Coding/shapebase.cpp
	Coding/kdrawline.h
	Coding/kdrawline.cpp
	Coding/kdrawrect.h
	Coding/kdrawrect.cpp
	Coding/kdrawellipse.h
	Coding/kdrawellipse.cpp
	Coding/kdrawfreeline.h
	Coding/kdrawfreeline.cpp
	Coding/kdrawingboard.h
	Coding/kdrawingboard.cpp
	Coding/kdrawhexagon.h
	Coding/kdrawhexagon.cpp
	Coding/kdrawtextbox.h
	Coding/kdrawtextbox.cpp
	Coding/kdrawtriangle.h
	Coding/kdrawtriangle.cpp
	Coding/kcustomwindow.h
	Coding/kcustomwindow.cpp
	Coding/mainwindow.h
	Coding/mainwindow.cpp
	res/image/res.qrc
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
