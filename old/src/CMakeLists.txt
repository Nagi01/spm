# QT settings
# see http://www.cmake.org/cmake/help/cmake-2-8-docs.html#module:FindQt4

# List of all the C++ source code files
SET(vtk_qt_framework_SRCS_CXX
	#gui/MainWindow.cpp
	#gui/MainWindowFunc.cpp
	#gui/MainWindowImageFunc.cpp
	#gui/MainWindowPolarisFunc.cpp
	#gui/MainWindowNavigationFunc.cpp
	#gui/VTK_FRE.cpp
	#Thread/PolarisThread.cpp
	#Thread/UDPThread.cpp
	#gui/OGLWidget.cpp
	#gui/MainWindowRegistrationFunc.cpp
	main.cpp
	qt_toolbox.cpp
	surgicaldetection.cpp
	surgicalposition.cpp
	surgicalIdentification.cpp
	#qcvwidget.cpp
)

# List of all Qt header files
SET(vtk_qt_framework_MOC_HDRS
	#gui/MainWindow.h
	#ui_MainWindow.h
	#gui/OGLWidget.h
	#NavigtionStruct.h
	#Thread/PolarisThread.h
	#Thread/UDPThread.h
	#Thread/MotorThread.h
	#cu.h
	qt_toolbox.h
	surgicaldetection.h
	surgicalposition.h
	surgicalIdentification.h
)

# List of Qt .ui files
SET(vtk_qt_framework_UIS
	qt_toolbox.ui
)


# .ui files
QT5_WRAP_UI(vtk_qt_framework_SRCS_CXX ${vtk_qt_framework_UIS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} gui)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Multimedia_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${QT5MultimediaWidgets_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
add_definitions(${Qt5Multimedia_DEFINITIONS})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${QT5MultimediaWidgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# for moc
QT5_WRAP_CPP(vtk_qt_framework_SRCS_CXX ${vtk_qt_framework_MOC_HDRS})
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}  ${vtk_qt_framework_SRCS_CXX} ${vtk_qt_framework_MOC_HDRS})


set(CMAKE_SUPPRESS_REGENERATION true)

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} 
 ${Qt5Widgets_LIBRARIES}
 #${Qt5WebKitWidgets_LIBRARIES}
 ${Qt5Core_LIBRARIES}
 ${Qt5Network_LIBRARIES}
 ${Qt5Gui_LIBRARIES}
 ${Qt5Multimedia_LIBRARIES}
 ${QT5MultimediaWidgets_LIBRARIES}
 ${QT5OpenGL_LIBRARIES}
 # ${Qt5Sql_LIBRARIES}
 # ${Qt5PrintSupport_LIBRARIES}
 ${VTK_LIBRARIES} 
#${ITK_LIBRARIES}
)

INSTALL(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
if(OpenCV_FOUND)
	include_directories(${OpenCV_INCLUDE_DIRS})
	link_directories(${OpenCV_LIB_DIRS})
endif(OpenCV_FOUND)

QT5_USE_MODULES (SurgicalProcessIdentificationSystem Multimedia MultimediaWidgets)
target_link_libraries(SurgicalProcessIdentificationSystem ${OpenCV_LIBS})
target_link_libraries(SurgicalProcessIdentificationSystem ${ITK_LIBRARIES})
target_link_libraries(SurgicalProcessIdentificationSystem OpenIGTLink)

