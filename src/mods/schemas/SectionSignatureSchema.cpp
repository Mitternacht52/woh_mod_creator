#include "SectionSignatureSchema.h"

#include <utility>

namespace woh::mods {

SectionSignatureSchema::SectionSignatureSchema(ModType type, QString display_name,
                                               QString primary_section) :
	m_type(type),
	m_display_name(std::move(display_name)),
	m_primary_section(std::move(primary_section)) {
}

ModType SectionSignatureSchema::type() const {
	return m_type;
}

QString SectionSignatureSchema::display_name() const {
	return m_display_name;
}

bool SectionSignatureSchema::matches_signature(const woh::ito::ItoDocument& document) const {
	return document.has_section(m_primary_section);
}

ValidationReport SectionSignatureSchema::validate(const woh::ito::ItoDocument&) const {
	return {};
}

std::unique_ptr<IModModel> SectionSignatureSchema::build_model(const woh::ito::ItoDocument&) const {
	return nullptr;
}

} //namespace woh::mods
