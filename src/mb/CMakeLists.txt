add_subdirectory(data)
add_subdirectory(types)
add_subdirectory(config)
add_subdirectory(action)
add_subdirectory(mem)

add_library(mb OBJECT MB.cpp)

# Exported includes
target_include_directories(mb PUBLIC interface)

target_link_libraries(mb PRIVATE helpers 
                                 data_manager
                                 action_manager
                                 mem_manager
                                 config)
