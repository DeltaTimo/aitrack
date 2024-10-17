SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.30.5)

project(aitrack VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(onnxruntime REQUIRED)
find_package(OpenMP REQUIRED)
find_package(OpenCV COMPONENTS opencv_core REQUIRED)
find_package(OpenCV COMPONENTS opencv_imgproc REQUIRED)
find_package(OpenCV COMPONENTS opencv_video REQUIRED)
find_package(OpenCV COMPONENTS opencv_videoio REQUIRED)
find_package(LIBUSB REQUIRED)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS UiTools REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Quick REQUIRED)

add_executable(aitrack
               Client/src/presenter/presenter.cpp
               Client/src/tracker/TrackerWrapper.cpp
               Client/src/tracker/TrackerFactory.cpp
               Client/src/camera/CameraSettings.cpp
               Client/src/camera/NullCamera.cpp
               Client/src/camera/Ps3Camera.cpp
               Client/src/camera/CameraFactory.cpp
               Client/src/camera/OCVCamera.cpp
               Client/src/Main.cpp
               Client/src/view/Resource.qrc
               Client/src/view/HeadCalibrationWindow.ui
               Client/src/view/HeadCalibrationWindow.cpp
               Client/src/view/ConfigWindow.ui
               Client/src/view/ConfigWindow.cpp
               Client/src/view/MainWindow.ui
               Client/src/view/WindowMain.cpp
               Client/src/model/UDPSender.cpp
               Client/src/model/Config.cpp
               Client/src/model/UpdateChecker.cpp
               Client/res/Resource.qrc
               AITracker/src/data.cpp
               AITracker/src/imageprocessor.cpp
               AITracker/src/model.cpp
               AITracker/src/filters.cpp
               AITracker/src/PositionSolver.cpp
               PS3Driver/src/ps3eye.h
               PS3Driver/src/ps3eye_capi.h
               PS3Driver/src/ps3eye.cpp
               PS3Driver/src/ps3eye_capi.cpp
)

message(OpenCV_Libs="${OpenCV_Libs}")
target_include_directories(aitrack PUBLIC AITracker/src PS3Driver/src ${OpenCV_INCLUDE_DIRS} ${onnxruntime_INCLUDE_DIRS} ${LIBUSB_INCLUDE_DIR} ${Qt5_INCLUDE_DIRS})
target_link_libraries(aitrack onnxruntime::onnxruntime opencv_core opencv_imgproc opencv_video opencv_videoio opencv_objdetect OpenMP::OpenMP_CXX ${OpenCV_Libs} ${LIBUSB_Libs} Qt5::Core Qt5::UiTools Qt5::Widgets Qt5::Quick)
