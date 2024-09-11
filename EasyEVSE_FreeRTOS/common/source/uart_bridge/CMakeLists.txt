add_library(stx_driver_uart_bridge ${PROJECT_SOURCE_DIR}/helper/empty.c)
stx_unit(NAME uart_bridge_driver TARGET stx_driver_uart_bridge SOURCES stx_hal_uart_bridge.c HEADERS stx_hal_uart_bridge.h ACTIVE ON)

target_include_directories(stx_driver_uart_bridge
	PUBLIC .
	PRIVATE ../../products/v2g
	PRIVATE ../../target/imx93/include
	PRIVATE ../trgthw
	)

target_compile_definitions(stx_driver_uart_bridge
        PUBLIC TRGTHW_SUPPORTED=1
        )
