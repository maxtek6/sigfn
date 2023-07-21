add_library(sigfn SHARED sigfn.c)

add_library(sigfn_a STATIC sigfn.c)

target_include_directories(sigfn PUBLIC ${SIGFN_INCLUDE})

target_include_directories(sigfn_a PUBLIC ${SIGFN_INCLUDE})