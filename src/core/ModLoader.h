#pragma once

#include <ito/ItoDocument.h>
#include <ito/ItoParser.h>
#include <mods/ModLoadResult.h>
#include <mods/IModSchema.h>
#include <mods/ModSchemaRegistry.h>
#include <mods/ModClassifier.h>

#include <QString>
#include <QStringView>

namespace woh::core {

class ModLoader {
public:
	ModLoader() = default;

	[[nodiscard]] woh::mods::ModLoadResult load_from_file(const QString& file_path) const;
	[[nodiscard]] woh::mods::ModLoadResult load_from_text(QString file_path, QStringView text) const;

private:
	[[nodiscard]] woh::mods::ModLoadResult load_from_document(
		QString file_path,
		const woh::ito::ItoDocument& document) const;
	[[nodiscard]] const woh::mods::IModSchema* resolve_schema(const woh::mods::ModType type) const;

	woh::ito::ItoParser m_parser;
	woh::mods::ModSchemaRegistry m_registry;
};

} //namespace woh::core
