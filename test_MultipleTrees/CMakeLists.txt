# target name
set(TARGET_NAME test_MultipleTrees)

# target configurations
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc *.ui)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

# library linking
if (QT_VERSION_MAJOR EQUAL 4)
    target_link_libraries(${TARGET_NAME} PRIVATE
        DIQt
        Qt4::QtTest
    )
elseif (QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${TARGET_NAME} PRIVATE
        DIQt
        Qt5::Test
    )
endif()
