# AECM 

set(AECM_SOURCES
    aecm_core.cc
	aecm_core_c.cc
	complex_fft.c
	delay_estimator.cc
	delay_estimator_wrapper.cc
	real_fft.c
	ring_buffer.c
	signal_processing_library.cc
	echo_control_mobile.cc
)

add_library(aecm_h7 ${AECM_SOURCES})

target_include_directories(aecm_h7 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/WebRTC/aecm/inc
)


