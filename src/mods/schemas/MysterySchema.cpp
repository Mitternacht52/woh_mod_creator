#include "MysterySchema.h"

#include "mods/models/MysteryModModel.h"

#include <QSet>
#include <QStringList>

namespace woh::mods {

namespace {

const auto mystery_section = QStringLiteral("mystery");
const auto intro_section = QStringLiteral("intro");
const auto progress_section = QStringLiteral("progress");
const auto restricted_locations_section = QStringLiteral("restricted_locations");
const auto card_a_section = QStringLiteral("card_a");
const auto card_b_section = QStringLiteral("card_b");
const auto ending_a_section = QStringLiteral("ending_a");
const auto ending_b_section = QStringLiteral("ending_b");
const auto ending_c_section = QStringLiteral("ending_c");
const auto big_ending_section = QStringLiteral("big_ending");

const auto code_missing_section = QStringLiteral("mods.mystery.missing_section");
const auto code_duplicate_section = QStringLiteral("mods.mystery.duplicate_section");
const auto code_unknown_section = QStringLiteral("mods.mystery.unknown_section");
const auto code_missing_key = QStringLiteral("mods.mystery.missing_key");
const auto code_empty_required_key = QStringLiteral("mods.mystery.empty_required_key");
const auto code_unknown_key = QStringLiteral("mods.mystery.unknown_key");
const auto code_invalid_enum = QStringLiteral("mods.mystery.invalid_enum");
const auto code_invalid_integer = QStringLiteral("mods.mystery.invalid_integer");
const auto code_invalid_progress_step = QStringLiteral("mods.mystery.invalid_progress_step");
const auto code_empty_big_ending = QStringLiteral("mods.mystery.empty_big_ending");

const QStringList& required_sections() {
	static const QStringList sections = {
		mystery_section,
		intro_section,
		progress_section,
		ending_a_section,
		big_ending_section,
	};

	return sections;
}

const QStringList& optional_sections() {
	static const QStringList sections = {
		restricted_locations_section,
		card_a_section,
		card_b_section,
		ending_b_section,
		ending_c_section,
	};

	return sections;
}

const QSet<QString>& allowed_sections() {
	static const QSet<QString> sections = [] {
		QSet<QString> values;

		for (const auto& section : required_sections()) {
			values.insert(section);
		}

		for (const auto& section : optional_sections()) {
			values.insert(section);
		}

		return values;
	}();

	return sections;
}

const QStringList& mystery_required_keys() {
	static const QStringList keys = {
		QStringLiteral("name"),
		QStringLiteral("author"),
		QStringLiteral("art"),
		QStringLiteral("description"),
	};

	return keys;
}

const QStringList& mystery_optional_keys() {
	static const QStringList keys = {
		QStringLiteral("custom_ui"),
		QStringLiteral("mystery_sound"),
		QStringLiteral("combat_sound"),
	};

	return keys;
}

const QSet<QString>& mystery_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : mystery_required_keys()) {
			values.insert(key);
		}

		for (const auto& key : mystery_optional_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QStringList& intro_required_keys() {
	static const QStringList keys = {
		QStringLiteral("art"),
		QStringLiteral("text_one"),
	};

	return keys;
}

const QStringList& intro_optional_keys() {
	static const QStringList keys = {
		QStringLiteral("text_two"),
		QStringLiteral("text_thr"),
		QStringLiteral("text_fou"),
	};

	return keys;
}

const QSet<QString>& intro_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : intro_required_keys()) {
			values.insert(key);
		}

