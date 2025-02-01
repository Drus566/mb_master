add_library(mem_manager OBJECT MemManager.cpp)

target_include_directories(mem_manager PUBLIC .)
target_link_libraries(mem_manager PUBLIC types)

