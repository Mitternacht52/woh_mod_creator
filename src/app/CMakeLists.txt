qt_add_executable(woh_mod_creator_app
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(woh_mod_creator_app
    PRIVATE
        Qt6::Widgets
        woh_core
        woh_ito
)

target_compile_features(woh_mod_creator_app PRIVATE cxx_std_20)

set_target_properties(woh_mod_creator_app PROPERTIES
    OUTPUT_NAME "woh_mod_creator"
)