file(GLOB HEADERS "include/eosio/epn_plugin/*.hpp")
add_library( epn_plugin
             epn_plugin.cpp
             ${HEADERS} )

target_link_libraries( epn_plugin chain_plugin appbase fc )
target_include_directories( epn_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