		for (const auto& key : intro_optional_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QStringList& progress_step_prefixes() {
	static const QStringList prefixes = {
		QStringLiteral("one"),
		QStringLiteral("two"),
		QStringLiteral("thr"),
		QStringLiteral("fou"),
		QStringLiteral("fiv"),
		QStringLiteral("six"),
		QStringLiteral("sev"),
		QStringLiteral("eig"),
		QStringLiteral("nin"),
		QStringLiteral("ten"),
	};

	return prefixes;
}

const QStringList& progress_required_keys() {
	static const QStringList keys = {
		QStringLiteral("location"),
		QStringLiteral("background"),
	};

	return keys;
}

QString progress_step_key(QStringView prefix, QStringView suffix) {
	return QStringLiteral("%1_%2").arg(prefix, suffix);
}

QStringList progress_step_keys(QStringView prefix) {
	return {
		progress_step_key(prefix, QStringLiteral("loc")),
		progress_step_key(prefix, QStringLiteral("txt")),
		progress_step_key(prefix, QStringLiteral("frc")),
	};
}

const QSet<QString>& progress_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : progress_required_keys()) {
			values.insert(key);
		}

		for (const auto& prefix : progress_step_prefixes()) {
			for (const auto& key : progress_step_keys(prefix)) {
				values.insert(key);
			}
		}

		return values;
	}();

	return keys;
}

const QStringList& restricted_location_keys() {
	static const QStringList keys = {
		QStringLiteral("school"),
		QStringLiteral("hospital"),
		QStringLiteral("downtown"),
		QStringLiteral("seaside"),
		QStringLiteral("mansion"),
		QStringLiteral("forest"),
		QStringLiteral("village"),
		QStringLiteral("apartment"),
	};

	return keys;
}

const QSet<QString>& restricted_locations_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : restricted_location_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QStringList& card_required_keys() {
	static const QStringList keys = {
		QStringLiteral("name"),
		QStringLiteral("trigger"),
		QStringLiteral("art"),
		QStringLiteral("flavor"),
		QStringLiteral("tags"),
		QStringLiteral("a_effect"),
		QStringLiteral("b_effect"),
	};

	return keys;
}

const QStringList& card_optional_keys() {
	static const QStringList keys = {
		QStringLiteral("a_value"),
		QStringLiteral("b_value"),
	};

	return keys;
}

const QSet<QString>& card_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : card_required_keys()) {
			values.insert(key);
		}

		for (const auto& key : card_optional_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QStringList& ending_required_keys() {
	static const QStringList keys = {
		QStringLiteral("end_title"),
		QStringLiteral("end_img"),
		QStringLiteral("end_txta"),
	};

	return keys;
}

const QStringList& ending_optional_keys() {
	static const QStringList keys = {
		QStringLiteral("end_txtb"),
		QStringLiteral("end_txtc"),
		QStringLiteral("end_txtd"),
	};

	return keys;
}

const QSet<QString>& ending_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : ending_required_keys()) {
			values.insert(key);
		}

		for (const auto& key : ending_optional_keys()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QStringList& big_ending_allowed_key_list() {
	static const QStringList keys = {
		QStringLiteral("end_txt"),
		QStringLiteral("end_txta"),
		QStringLiteral("end_txtb"),
		QStringLiteral("end_txtc"),
	};

	return keys;
}

const QSet<QString>& big_ending_allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : big_ending_allowed_key_list()) {
			values.insert(key);
		}

		return values;
	}();

	return keys;
}

const QSet<QString>& binary_values() {
	static const QSet<QString> values = {
		QStringLiteral("0"),
		QStringLiteral("1"),
	};

	return values;
}

const QSet<QString>& progress_location_values() {
	static const QSet<QString> values = {
		QStringLiteral("downtown"),
		QStringLiteral("apartment"),
		QStringLiteral("school"),
		QStringLiteral("hospital"),
		QStringLiteral("seaside"),
		QStringLiteral("forest"),
		QStringLiteral("mansion"),
		QStringLiteral("village"),
		QStringLiteral("ending"),
	};

	return values;
}

int count_sections(const woh::ito::ItoDocument& document, QStringView section_name) {
	int count = 0;

	for (const auto& section : document.sections) {
		if (section.name == section_name) {
			++count;
		}
	}

	return count;
}

const woh::ito::ItoSection* find_section(
	const woh::ito::ItoDocument& document,
	QStringView section_name
) {
	return document.find_section(section_name);
}

