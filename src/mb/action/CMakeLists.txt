add_library(action_manager OBJECT
    ActionManager.cpp
    modbus/ModbusMaster.cpp
    request/DirectRequestManager.cpp
)

target_include_directories(action_manager PUBLIC .
                                                 request)

target_include_directories(action_manager PRIVATE modbus)

target_link_libraries(action_manager PUBLIC helpers 
                                            types
                                            config
                                            data_manager
                                            mem_manager
                                            modbus-static
                                            log)

