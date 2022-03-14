cmake_minimum_required(VERSION 3.13)

# project attributes
project(DIQt)
set(PROJECT_LONGNAME "DIQt: Qt-based Dependency Injection Library")

# build options
option(${PROJECT_NAME}_BUILD_TESTS_AND_EXAMPLES "Build ${PROJECT_NAME} tests and examples" ON)  # build tests and examples
option(${PROJECT_NAME}_BUILD_DOCS "Build ${PROJECT_NAME} documentations" ON)  # build documentations

# common configurations
include(cmake/common.cmake)

add_subdirectory(DIQt)

# tests and examples
if (${PROJECT_NAME}_BUILD_TESTS_AND_EXAMPLES)
    add_subdirectory(demo_BasicWidget)
    add_subdirectory(demo_BasicQml)
    add_subdirectory(demo_Visualization)
    add_subdirectory(test_MultipleTrees)
endif()

## documentations
#if (${PROJECT_NAME}_BUILD_DOCS)
#    add_subdirectory(docs)
#endif()

