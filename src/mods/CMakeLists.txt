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
	models/EnemyModModel.h
	models/EventModModel.h
	schemas/SectionSignatureSchema.h
	schemas/SectionSignatureSchema.cpp
	schemas/EnemySchema.h
	schemas/EnemySchema.cpp
	schemas/EventSchema.h
	schemas/EventSchema.cpp
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
