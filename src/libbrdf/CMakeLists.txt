project(libbrdf)

set(SOURCES timer.h)

add_library(${PROJECT_NAME} timer.h timer.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

# TODO INTERFACE + OBJECTS
#add_library(${PROJECT_NAME} timer.h)
#add_library(${PROJECT_NAME}_objects timer.cpp)
#target_link_libraries(${PROJECT_NAME}_objects interface ${PROJECT_NAME})

pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
                      OUTPUT FORMAT errorfile
                      ANALYZE ${PROJECT_NAME}
                      MODE GA:1,2 OP
                      LOG ${PROJECT_NAME}.err)