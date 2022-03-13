# Qt cmake integration
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt version check
find_program(QMAKE_EXECUTABLE NAMES qmake HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

if(QT_VERSION LESS "5.0.0")
    set(QT_VERSION_MAJOR 4)
elseif(QT_VERSION LESS "6.0.0")
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Qt library linking
if (QT_VERSION_MAJOR EQUAL 4)
    find_package(Qt4 REQUIRED QtCore QtGui QtNetwork QtTest QAxServer)
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-") # fixing QString::toWCharArray in Qt4 + MSVC
    endif()
elseif (QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt5 REQUIRED Core Gui Widgets Network Quick Test AxServer PrintSupport Concurrent)
endif()

# c++ configurations
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# testing configurations
enable_testing()

# Doxygen configurations
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message("Doxygen not found. Documentations will not be built.")
endif()

# Unix configurations
if (UNIX)
    set(CMAKE_EXECUTABLE_SUFFIX .out) # set binary extension in unix systems
endif()

# Windows configurations
if (MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS) # supress warnings
endif()
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # export all symbols
