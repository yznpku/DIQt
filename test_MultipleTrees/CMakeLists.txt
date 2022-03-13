# 目标名称
set(TARGET_NAME test_MultipleTrees)

# 目标设置
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc *.ui)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

# 链接依赖库
if (QT_VERSION_MAJOR EQUAL 4)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            DIQt
            Qt4::QtTest
    )
elseif (QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            DIQt
            Qt5::Test
    )
endif()

# 将源码复制到安装目录
install(
    DIRECTORY .
    DESTINATION ${TARGET_NAME}
)
