add_library(woh_mods STATIC
	ModType.cpp
	ModType.h
	ClassificationResult.h
	ValidationIssue.h
	ValidationReport.cpp
	ValidationReport.h
	IModModel.h
	ModLoadResult.cpp
	ModLoadResult.h
)

target_link_libraries(woh_mods
	PUBLIC
		Qt6::Core
		woh_ito
)

target_include_directories(woh_mods
	PUBLIC
		${PROJECT_SOURCE_DIR}/src
)