const woh::ito::ItoField* find_field(const woh::ito::ItoSection& section, QStringView key) {
	return section.find_field(key);
}

bool has_field(const woh::ito::ItoSection& section, QStringView key) {
	return find_field(section, key) != nullptr;
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

QString read_value_or_empty(const woh::ito::ItoSection& section, QStringView key) {
	return read_value_or_default(section, key, {});
}

bool is_integer(QStringView value) {
	bool ok = false;
	value.toString().toInt(&ok);
	return ok;
}

void validate_unknown_sections(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	for (const auto& section : document.sections) {
		if (!allowed_sections().contains(section.name)) {
			report.add_error(
				QStringLiteral("Unknown section '%1' in mystery mod").arg(section.name),
				section.name,
				{},
				code_unknown_section
			);
		}
	}
}

void validate_duplicate_section(
	const woh::ito::ItoDocument& document,
	QStringView section_name,
	ValidationReport& report
) {
	if (count_sections(document, section_name) > 1) {
		report.add_error(
			QStringLiteral("Duplicate [%1] section").arg(section_name),
			section_name.toString(),
			{},
			code_duplicate_section
		);
	}
}

void validate_duplicate_sections(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	for (const auto& section_name : required_sections()) {
		validate_duplicate_section(document, section_name, report);
	}

	for (const auto& section_name : optional_sections()) {
		validate_duplicate_section(document, section_name, report);
	}
}

void validate_required_section(
	const woh::ito::ItoDocument& document,
	QStringView section_name,
	ValidationReport& report
) {
	if (document.has_section(section_name)) {
		return;
	}

	report.add_error(
		QStringLiteral("Missing required [%1] section").arg(section_name),
		section_name.toString(),
		{},
		code_missing_section
	);
}

void validate_required_sections(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	for (const auto& section_name : required_sections()) {
		validate_required_section(document, section_name, report);
	}
}

void validate_unknown_keys(
	const woh::ito::ItoSection& section,
	const QSet<QString>& allowed_keys,
	ValidationReport& report
) {
	for (const auto& field : section.fields) {
		if (!allowed_keys.contains(field.key)) {
			report.add_error(
				QStringLiteral("Unknown key '%1' in [%2] section").arg(field.key, section.name),
				section.name,
				field.key,
				code_unknown_key
			);
		}
	}
}

void validate_required_key_non_empty(
	const woh::ito::ItoSection& section,
	QStringView key,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		report.add_error(
			QStringLiteral("Missing required key '%1'").arg(key_string),
			section.name,
			key_string,
			code_missing_key
		);

		return;
	}

	if (field->value.isEmpty()) {
		report.add_error(
			QStringLiteral("Required key '%1' must not be empty").arg(key_string),
			section.name,
			key_string,
			code_empty_required_key
		);
	}
}

void validate_required_keys(
	const woh::ito::ItoSection& section,
	const QStringList& required_keys,
	ValidationReport& report
) {
	for (const auto& key : required_keys) {
		validate_required_key_non_empty(section, key, report);
	}
}

void validate_enum_value(
	const woh::ito::ItoSection& section,
	QStringView key,
	const QSet<QString>& allowed_values,
	bool allow_empty,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		return;
	}

	if (field->value.isEmpty()) {
		if (!allow_empty) {
			report.add_error(
				QStringLiteral("Enum key '%1' must not be empty").arg(key_string),
				section.name,
				key_string,
				code_invalid_enum
			);
		}

		return;
	}

	if (!allowed_values.contains(field->value)) {
		report.add_error(
			QStringLiteral("Invalid enum value '%1' for key '%2'")
			.arg(field->value, key_string),
			section.name,
			key_string,
			code_invalid_enum
		);
	}
}

