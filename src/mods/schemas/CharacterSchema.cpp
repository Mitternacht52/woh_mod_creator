#include "CharacterSchema.h"

#include "mods/models/CharacterModModel.h"

#include <QSet>

namespace woh::mods {

namespace {

const auto character_section = QStringLiteral("character");

const auto code_missing_section = QStringLiteral("mods.character.missing_section");
const auto code_duplicate_section = QStringLiteral("mods.character.duplicate_section");
const auto code_unknown_section = QStringLiteral("mods.character.unknown_section");
const auto code_missing_key = QStringLiteral("mods.character.missing_key");
const auto code_empty_required_key = QStringLiteral("mods.character.empty_required_key");
const auto code_unknown_key = QStringLiteral("mods.character.unknown_key");
const auto code_invalid_enum = QStringLiteral("mods.character.invalid_enum");
const auto code_invalid_integer = QStringLiteral("mods.character.invalid_integer");
const auto code_duplicate_perkpack = QStringLiteral("mods.character.duplicate_perkpack");

const QSet<QString>& required_keys() {
	static const QSet<QString> keys = {
		QStringLiteral("name"),
		QStringLiteral("author"),
		QStringLiteral("contact"),
		QStringLiteral("strength"),
		QStringLiteral("dexterity"),
		QStringLiteral("perception"),
		QStringLiteral("charisma"),
		QStringLiteral("knowledge"),
		QStringLiteral("luck"),
		QStringLiteral("sprite_icon"),
		QStringLiteral("sprite_back"),
		QStringLiteral("sprite_house"),
		QStringLiteral("portrait_a"),
		QStringLiteral("name_a"),
		QStringLiteral("portrait_b"),
		QStringLiteral("name_b"),
		QStringLiteral("menu_tag"),
		QStringLiteral("menu_desc"),
	};

	return keys;
}

const QSet<QString>& optional_keys() {
	static const QSet<QString> keys = {
		QStringLiteral("perkpack_a"),
		QStringLiteral("perkpack_b"),
	};

	return keys;
}

const QSet<QString>& allowed_keys() {
	static const QSet<QString> keys = [] {
		auto values = required_keys();

		for (const auto& key : optional_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QSet<QString>& perkpack_values() {
	static const QSet<QString> values = {
		QStringLiteral("[SKILLED BUTCHER]"),
		QStringLiteral("[HOUNDED ARTIST]"),
		QStringLiteral("[OBSESSIVE SHOPPER]"),
		QStringLiteral("[TAROT READER]"),
		QStringLiteral("[INNER STRENGTH]"),
		QStringLiteral("[PUSHING YOURSELF]"),
		QStringLiteral("[MAGIC CASTER]"),
		QStringLiteral("[LONE WOLF]"),
		QStringLiteral("[ESCAPE ARTIST]"),
		QStringLiteral("[CLOSE QUARTERS]"),
		QStringLiteral("[MAFIA MEMBER]"),
		QStringLiteral("[MILITARY BACKGROUND]"),
	};

	return values;
}

const QSet<QString>& stat_keys() {
	static const QSet<QString> keys = {
		QStringLiteral("strength"),
		QStringLiteral("dexterity"),
		QStringLiteral("perception"),
		QStringLiteral("charisma"),
		QStringLiteral("knowledge"),
		QStringLiteral("luck"),
	};

	return keys;
}

int count_character_sections(const woh::ito::ItoDocument& document) {
	int count = 0;

	for (const auto& section : document.sections) {
		if (section.name == character_section) {
			++count;
		}
	}

	return count;
}

const woh::ito::ItoSection* find_character_section(const woh::ito::ItoDocument& document) {
	return document.find_section(character_section);
}

const woh::ito::ItoField* find_field(const woh::ito::ItoSection& section, QStringView key) {
	return section.find_field(key);
}

QString read_value_or_default(
	const woh::ito::ItoSection& section,
	QStringView key,
	const QString& default_value
) {
	const auto* field = find_field(section, key);

	if (field == nullptr) {
		return default_value;
	}

	return field->value;
}

void validate_unknown_sections(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	for (const auto& section : document.sections) {
		if (section.name != character_section) {
			report.add_error(
				QStringLiteral("Unknown section '%1' in character mod").arg(section.name),
				section.name,
				{},
				code_unknown_section
			);
		}
	}
}

void validate_duplicate_character_section(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	if (count_character_sections(document) > 1) {
		report.add_error(
			QStringLiteral("Duplicate [character] section"),
			character_section,
			{},
			code_duplicate_section
		);
	}
}

void validate_unknown_keys(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	for (const auto& field : section.fields) {
		if (!allowed_keys().contains(field.key)) {
			report.add_error(
				QStringLiteral("Unknown key '%1' in [character] section").arg(field.key),
				character_section,
				field.key,
				code_unknown_key
			);
		}
	}
}

void validate_required_key(
	const woh::ito::ItoSection& section,
	QStringView key,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		report.add_error(
			QStringLiteral("Missing required key '%1'").arg(key_string),
			character_section,
			key_string,
			code_missing_key
		);

		return;
	}

	if (field->value.isEmpty()) {
		report.add_error(
			QStringLiteral("Required key '%1' must not be empty").arg(key_string),
			character_section,
			key_string,
			code_empty_required_key
		);
	}
}

void validate_required_keys(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	for (const auto& key : required_keys()) {
		validate_required_key(section, key, report);
	}
}

void validate_integer_value(
	const woh::ito::ItoSection& section,
	QStringView key,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		return;
	}

	if (field->value.isEmpty()) {
		report.add_error(
			QStringLiteral("Integer key '%1' must not be empty").arg(key_string),
			character_section,
			key_string,
			code_invalid_integer
		);

		return;
	}

	bool ok = false;
	field->value.toInt(&ok);

	if (!ok) {
		report.add_error(
			QStringLiteral("Invalid integer value '%1' for key '%2'")
			.arg(field->value, key_string),
			character_section,
			key_string,
			code_invalid_integer
		);
	}
}

void validate_stat_integers(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	for (const auto& key : stat_keys()) {
		validate_integer_value(section, key, report);
	}
}

void validate_enum_value(
	const woh::ito::ItoSection& section,
	QStringView key,
	const QSet<QString>& allowed_values,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		return;
	}

	if (field->value.isEmpty()) {
		return;
	}

	if (!allowed_values.contains(field->value)) {
		report.add_error(
			QStringLiteral("Invalid enum value '%1' for key '%2'")
			.arg(field->value, key_string),
			character_section,
			key_string,
			code_invalid_enum
		);
	}
}

void validate_perkpacks(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_enum_value(section, QStringLiteral("perkpack_a"), perkpack_values(), report);
	validate_enum_value(section, QStringLiteral("perkpack_b"), perkpack_values(), report);

	const auto first = read_value_or_default(section, QStringLiteral("perkpack_a"), {});
	const auto second = read_value_or_default(section, QStringLiteral("perkpack_b"), {});

	if (!first.isEmpty() && first == second) {
		report.add_error(
			QStringLiteral("perkpack_a and perkpack_b must not contain the same perk pack"),
			character_section,
			QStringLiteral("perkpack_b"),
			code_duplicate_perkpack
		);
	}
}

} // namespace

ModType CharacterSchema::type() const {
	return ModType::character;
}

QString CharacterSchema::display_name() const {
	return QStringLiteral("Character");
}

bool CharacterSchema::matches_signature(const woh::ito::ItoDocument& document) const {
	return document.has_section(character_section);
}

ValidationReport CharacterSchema::validate(const woh::ito::ItoDocument& document) const {
	ValidationReport report;

	validate_unknown_sections(document, report);
	validate_duplicate_character_section(document, report);

	const auto* section = find_character_section(document);

	if (section == nullptr) {
		report.add_error(
			QStringLiteral("Missing required [character] section"),
			character_section,
			{},
			code_missing_section
		);

		return report;
	}

	validate_unknown_keys(*section, report);
	validate_required_keys(*section, report);
	validate_stat_integers(*section, report);
	validate_perkpacks(*section, report);

	return report;
}

std::unique_ptr<IModModel> CharacterSchema::build_model(
	const woh::ito::ItoDocument& document
) const {
	const auto* section = find_character_section(document);

	if (section == nullptr) {
		return nullptr;
	}

	auto model = std::make_unique<CharacterModModel>();

	model->name = read_value_or_default(*section, QStringLiteral("name"), {});
	model->author = read_value_or_default(*section, QStringLiteral("author"), {});
	model->contact = read_value_or_default(*section, QStringLiteral("contact"), {});

	model->strength = read_value_or_default(*section, QStringLiteral("strength"), {});
	model->dexterity = read_value_or_default(*section, QStringLiteral("dexterity"), {});
	model->perception = read_value_or_default(*section, QStringLiteral("perception"), {});
	model->charisma = read_value_or_default(*section, QStringLiteral("charisma"), {});
	model->knowledge = read_value_or_default(*section, QStringLiteral("knowledge"), {});
	model->luck = read_value_or_default(*section, QStringLiteral("luck"), {});

	model->sprite_icon = read_value_or_default(*section, QStringLiteral("sprite_icon"), {});
	model->sprite_back = read_value_or_default(*section, QStringLiteral("sprite_back"), {});
	model->sprite_house = read_value_or_default(*section, QStringLiteral("sprite_house"), {});

	model->portrait_a = read_value_or_default(*section, QStringLiteral("portrait_a"), {});
	model->name_a = read_value_or_default(*section, QStringLiteral("name_a"), {});
	model->portrait_b = read_value_or_default(*section, QStringLiteral("portrait_b"), {});
	model->name_b = read_value_or_default(*section, QStringLiteral("name_b"), {});

	model->menu_tag = read_value_or_default(*section, QStringLiteral("menu_tag"), {});
	model->menu_desc = read_value_or_default(*section, QStringLiteral("menu_desc"), {});

	model->perkpack_a = read_value_or_default(*section, QStringLiteral("perkpack_a"), {});
	model->perkpack_b = read_value_or_default(*section, QStringLiteral("perkpack_b"), {});

	return model;
}

} //namespace woh::mods
