add_library(woh_ito STATIC
    ItoDocument.cpp
    ItoDocument.h
    ItoParser.cpp
    ItoParser.h
)

target_link_libraries(woh_ito
    PUBLIC
        Qt6::Core
)

target_compile_features(woh_ito PUBLIC cxx_std_20)

target_include_directories(woh_ito
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
