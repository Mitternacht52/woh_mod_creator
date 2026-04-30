#pragma once

#include "ClassificationResult.h"
#include "ModSchemaRegistry.h"

#include "ito/ItoDocument.h"

namespace woh::mods {

	class ModClassifier {
	public:
		explicit ModClassifier(const ModSchemaRegistry& registry);

		[[nodiscard]] ClassificationResult classify(const woh::ito::ItoDocument& document) const;

	private:
		const ModSchemaRegistry& m_registry;
	};

} // namespace woh::mods
