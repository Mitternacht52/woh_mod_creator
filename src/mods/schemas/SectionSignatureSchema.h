#pragma once

#include "mods/IModSchema.h"

#include <QString>

namespace woh::mods {

class SectionSignatureSchema final : public IModSchema {
public:
	SectionSignatureSchema(ModType type, QString display_name, QString primary_section);

	[[nodiscard]] ModType type() const override;
	[[nodiscard]] QString display_name() const override;

	[[nodiscard]] bool matches_signature(const woh::ito::ItoDocument& document) const override;
	[[nodiscard]] ValidationReport validate(const woh::ito::ItoDocument& document) const override;
	[[nodiscard]] std::unique_ptr<IModModel> build_model(const woh::ito::ItoDocument& document) const override;

private:
	ModType m_type = ModType::unknown;
	QString m_display_name;
	QString m_primary_section;
};

} //namespace woh::mods