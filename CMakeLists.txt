PROJECT(SurgicalProcessIdentificationSystem)

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

#Find_PACKAGE LIST
#openCV
find_package(OpenCV REQUIRED)
#ITK
find_package(ITK REQUIRED)
#OpenIGTLink
find_package(OpenIGTLink REQUIRED)
#VTK
find_package(VTK REQUIRED)
#QT
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

#Qt
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
#?
set(CMAKE_SUPPRESS_REGENERATION true)
#パッケージが見つかった場合のインクルードパスの設定
#opencv
include_directories(${OpenCV_INCLUDE_DIRS})
#VTK
include(${VTK_USE_FILE})
#ITK
include(${ITK_USE_FILE})
#openIGTLink
include(${OpenIGTLink_USE_FILE}) 
#eigen
include_directories("E:/SOURCE/eigen-eigen-b3f3d4950030")


# List of all the C++ source code files
SET(vtk_qt_framework_SRCS_CXX
	qt_toolbox.ui
	main.cpp
	qt_toolbox.cpp
	surgicaldetection.cpp
	surgicalposition.cpp
	surgicalIdentification.cpp
)
# List of all Qt header files
SET(vtk_qt_framework_MOC_HDRS
	qt_toolbox.h
	surgicaldetection.h
	surgicalposition.h
	surgicalIdentification.h
)
#上記SETしたものをexecutable
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}  ${vtk_qt_framework_SRCS_CXX} ${vtk_qt_framework_MOC_HDRS})

#install
install(FILES README.md LICENSE DESTINATION share/doc/${CMAKE_PROJECT_NAME}/)
INSTALL(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

#TARGET_LINK_LIBRARIES
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}
 Qt5::Widgets 
 Qt5::Core
 Qt5::Gui
 Qt5::Multimedia
 Qt5::MultimediaWidgets
 ${OpenCV_LIBS}
 ${VTK_LIBRARIES}
 ${ITK_LIBRARIES}
 OpenIGTLink
)
