set(MySourcesList)
set(CMAKE_CXX_STANDARD 20)

if(APPLE) 
    
else()
file(GLOB ARRAY "render/vertexArray/*.cpp" "render/vertexArray/*.h")
file(GLOB MESH "render/mesh/*.cpp" "render/mesh/*.h")
file(GLOB SHADER "render/shader/*.cpp" "render/shader/*.h")
file(GLOB OBJECT "render/object/*.cpp" "render/object/*.h")
file(GLOB TEXTURE "render/texture/*.cpp" "render/texture/*.h")
file(GLOB AUDIO "audio/*.cpp" "audio/*.h")
endif()

file(GLOB SERVICE "services/*.cpp" "services/*.h")
file(GLOB DI "di/*.cpp" "di/*.h")
file(GLOB ECS "ecs/*.cpp" "ecs/*.h")
file(GLOB Utils "utils/*.cpp" "utils/*.h")
file(GLOB Models "models/*/*.cpp" "models/*/*.h")

list(APPEND MySourcesList ${ECS})
list(APPEND MySourcesList ${DI})
list(APPEND MySourcesList ${SERVICE})
list(APPEND MySourcesList ${Utils})
list(APPEND MySourcesList ${Models})

if(APPLE) 

else()
list(APPEND MySourcesList ${ARRAY})
list(APPEND MySourcesList ${SHADER})
list(APPEND MySourcesList ${OBJECT})
list(APPEND MySourcesList ${TEXTURE})
list(APPEND MySourcesList ${MESH})
list(APPEND MySourcesList ${AUDIO})
endif()


add_library(core STATIC ${MySourcesList})
target_include_directories(core PUBLIC "./")
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(core PRIVATE glm::glm)
target_link_libraries(core PRIVATE SDL2::SDL2)
target_link_libraries(core PRIVATE GLEW::GLEW)

if(UNIX AND NOT APPLE)
    target_include_directories(core PUBLIC "./audio/linux/core/inc")
    target_include_directories(core PUBLIC "./audio/linux/studio/inc")
    target_link_directories(core PUBLIC "./audio/linux/core/lib/x86_64")
    target_link_directories(core PUBLIC "./audio/linux/studio/lib/x86_64")
    target_link_libraries(core PRIVATE fmodstudioL)
    target_link_libraries(core PRIVATE fmodL)
endif()

add_compile_definitions(GLEW_NO_GLU)
