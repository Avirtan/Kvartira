set(MySourcesList)
set(CMAKE_CXX_STANDARD 20)

if(APPLE) 
    
else()
file(GLOB ARRAY "render/vertexArray/*.cpp" "render/vertexArray/*.h")
file(GLOB MESH "render/mesh/*.cpp" "render/mesh/*.h")
file(GLOB SHADER "render/shader/*.cpp" "render/shader/*.h")
file(GLOB OBJECT "render/object/*.cpp" "render/object/*.h")
file(GLOB TEXTURE "render/texture/*.cpp" "render/texture/*.h")
endif()

file(GLOB SERVICE "services/*.cpp" "services/*.h")
file(GLOB DI "di/*.cpp" "di/*.h")
file(GLOB ECS "ecs/*.cpp" "ecs/*.h")

list(APPEND MySourcesList ${ECS})
list(APPEND MySourcesList ${DI})
list(APPEND MySourcesList ${SERVICE})

if(APPLE) 

else()
list(APPEND MySourcesList ${ARRAY})
list(APPEND MySourcesList ${SHADER})
list(APPEND MySourcesList ${OBJECT})
list(APPEND MySourcesList ${TEXTURE})
list(APPEND MySourcesList ${MESH})
endif()


add_library(core STATIC ${MySourcesList})
target_include_directories(core PUBLIC "./")
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(core PRIVATE glm::glm)
target_link_libraries(core PRIVATE SDL2::SDL2)
target_link_libraries(core PRIVATE GLEW::GLEW)


target_link_libraries(${PROJECT_NAME} PUBLIC core)
add_compile_definitions(GLEW_NO_GLU)
