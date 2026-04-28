#include "ModSchemaRegistry.h"

#include "schemas/EnemySchema.h"
#include "schemas/EventSchema.h"
#include "schemas/SectionSignatureSchema.h"

namespace woh::mods {

ModSchemaRegistry::ModSchemaRegistry() {
	m_schemas.push_back(std::make_unique<SectionSignatureSchema>(
		ModType::character,
		QStringLiteral("Character"),
		QStringLiteral("character")
	));

	m_schemas.push_back(std::make_unique<EnemySchema>());

	m_schemas.push_back(std::make_unique<EventSchema>());

	m_schemas.push_back(std::make_unique<SectionSignatureSchema>(
		ModType::mystery,
		QStringLiteral("Mystery"),
		QStringLiteral("mystery")
	));
}

const QList<std::unique_ptr<IModSchema>>& ModSchemaRegistry::schemas() const {
	return m_schemas;
}

const IModSchema* ModSchemaRegistry::schema_for(ModType type) const {
	for (const auto& schema : m_schemas) {
		if (schema->type() == type) {
			return schema.get();
		}
	}

	return nullptr;
}

} //namespace woh::mods
