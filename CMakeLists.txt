cmake_minimum_required(VERSION 3.5)

project(Calibration_ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )
find_package(OpenCV 4.5.1 REQUIRED)

include_directories(/home/tsokurenkosv/opencv_build/opencv/include/opencv2 ./widgets ./utils ./resources ./models ./controllers ./widgets/Dialogs)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        controllers/CameraController.cpp
        widgets/Dialogs/ArucoSaveMarkersDialog.cpp
        widgets/Dialogs/ArucoSaveCalibration.cpp
        widgets/Dialogs/ChArukoSaveCalibration.cpp
        widgets/Dialogs/ChArucoSaveBoardDialog.cpp

        widgets/ExternalCalibrationWidget.cpp
        widgets/InternalCalibrationWidget.cpp
        widgets/VideoFlowWidget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Calibration_ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Calibration_ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Calibration_ui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Calibration_ui
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_link_libraries(Calibration_ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS} opencv_core opencv_imgproc opencv_video)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Calibration_ui)
endif()
set_target_properties(Calibration_ui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Calibration_ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Calibration_ui)
endif()
