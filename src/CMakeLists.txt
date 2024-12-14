add_library(sigfn SHARED ${SIGFN_SOURCES})

add_library(sigfn_a STATIC ${SIGFN_SOURCES})

target_include_directories(sigfn PUBLIC ${SIGFN_INCLUDE})

target_include_directories(sigfn_a PUBLIC ${SIGFN_INCLUDE})
