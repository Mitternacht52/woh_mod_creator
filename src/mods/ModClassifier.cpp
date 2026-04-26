#include "ModClassifier.h"

#include <QStringList>

namespace woh::mods {

namespace {

QString matched_types_message(const QList<ModType>& matched_types) {
	QStringList names;
	names.reserve(matched_types.size());

	for (const auto& type : matched_types) {
		names.push_back(mod_type_to_string(type));
	}

	return names.join(QStringLiteral(", "));
}

} // namespace

ModClassifier::ModClassifier(const ModSchemaRegistry& registry) :
	m_registry(registry) {
}

ClassificationResult ModClassifier::classify(const woh::ito::ItoDocument& document) const {
	ClassificationResult result;

	for (const auto& schema : m_registry.schemas()) {
		if (schema->matches_signature(document)) {
			result.matched_types.push_back(schema->type());
		}
	}

	if (result.matched_types.empty()) {
		result.status = ClassificationStatus::unknown;
		result.type = ModType::unknown;
		result.message = QStringLiteral("Unknown mod type");

		return result;
	}

	if (result.matched_types.size() > 1) {
		result.status = ClassificationStatus::ambiguous;
		result.type = ModType::unknown;
		result.message = QStringLiteral("Ambiguous mod type: matched %1")
			             .arg(matched_types_message(result.matched_types));

		return result;
	}

	result.status = ClassificationStatus::success;
	result.type = result.matched_types.front();
	result.message = QStringLiteral("Mod type classified successfully");

	return result;
}

} //namespace woh::mods
