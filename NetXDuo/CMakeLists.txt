# netxduo Library

set(netxduo_SOURCES
    App/app_netxduo.c
)

add_library(netxduo ${netxduo_SOURCES})

# BAA add: ${CMAKE_SOURCE_DIR}/Core/Inc
target_include_directories(netxduo PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/Target
   ${CMAKE_CURRENT_SOURCE_DIR}/App
   ${CMAKE_SOURCE_DIR}/Middlewares/netxduo/common/inc
   ${CMAKE_SOURCE_DIR}/Middlewares/threadx/ports/cortex_m7/gnu/inc
   ${CMAKE_SOURCE_DIR}/Middlewares/threadx/common/inc
   ${CMAKE_SOURCE_DIR}/Middlewares/netxduo/common/inc
   ${CMAKE_SOURCE_DIR}/Middlewares/netxduo/ports/cortex_m7/gnu/inc
   ${CMAKE_SOURCE_DIR}/Middlewares/netxduo/common/drivers/ethernet
   ${CMAKE_SOURCE_DIR}/Middlewares/netxduo/addons/dhcp
)

