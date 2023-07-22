add_library(sigfn SHARED sigfn.c)

add_library(sigfn_a STATIC sigfn.c)

add_library(sigfn++ SHARED sigfn.c sigfn.cpp)

add_library(sigfn++_a STATIC sigfn.c sigfn.cpp)

target_include_directories(sigfn PUBLIC ${SIGFN_INCLUDE})

target_include_directories(sigfn_a PUBLIC ${SIGFN_INCLUDE})

target_include_directories(sigfn++ PUBLIC ${SIGFN_INCLUDE})

target_include_directories(sigfn++_a PUBLIC ${SIGFN_INCLUDE})