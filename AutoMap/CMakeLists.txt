set(TARGET_NAME "AutoMap")
file(GLOB AUTO_MAP_H *.h)
file(GLOB AUTO_MAP_CPP *.cpp)

add_executable(${TARGET_NAME}  ${AUTO_MAP_H}  ${AUTO_MAP_CPP})
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)
include_directories(${PROJECT_SOURCE_DIR}/glm)

set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${TARGET_NAME}")

