# target name
set(TARGET_NAME demo_BasicWidget)

# target configurations
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc *.ui)
add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES})

# library linking
target_link_libraries(${TARGET_NAME} PRIVATE
    DIQt
)
