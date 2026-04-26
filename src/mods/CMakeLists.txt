add_library(woh_mods STATIC
	ModType.cpp
	ModType.h
	ClassificationResult.h
	ValidationIssue.h
	ValidationReport.cpp
	ValidationReport.h
	IModModel.h
	IModSchema.h
	ModLoadResult.cpp
	ModLoadResult.h
	ModSchemaRegistry.h
	ModSchemaRegistry.cpp
	ModClassifier.h
	ModClassifier.cpp
	schemas/SectionSignatureSchema.h
	schemas/SectionSignatureSchema.cpp
	models/EnemyModModel.h
	schemas/EnemySchema.h
	schemas/EnemySchema.cpp
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
