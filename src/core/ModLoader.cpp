#include "ModLoader.h"

namespace woh::core {

mods::ModLoadResult ModLoader::load_from_file(const QString& file_path) const {
	const auto parse_result = m_parser.parse_file(file_path);

	if (!parse_result.is_ok()) {
		return mods::ModLoadResult::parse_error(
			file_path,
			parse_result.diagnostics.summary()
		);
	}

	return load_from_document(file_path, parse_result.document);
}

mods::ModLoadResult ModLoader::load_from_text(QString file_path, QStringView text) const {
	const auto parse_result = m_parser.parse_text(text);

	if (!parse_result.is_ok()) {
		return mods::ModLoadResult::parse_error(
			file_path,
			parse_result.diagnostics.summary()
		);
	}

	return load_from_document(std::move(file_path), parse_result.document);
}

mods::ModLoadResult ModLoader::load_from_document(
	QString file_path,
	const ito::ItoDocument& document) const {
	mods::ModClassifier classifier(m_registry);
	const auto classification = classifier.classify(document);

	if (!classification.is_success()) {
		return mods::ModLoadResult::classification_error(
			std::move(file_path),
			classification,
			classification.message
		);
	}

	const mods::IModSchema* schema = resolve_schema(classification.type);
	if (schema == nullptr) {
		return mods::ModLoadResult::classification_error(
			std::move(file_path),
			classification,
			QStringLiteral("Schema for classified mod type was not found")
		);
	}

	const auto validation = schema->validate(document);
	if (!validation.is_valid()) {
		return mods::ModLoadResult::validation_error(
			std::move(file_path),
			classification,
			validation,
			QStringLiteral("Mod validation failed")
		);
	}

	std::unique_ptr<mods::IModModel> model = schema->build_model(document);
	if (!model) {
		return mods::ModLoadResult::validation_error(
			std::move(file_path),
			classification,
			validation,
			QStringLiteral("Schema failed to build mod model")
		);
	}

	return mods::ModLoadResult::success(
		std::move(file_path),
		classification,
		validation,
		std::move(model)
	);
}

const mods::IModSchema* ModLoader::resolve_schema(mods::ModType type) const {
	return m_registry.schema_for(type);
}

} //namespace woh::core