void validate_integer_value(
	const woh::ito::ItoSection& section,
	QStringView key,
	bool allow_empty,
	ValidationReport& report
) {
	const auto* field = find_field(section, key);
	const QString key_string = key.toString();

	if (field == nullptr) {
		return;
	}

	if (field->value.isEmpty()) {
		if (!allow_empty) {
			report.add_error(
				QStringLiteral("Integer key '%1' must not be empty").arg(key_string),
				section.name,
				key_string,
				code_invalid_integer
			);
		}

		return;
	}

	if (!is_integer(field->value)) {
		report.add_error(
			QStringLiteral("Invalid integer value '%1' for key '%2'")
			.arg(field->value, key_string),
			section.name,
			key_string,
			code_invalid_integer
		);
	}
}

void validate_mystery_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, mystery_allowed_keys(), report);
	validate_required_keys(section, mystery_required_keys(), report);
}

void validate_intro_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, intro_allowed_keys(), report);
	validate_required_keys(section, intro_required_keys(), report);
}

void validate_progress_step(
	const woh::ito::ItoSection& section,
	QStringView prefix,
	ValidationReport& report
) {
	const auto loc_key = progress_step_key(prefix, QStringLiteral("loc"));
	const auto txt_key = progress_step_key(prefix, QStringLiteral("txt"));
	const auto frc_key = progress_step_key(prefix, QStringLiteral("frc"));

	const auto loc = read_value_or_empty(section, loc_key);
	const auto txt = read_value_or_empty(section, txt_key);
	const auto frc = read_value_or_empty(section, frc_key);

	const bool is_active = !loc.isEmpty() || !txt.isEmpty() || !frc.isEmpty();

	if (!is_active) {
		return;
	}

	if (loc.isEmpty()) {
		report.add_error(
			QStringLiteral("Active progress step '%1' is missing '%2'").arg(prefix, loc_key),
			section.name,
			loc_key,
			code_invalid_progress_step
		);
	}
	else if (!progress_location_values().contains(loc)) {
		report.add_error(
			QStringLiteral("Invalid progress location '%1' for key '%2'").arg(loc, loc_key),
			section.name,
			loc_key,
			code_invalid_enum
		);
	}

	if (txt.isEmpty()) {
		report.add_error(
			QStringLiteral("Active progress step '%1' is missing '%2'").arg(prefix, txt_key),
			section.name,
			txt_key,
			code_invalid_progress_step
		);
	}
}

void validate_progress_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, progress_allowed_keys(), report);
	validate_required_keys(section, progress_required_keys(), report);
	validate_enum_value(
		section,
		QStringLiteral("location"),
		progress_location_values(),
		false,
		report
	);

	for (const auto& prefix : progress_step_prefixes()) {
		validate_progress_step(section, prefix, report);
	}
}

void validate_restricted_locations_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, restricted_locations_allowed_keys(), report);

	for (const auto& key : restricted_location_keys()) {
		validate_enum_value(section, key, binary_values(), true, report);
	}
}

void validate_card_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, card_allowed_keys(), report);
	validate_required_keys(section, card_required_keys(), report);
	validate_integer_value(section, QStringLiteral("a_value"), true, report);
	validate_integer_value(section, QStringLiteral("b_value"), true, report);
}

void validate_ending_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, ending_allowed_keys(), report);
	validate_required_keys(section, ending_required_keys(), report);
}

void validate_big_ending_section(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	validate_unknown_keys(section, big_ending_allowed_keys(), report);

	bool has_any_text = false;

	for (const auto& key : big_ending_allowed_key_list()) {
		if (!read_value_or_empty(section, key).isEmpty()) {
			has_any_text = true;
			break;
		}
	}

	if (!has_any_text) {
		report.add_error(
			QStringLiteral("Section [big_ending] must contain at least one non-empty ending text"),
			section.name,
			{},
			code_empty_big_ending
		);
	}
}

void populate_progress_step(
	const woh::ito::ItoSection& section,
	QStringView prefix,
	MysteryProgressStep& step
) {
	step.loc = read_value_or_empty(section, progress_step_key(prefix, QStringLiteral("loc")));
	step.txt = read_value_or_empty(section, progress_step_key(prefix, QStringLiteral("txt")));
	step.frc = read_value_or_empty(section, progress_step_key(prefix, QStringLiteral("frc")));
}

