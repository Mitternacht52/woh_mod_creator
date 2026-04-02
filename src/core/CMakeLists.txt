add_library(woh_core STATIC
    core_dummy.cpp
    core_dummy.h
)

target_link_libraries(woh_core
    PUBLIC
        Qt6::Core
        woh_ito    
)

target_compile_features(woh_core PUBLIC cxx_std_20)

target_include_directories(woh_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)