project(brdfviz)

set(FRONTEND_SRC
    gui.cpp
    gui.h

    embree/embreerenderer.cpp
    embree/embreerenderer.h
    )

set(SRC
    gl/vertexbufferobject.cpp
    gl/vertexbufferobject.h
    gl/vertex.h
    gl/vbos/cube.h
    gl/shader.cpp
    gl/shader.h
    gl/shaderloader.cpp
    gl/shaderloader.h
    gl/uniformlocations.h
    gl/camera.cpp
    gl/camera.h
    gl/camerainfo.h
    gl/transformation.cpp
    gl/transformation.h
    gl/objecttransformation.cpp
    gl/objecttransformation.h
    gl/cameratransformation.cpp
    gl/cameratransformation.h
    gl/material.cpp
    gl/material.h
    gl/color.h
    gl/texturetype.h
    gl/texture.cpp
    gl/texture.h
    gl/light.cpp
    gl/light.h
    gl/lightinfo.h
    gl/shaders/diffuseshader.cpp
    gl/shaders/diffuseshader.h
    gl/shaders/normalshader.cpp
    gl/shaders/normalshader.h
    gl/framebufferobject.cpp
    gl/framebufferobject.h
    gl/renderer.cpp
    gl/renderer.h
    gl/openglrenderer.cpp
    gl/openglrenderer.h
    gl/openglscene.cpp
    gl/openglscene.h
    gl/object.cpp
    gl/object.h
    gl/vbos/plane.h
    gl/linevertexbufferobject.cpp
    gl/linevertexbufferobject.h
    gl/vbos/gizmo.h
    gl/vbos/icosahedron.h
    gl/vbos/halficosahedron.h
    gl/icospherevertexbufferobject.cpp
    gl/icospherevertexbufferobject.h
    gl/shaders/brdfshader.cpp
    gl/shaders/brdfshader.h
    gl/uniformlocationpair.h
    gl/samplervisualizerobject.h
    gl/samplervisualizerobject.cpp

    embree/rtcshader.cpp
    embree/rtcshader.h
    embree/rtcamera.cpp
    embree/rtcamera.h
    embree/rtlight.cpp
    embree/rtlight.h
    embree/rtsphericalmap.cpp
    embree/rtsphericalmap.h
    embree/rtcrayhitior.h
    embree/pathtracerhelper.cpp
    embree/pathtracerhelper.h
    embree/sphere.cpp
    embree/sphere.h
    embree/mathscene.cpp
    embree/mathscene.h
    embree/rtccommonshader.cpp
    embree/rtccommonshader.h
    embree/rtcshadingtype.h

    common/utils/coordsystem.h
    common/utils/coordsystem.cpp
    common/utils/utils.h
    common/utils/rng.cpp
    common/utils/rng.h
    common/utils/timer.cpp
    common/utils/timer.h
    common/utils/math.h
    common/utils/rgb.cpp
    common/utils/rgb.h
    common/utils/assimploading.cpp
    common/utils/assimploading.h
    common/sampler.h
    common/sampler.cpp
    )


add_executable(${PROJECT_NAME}
               main.cpp

               ${IMGUI_SRC}
               ${IMGUI_FD_SRC}

               ${SRC}
               ${FRONTEND_SRC}
               )

add_executable(${PROJECT_NAME}_benchmark
               benchmark.cpp
               ${SRC})

target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}
                           ${IMGUI_FD_ROOT}
                           ${LIB_DIR}/imgui
                           ${LIB_DIR}/imgui/backends)

target_include_directories(${PROJECT_NAME}_benchmark
                           PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}
                           ${IMGUI_FD_ROOT}
                           ${LIB_DIR}/imgui
                           ${LIB_DIR}/imgui/backends)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad glm spdlog::spdlog freeimage assimp embree ${EMBREE_LIBRARY})
target_link_libraries(${PROJECT_NAME}_benchmark PUBLIC glfw glad glm spdlog::spdlog freeimage assimp embree ${EMBREE_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(${PROJECT_NAME}_benchmark PRIVATE NGL=1)
target_precompile_headers(${PROJECT_NAME} PUBLIC pch.h)
target_precompile_headers(${PROJECT_NAME}_benchmark PUBLIC pch.h)

target_compile_options(${PROJECT_NAME}
                       PUBLIC
                       -Wall
                       -Wextra
                       #                       -pedantic
                       #                       -Werror
                       -Wno-unknown-pragmas
                       )

target_compile_options(${PROJECT_NAME}_benchmark
                       PUBLIC
                       -Wall
                       -Wextra
                       #                       -pedantic
                       #                       -Werror
                       -Wno-unknown-pragmas
                       )

pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
                      OUTPUT FORMAT errorfile
                      ANALYZE ${PROJECT_NAME}
                      MODE GA:1,2 OP
                      LOG ${PROJECT_NAME}.err)