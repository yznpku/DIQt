# target name
set(TARGET_NAME DIQt)

# target configurations
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc)
add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})

# library linking
if (QT_VERSION_MAJOR EQUAL 4)
    target_link_libraries(${TARGET_NAME} PUBLIC
        Qt4::QtCore
        Qt4::QtGui
    )
elseif (QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${TARGET_NAME} PUBLIC
        Qt5::Widgets
        Qt5::Quick
    )
endif()

# compile definitions
target_compile_definitions(${TARGET_NAME} PRIVATE
    DIQT_LIB
)

# include directories
target_include_directories(${TARGET_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
