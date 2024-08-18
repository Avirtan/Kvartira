cmake_minimum_required(VERSION 3.16)

project(game VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)

# message(${CMAKE_CXX_FLAGS_DEBUG})
# message(${CMAKE_CXX_FLAGS_RELEASE})


# поиск библиотек


set(MySourcesList "./src/main.cpp")

file(GLOB Manager "./include/managers/*.cpp" "./include/managers/*.hpp")
# file(GLOB Render "./include/render/*.cpp" "./include/render/*.hpp")
# file(GLOB Actors "./include/actors/*.cpp" "./include/actors/*.hpp")
# file(GLOB Components "./include/components/*.cpp" "./include/components/*.hpp")
# file(GLOB Ai "./include/ai/*.cpp" "./include/ai/*.h")
# file(GLOB Utils "./include/utils/*.cpp" "./include/utils/*.hpp")

list(APPEND MySourcesList ${Manager})
# list(APPEND MySourcesList ${Actors})
# list(APPEND MySourcesList ${Components})
# list(APPEND MySourcesList ${Ai})
# list(APPEND MySourcesList ${Render})
# list(APPEND MySourcesList ${Utils})

#создание запускаемого файла
add_executable(${PROJECT_NAME} ${MySourcesList})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# target_include_directories(${PROJECT_NAME} PUBLIC "./includes/")

# прекомпиляция заголовков
# target_precompile_headers(${PROJECT_NAME} PRIVATE "./includes/objects/Component.hpp" "./includes/objects/Actor.hpp")

add_subdirectory("./include/core")

# связывание библиотек
find_package(imgui CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(soil2 REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${PROJECT_NAME} PRIVATE soil2)

add_compile_definitions(GLEW_NO_GLU)

