if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Adding embree as submodule for debug")
  set(LIB_EMBREE_DIR "${LIB_DIR}/embree/")
  
  set(EMBREE_TUTORIALS OFF CACHE INTERNAL "Enable to build Embree tutorials")
  
  add_subdirectory(${LIB_EMBREE_DIR})
else ()
  message("Looking for embree lib")
  find_package(Embree 3.12.1 REQUIRED)
endif ()
include_directories(${EMBREE_INCLUDE_DIRS})
