add_executable(bridge_setup BridgeSetup.cpp)
set_property(TARGET bridge_setup PROPERTY CXX_STANDARD 14)
set_property(TARGET bridge_setup PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(bridge_setup hueplusplusstatic)

add_executable(lights_off LightsOff.cpp)
set_property(TARGET lights_off PROPERTY CXX_STANDARD 14)
set_property(TARGET lights_off PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(lights_off hueplusplusstatic)


add_executable(username_config UsernameConfig.cpp)
set_property(TARGET lights_off PROPERTY CXX_STANDARD 14)
set_property(TARGET lights_off PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(username_config hueplusplusstatic)

add_custom_target(hueplusplus_examples)
add_dependencies(hueplusplus_examples bridge_setup lights_off username_config)

# Snippets for documentation, not included with the examples target
add_executable(hueplusplus_snippets Snippets.cpp)
set_property(TARGET hueplusplus_snippets PROPERTY CXX_STANDARD 14)
set_property(TARGET hueplusplus_snippets PROPERTY CXX_EXTENSIONS OFF)
target_link_libraries(hueplusplus_snippets hueplusplusstatic)