void populate_card_block(const woh::ito::ItoSection& section, MysteryCardBlock& card) {
	card.name = read_value_or_empty(section, QStringLiteral("name"));
	card.trigger = read_value_or_empty(section, QStringLiteral("trigger"));
	card.art = read_value_or_empty(section, QStringLiteral("art"));
	card.flavor = read_value_or_empty(section, QStringLiteral("flavor"));
	card.tags = read_value_or_empty(section, QStringLiteral("tags"));
	card.a_effect = read_value_or_empty(section, QStringLiteral("a_effect"));
	card.a_value = read_value_or_empty(section, QStringLiteral("a_value"));
	card.b_effect = read_value_or_empty(section, QStringLiteral("b_effect"));
	card.b_value = read_value_or_empty(section, QStringLiteral("b_value"));
}

void populate_ending_block(const woh::ito::ItoSection& section, MysteryEndingBlock& ending) {
	ending.end_title = read_value_or_empty(section, QStringLiteral("end_title"));
	ending.end_img = read_value_or_empty(section, QStringLiteral("end_img"));
	ending.end_txta = read_value_or_empty(section, QStringLiteral("end_txta"));
	ending.end_txtb = read_value_or_empty(section, QStringLiteral("end_txtb"));
	ending.end_txtc = read_value_or_empty(section, QStringLiteral("end_txtc"));
	ending.end_txtd = read_value_or_empty(section, QStringLiteral("end_txtd"));
}

} // namespace

ModType MysterySchema::type() const {
	return ModType::mystery;
}

QString MysterySchema::display_name() const {
	return QStringLiteral("Mystery");
}

bool MysterySchema::matches_signature(const woh::ito::ItoDocument& document) const {
	return document.has_section(mystery_section);
}

ValidationReport MysterySchema::validate(const woh::ito::ItoDocument& document) const {
	ValidationReport report;

	validate_unknown_sections(document, report);
	validate_duplicate_sections(document, report);
	validate_required_sections(document, report);

	if (const auto* section = find_section(document, mystery_section)) {
		validate_mystery_section(*section, report);
	}

	if (const auto* section = find_section(document, intro_section)) {
		validate_intro_section(*section, report);
	}

	if (const auto* section = find_section(document, progress_section)) {
		validate_progress_section(*section, report);
	}

	if (const auto* section = find_section(document, restricted_locations_section)) {
		validate_restricted_locations_section(*section, report);
	}

	if (const auto* section = find_section(document, card_a_section)) {
		validate_card_section(*section, report);
	}

	if (const auto* section = find_section(document, card_b_section)) {
		validate_card_section(*section, report);
	}

	if (const auto* section = find_section(document, ending_a_section)) {
		validate_ending_section(*section, report);
	}

	if (const auto* section = find_section(document, ending_b_section)) {
		validate_ending_section(*section, report);
	}

	if (const auto* section = find_section(document, ending_c_section)) {
		validate_ending_section(*section, report);
	}

	if (const auto* section = find_section(document, big_ending_section)) {
		validate_big_ending_section(*section, report);
	}

	return report;
}

