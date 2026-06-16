add_library(woh_core STATIC
    ModLoader.h
    ModLoader.cpp
)

target_link_libraries(woh_core
    PUBLIC
        Qt6::Core
        woh_ito
        woh_mods
)

target_compile_features(woh_core PUBLIC cxx_std_20)

target_include_directories(woh_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)