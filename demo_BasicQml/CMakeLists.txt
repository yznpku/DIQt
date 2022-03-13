if (QT_VERSION_MAJOR LESS 5)
    return()
endif()

# 目标名称
set(TARGET_NAME demo_BasicQml)

# 目标设置
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc *.ui)
add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES})

# 链接依赖库
target_link_libraries(${TARGET_NAME}
    PRIVATE
        DIQt
        Qt5::Quick
)

# 将源码复制到安装目录
install(
    DIRECTORY .
    DESTINATION ${TARGET_NAME}
)
