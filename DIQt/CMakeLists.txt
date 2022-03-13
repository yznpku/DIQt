# 目标名称
set(TARGET_NAME DIQt)

# 目标设置
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.h *.qrc)
add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})

# 链接依赖库
if (QT_VERSION_MAJOR EQUAL 4)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
            Qt4::QtCore
            Qt4::QtGui
    )
elseif (QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
            Qt5::Widgets
            Qt5::Quick
    )
endif()

# 目标宏定义
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        DIQT_LIB
)

# 目标包含文件
target_include_directories(${TARGET_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
)

# 安装目标
install(
    TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_NAME}Targets
    INCLUDES DESTINATION include
)

# 安装包含文件
install(
    DIRECTORY include
    DESTINATION .
)

# 安装cmake配置
install(
    EXPORT ${TARGET_NAME}Targets
    FILE ${TARGET_NAME}Targets.cmake
    DESTINATION lib/cmake
)
