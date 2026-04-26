#pragma once

#include "IModSchema.h"
#include "ModType.h"

#include <memory>

#include <QList>

namespace woh::mods {

class ModSchemaRegistry {
public:
	ModSchemaRegistry();

	[[nodiscard]] const QList<std::unique_ptr<IModSchema>>& schemas() const;
	[[nodiscard]] const IModSchema* schema_for(ModType type) const;

private:
	QList<std::unique_ptr<IModSchema>> m_schemas;
};

} //namespace woh::mods
