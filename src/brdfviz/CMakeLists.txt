project(brdfviz)

add_executable(${PROJECT_NAME}
               main.cpp
               gui.cpp
               gui.h
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
               gl/uniformlocationpair.h)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC libbrdf libimgui glfw glad glm spdlog::spdlog freeimage)
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_precompile_headers(${PROJECT_NAME} PUBLIC pch.h)


pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
                      OUTPUT FORMAT errorfile
                      ANALYZE ${PROJECT_NAME}
                      MODE GA:1,2 OP
                      LOG ${PROJECT_NAME}.err)