std::unique_ptr<IModModel> MysterySchema::build_model(
	const woh::ito::ItoDocument& document
) const {
	const auto* mystery = find_section(document, mystery_section);

	if (mystery == nullptr) {
		return nullptr;
	}

	auto model = std::make_unique<MysteryModModel>();

	model->name = read_value_or_empty(*mystery, QStringLiteral("name"));
	model->author = read_value_or_empty(*mystery, QStringLiteral("author"));
	model->art = read_value_or_empty(*mystery, QStringLiteral("art"));
	model->description = read_value_or_empty(*mystery, QStringLiteral("description"));
	model->custom_ui = read_value_or_empty(*mystery, QStringLiteral("custom_ui"));
	model->mystery_sound = read_value_or_empty(*mystery, QStringLiteral("mystery_sound"));
	model->combat_sound = read_value_or_empty(*mystery, QStringLiteral("combat_sound"));

	if (const auto* intro = find_section(document, intro_section)) {
		model->intro_block.art = read_value_or_empty(*intro, QStringLiteral("art"));
		model->intro_block.text_one = read_value_or_empty(*intro, QStringLiteral("text_one"));
		model->intro_block.text_two = read_value_or_empty(*intro, QStringLiteral("text_two"));
		model->intro_block.text_thr = read_value_or_empty(*intro, QStringLiteral("text_thr"));
		model->intro_block.text_fou = read_value_or_empty(*intro, QStringLiteral("text_fou"));
	}

	if (const auto* progress = find_section(document, progress_section)) {
		model->progress.location = read_value_or_empty(*progress, QStringLiteral("location"));
		model->progress.background = read_value_or_empty(*progress, QStringLiteral("background"));

		populate_progress_step(*progress, QStringLiteral("one"), model->progress.one);
		populate_progress_step(*progress, QStringLiteral("two"), model->progress.two);
		populate_progress_step(*progress, QStringLiteral("thr"), model->progress.thr);
		populate_progress_step(*progress, QStringLiteral("fou"), model->progress.fou);
		populate_progress_step(*progress, QStringLiteral("fiv"), model->progress.fiv);
		populate_progress_step(*progress, QStringLiteral("six"), model->progress.six);
		populate_progress_step(*progress, QStringLiteral("sev"), model->progress.sev);
		populate_progress_step(*progress, QStringLiteral("eig"), model->progress.eig);
		populate_progress_step(*progress, QStringLiteral("nin"), model->progress.nin);
		populate_progress_step(*progress, QStringLiteral("ten"), model->progress.ten);
	}

	if (const auto* restricted = find_section(document, restricted_locations_section)) {
		model->has_restricted_locations = true;
		model->restricted_locations.school = read_value_or_empty(*restricted, QStringLiteral("school"));
		model->restricted_locations.hospital = read_value_or_empty(*restricted, QStringLiteral("hospital"));
		model->restricted_locations.downtown = read_value_or_empty(*restricted, QStringLiteral("downtown"));
		model->restricted_locations.seaside = read_value_or_empty(*restricted, QStringLiteral("seaside"));
		model->restricted_locations.mansion = read_value_or_empty(*restricted, QStringLiteral("mansion"));
		model->restricted_locations.forest = read_value_or_empty(*restricted, QStringLiteral("forest"));
		model->restricted_locations.village = read_value_or_empty(*restricted, QStringLiteral("village"));
		model->restricted_locations.apartment = read_value_or_empty(*restricted, QStringLiteral("apartment"));
	}

	if (const auto* card_a = find_section(document, card_a_section)) {
		model->has_card_a = true;
		populate_card_block(*card_a, model->card_a);
	}

	if (const auto* card_b = find_section(document, card_b_section)) {
		model->has_card_b = true;
		populate_card_block(*card_b, model->card_b);
	}

	if (const auto* ending_a = find_section(document, ending_a_section)) {
		populate_ending_block(*ending_a, model->ending_a);
	}

	if (const auto* ending_b = find_section(document, ending_b_section)) {
		model->has_ending_b = true;
		populate_ending_block(*ending_b, model->ending_b);
	}

	if (const auto* ending_c = find_section(document, ending_c_section)) {
		model->has_ending_c = true;
		populate_ending_block(*ending_c, model->ending_c);
	}

	if (const auto* big_ending = find_section(document, big_ending_section)) {
		model->big_ending.end_txt = read_value_or_empty(*big_ending, QStringLiteral("end_txt"));
		model->big_ending.end_txta = read_value_or_empty(*big_ending, QStringLiteral("end_txta"));
		model->big_ending.end_txtb = read_value_or_empty(*big_ending, QStringLiteral("end_txtb"));
		model->big_ending.end_txtc = read_value_or_empty(*big_ending, QStringLiteral("end_txtc"));
	}

	return model;
}

} //namespace woh::mods
