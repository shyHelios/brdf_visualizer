set(LIB_EMBREE_DIR "${LIB_DIR}/embree/")

set(EMBREE_TUTORIALS OFF CACHE INTERNAL "Enable to build Embree tutorials")

add_subdirectory(${LIB_EMBREE_DIR})