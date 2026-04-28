#include "EventSchema.h"

#include "mods/models/EventModModel.h"

#include <QSet>
#include <QStringList>

namespace woh::mods {

namespace {

const auto event_section = QStringLiteral("event");

const auto code_missing_section = QStringLiteral("mods.event.missing_section");
const auto code_duplicate_section = QStringLiteral("mods.event.duplicate_section");
const auto code_unknown_section = QStringLiteral("mods.event.unknown_section");
const auto code_missing_key = QStringLiteral("mods.event.missing_key");
const auto code_empty_required_key = QStringLiteral("mods.event.empty_required_key");
const auto code_unknown_key = QStringLiteral("mods.event.unknown_key");
const auto code_invalid_enum = QStringLiteral("mods.event.invalid_enum");
const auto code_invalid_integer = QStringLiteral("mods.event.invalid_integer");
const auto code_invalid_decimal = QStringLiteral("mods.event.invalid_decimal");
const auto code_forbidden_option_block = QStringLiteral("mods.event.forbidden_option_block");
const auto code_invalid_option_count = QStringLiteral("mods.event.invalid_option_count");
const auto code_invalid_prize_payload = QStringLiteral("mods.event.invalid_prize_payload");
const auto code_invalid_extra_prize_payload = QStringLiteral("mods.event.invalid_extra_prize_payload");

const QStringList& required_non_empty_base_keys() {
	static const QStringList keys = {
		QStringLiteral("name"),
		QStringLiteral("location"),
		QStringLiteral("author"),
		QStringLiteral("flavor"),
		QStringLiteral("options"),
		QStringLiteral("about"),
	};

	return keys;
}

const QStringList& required_may_be_empty_base_keys() {
	static const QStringList keys = {
		QStringLiteral("contact"),
		QStringLiteral("image"),
	};

	return keys;
}

const QStringList& optional_global_keys() {
	static const QStringList keys = {
		QStringLiteral("wavy_art"),
		QStringLiteral("wavy_speed"),
	};

	return keys;
}

const QStringList& option_required_non_empty_bases() {
	static const QStringList keys = {
		QStringLiteral("option"),
		QStringLiteral("test"),
		QStringLiteral("success"),
		QStringLiteral("winprize"),
		QStringLiteral("failprize"),
	};

	return keys;
}

const QStringList& option_required_may_be_empty_bases() {
	static const QStringList keys = {
		QStringLiteral("winnumber"),
		QStringLiteral("failure"),
		QStringLiteral("failnumber"),
	};

	return keys;
}

const QStringList& option_optional_bases() {
	static const QStringList keys = {
		QStringLiteral("wineffect"),
		QStringLiteral("faileffect"),
		QStringLiteral("extra_winprize"),
		QStringLiteral("extra_winnumber"),
		QStringLiteral("extra_failprize"),
		QStringLiteral("extra_failnumber"),
	};

	return keys;
}

const QSet<QString>& locations() {
	static const QSet<QString> values = {
		QStringLiteral("downtown"),
		QStringLiteral("school"),
		QStringLiteral("hospital"),
		QStringLiteral("seaside"),
		QStringLiteral("forest"),
		QStringLiteral("mansion"),
		QStringLiteral("village"),
		QStringLiteral("apartment"),
		QStringLiteral("ithotu"),
		QStringLiteral("athyola"),
		QStringLiteral("gozu"),
		QStringLiteral("atorasu"),
	};

	return values;
}

const QSet<QString>& option_counts() {
	static const QSet<QString> values = {
		QStringLiteral("1"),
		QStringLiteral("2"),
		QStringLiteral("3"),
	};

	return values;
}

const QSet<QString>& test_values() {
	static const QSet<QString> values = {
		QStringLiteral("strength"),
		QStringLiteral("dexterity"),
		QStringLiteral("perception"),
		QStringLiteral("knowledge"),
		QStringLiteral("charisma"),
		QStringLiteral("luck"),
		QStringLiteral("story"),
		QStringLiteral("funds1"),
		QStringLiteral("funds2"),
	};

	return values;
}

const QSet<QString>& prize_values() {
	static const QSet<QString> values = {
		QStringLiteral("stamina"),
		QStringLiteral("reason"),
		QStringLiteral("doom"),
		QStringLiteral("experience"),
		QStringLiteral("funds"),
		QStringLiteral("curse"),
		QStringLiteral("injury"),
		QStringLiteral("ally"),
		QStringLiteral("item"),
		QStringLiteral("spell"),
		QStringLiteral("itempool"),
	};

	return values;
}

const QSet<QString>& numeric_prize_values() {
	static const QSet<QString> values = {
		QStringLiteral("stamina"),
		QStringLiteral("reason"),
		QStringLiteral("doom"),
		QStringLiteral("experience"),
		QStringLiteral("funds"),
	};

	return values;
}

const QSet<QString>& random_prize_values() {
	static const QSet<QString> values = {
		QStringLiteral("curse"),
		QStringLiteral("injury"),
		QStringLiteral("ally"),
	};

	return values;
}

const QSet<QString>& itempool_values() {
	static const QSet<QString> values = {
		QStringLiteral("book"),
		QStringLiteral("magicitem"),
		QStringLiteral("mask"),
		QStringLiteral("poor"),
		QStringLiteral("ring"),
		QStringLiteral("dogshop"),
		QStringLiteral("villagedogshop"),
		QStringLiteral("vendingshop"),
		QStringLiteral("hardwareshop"),
		QStringLiteral("pharmacyshop"),
		QStringLiteral("magicshop"),
		QStringLiteral("hideout"),
	};

	return values;
}

const QSet<QString>& wavy_art_values() {
	static const QSet<QString> values = {
		QStringLiteral("0"),
		QStringLiteral("1"),
	};

	return values;
}

const QSet<QString>& effect_values() {
	static const QSet<QString> values = {
		QStringLiteral("bloodsplat"),
		QStringLiteral("whiteflash"),
	};

	return values;
}

const QSet<QString>& extra_prize_values() {
	static const QSet<QString> values = {
		QStringLiteral("stamina"),
		QStringLiteral("reason"),
		QStringLiteral("doom"),
		QStringLiteral("experience"),
	};

	return values;
}

QString suffix_key(QStringView base, QChar suffix) {
	QString key = base.toString();
	key.append(suffix);
	return key;
}

QStringList option_block_keys(QChar suffix) {
	QStringList keys;

	for (const auto& base : option_required_non_empty_bases()) {
		keys.push_back(suffix_key(base, suffix));
	}

	for (const auto& base : option_required_may_be_empty_bases()) {
		keys.push_back(suffix_key(base, suffix));
	}

	for (const auto& base : option_optional_bases()) {
		keys.push_back(suffix_key(base, suffix));
	}

	return keys;
}

const QSet<QString>& allowed_keys() {
	static const QSet<QString> keys = [] {
		QSet<QString> values;

		for (const auto& key : required_non_empty_base_keys()) {
			values.insert(key);
		}

		for (const auto& key : required_may_be_empty_base_keys()) {
			values.insert(key);
		}

		for (const auto& key : optional_global_keys()) {
			values.insert(key);
		}

		for (const auto suffix : { u'a', u'b', u'c' }) {
			for (const auto& key : option_block_keys(suffix)) {
				values.insert(key);
			}
		}

		return values;
		}();

	return keys;
}

int count_event_sections(const woh::ito::ItoDocument& document) {
	int count = 0;

	for (const auto& section : document.sections) {
		if (section.name == event_section) {
			++count;
		}
	}

	return count;
}

const woh::ito::ItoSection* find_event_section(const woh::ito::ItoDocument& document) {
	return document.find_section(event_section);
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

QString read_value_or_empty(const woh::ito::ItoSection& section, QStringView key) {
	return read_value_or_default(section, key, {});
}

bool has_field(const woh::ito::ItoSection& section, QStringView key) {
	return find_field(section, key) != nullptr;
}

bool is_integer(QStringView value) {
	bool ok = false;
	value.toString().toInt(&ok);
	return ok;
}

bool is_decimal(QStringView value) {
	bool ok = false;
	value.toString().toDouble(&ok);
	return ok;
}

void validate_unknown_sections(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	for (const auto& section : document.sections) {
		if (section.name != event_section) {
			report.add_error(
				QStringLiteral("Unknown section '%1' in event mod").arg(section.name),
				section.name,
				{},
				code_unknown_section
			);
		}
	}
}

void validate_duplicate_event_section(
	const woh::ito::ItoDocument& document,
	ValidationReport& report
) {
	if (count_event_sections(document) > 1) {
		report.add_error(
			QStringLiteral("Duplicate [event] section"),
			event_section,
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
				QStringLiteral("Unknown key '%1' in [event] section").arg(field.key),
				event_section,
				field.key,
				code_unknown_key
			);
		}
	}
}

void validate_required_key_presence(
	const woh::ito::ItoSection& section,
	QStringView key,
	ValidationReport& report
) {
	if (has_field(section, key)) {
		return;
	}

	const QString key_string = key.toString();

	report.add_error(
		QStringLiteral("Missing required key '%1'").arg(key_string),
		event_section,
		key_string,
		code_missing_key
	);
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
			event_section,
			key_string,
			code_missing_key
		);

		return;
	}

	if (field->value.isEmpty()) {
		report.add_error(
			QStringLiteral("Required key '%1' must not be empty").arg(key_string),
			event_section,
			key_string,
			code_empty_required_key
		);
	}
}

void validate_required_base_keys(
	const woh::ito::ItoSection& section,
	ValidationReport& report
) {
	for (const auto& key : required_non_empty_base_keys()) {
		validate_required_key_non_empty(section, key, report);
	}

	for (const auto& key : required_may_be_empty_base_keys()) {
		validate_required_key_presence(section, key, report);
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
                event_section,
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
            event_section,
            key_string,
            code_invalid_enum
        );
    }
}

void validate_base_enums(
    const woh::ito::ItoSection& section,
    ValidationReport& report
) {
    validate_enum_value(section, QStringLiteral("location"), locations(), false, report);
    validate_enum_value(section, QStringLiteral("options"), option_counts(), false, report);
    validate_enum_value(section, QStringLiteral("wavy_art"), wavy_art_values(), false, report);
}

int read_options_count(const woh::ito::ItoSection& section) {
    const auto* field = find_field(section, QStringLiteral("options"));

    if (field == nullptr || field->value.isEmpty()) {
        return -1;
    }

    bool ok = false;
    const int value = field->value.toInt(&ok);

    if (!ok || value < 1 || value > 3) {
        return -1;
    }

    return value;
}

void validate_options_value(
    const woh::ito::ItoSection& section,
    ValidationReport& report
) {
    const auto* field = find_field(section, QStringLiteral("options"));

    if (field == nullptr || field->value.isEmpty()) {
        return;
    }

    if (!option_counts().contains(field->value)) {
        report.add_error(
            QStringLiteral("Invalid option count '%1'").arg(field->value),
            event_section,
            QStringLiteral("options"),
            code_invalid_option_count
        );
    }
}

void validate_forbidden_option_block(
    const woh::ito::ItoSection& section,
    QChar suffix,
    ValidationReport& report
) {
    for (const auto& key : option_block_keys(suffix)) {
        if (!has_field(section, key)) {
            continue;
        }

        report.add_error(
            QStringLiteral("Option block '%1' is forbidden by options value")
            .arg(QString(suffix)),
            event_section,
            key,
            code_forbidden_option_block
        );
    }
}

void validate_option_block_required_keys(
    const woh::ito::ItoSection& section,
    QChar suffix,
    ValidationReport& report
) {
    for (const auto& base : option_required_non_empty_bases()) {
        validate_required_key_non_empty(section, suffix_key(base, suffix), report);
    }

    for (const auto& base : option_required_may_be_empty_bases()) {
        validate_required_key_presence(section, suffix_key(base, suffix), report);
    }
}

void validate_prize_payload(
    const woh::ito::ItoSection& section,
    QStringView prize_key,
    QStringView payload_key,
    ValidationReport& report
) {
    const auto* prize_field = find_field(section, prize_key);
    const auto* payload_field = find_field(section, payload_key);

    if (prize_field == nullptr || prize_field->value.isEmpty()) {
        return;
    }

    if (payload_field == nullptr) {
        return;
    }

    const QString prize = prize_field->value;
    const QString payload = payload_field->value;
    const QString payload_key_string = payload_key.toString();

    if (numeric_prize_values().contains(prize)) {
        if (payload.isEmpty() || !is_integer(payload)) {
            report.add_error(
                QStringLiteral("Prize '%1' requires integer payload in '%2'")
                .arg(prize, payload_key_string),
                event_section,
                payload_key_string,
                code_invalid_prize_payload
            );
        }

        return;
    }

    if (prize == QStringLiteral("item")) {
        if (payload.isEmpty()) {
            report.add_error(
                QStringLiteral("Prize 'item' requires item name in '%1'")
                .arg(payload_key_string),
                event_section,
                payload_key_string,
                code_invalid_prize_payload
            );
        }

        return;
    }

    if (prize == QStringLiteral("spell")) {
        return;
    }

    if (prize == QStringLiteral("itempool")) {
        if (!itempool_values().contains(payload)) {
            report.add_error(
                QStringLiteral("Prize 'itempool' has invalid payload '%1' in '%2'")
                .arg(payload, payload_key_string),
                event_section,
                payload_key_string,
                code_invalid_prize_payload
            );
        }

        return;
    }

    if (random_prize_values().contains(prize)) {
        return;
    }
}

void validate_extra_prize_payload(
    const woh::ito::ItoSection& section,
    QStringView prize_key,
    QStringView number_key,
    ValidationReport& report
) {
    const auto* prize_field = find_field(section, prize_key);
    const auto* number_field = find_field(section, number_key);

    const QString prize = prize_field == nullptr ? QString{} : prize_field->value;
    const QString number = number_field == nullptr ? QString{} : number_field->value;

    const QString prize_key_string = prize_key.toString();
    const QString number_key_string = number_key.toString();

    if (prize.isEmpty()) {
        if (!number.isEmpty()) {
            report.add_error(
                QStringLiteral("Extra prize number '%1' is set without '%2'")
                .arg(number_key_string, prize_key_string),
                event_section,
                number_key_string,
                code_invalid_extra_prize_payload
            );
        }

        return;
    }

    if (!extra_prize_values().contains(prize)) {
        report.add_error(
            QStringLiteral("Invalid extra prize value '%1' for key '%2'")
            .arg(prize, prize_key_string),
            event_section,
            prize_key_string,
            code_invalid_enum
        );

        return;
    }

    if (number.isEmpty() || !is_integer(number)) {
        report.add_error(
            QStringLiteral("Extra prize '%1' requires integer payload in '%2'")
            .arg(prize, number_key_string),
            event_section,
            number_key_string,
            code_invalid_extra_prize_payload
        );
    }
}

void validate_option_block(
    const woh::ito::ItoSection& section,
    QChar suffix,
    ValidationReport& report
) {
    validate_option_block_required_keys(section, suffix, report);

    const QString test_key = suffix_key(QStringLiteral("test"), suffix);
    const QString winprize_key = suffix_key(QStringLiteral("winprize"), suffix);
    const QString failprize_key = suffix_key(QStringLiteral("failprize"), suffix);
    const QString winnumber_key = suffix_key(QStringLiteral("winnumber"), suffix);
    const QString failnumber_key = suffix_key(QStringLiteral("failnumber"), suffix);
    const QString wineffect_key = suffix_key(QStringLiteral("wineffect"), suffix);
    const QString faileffect_key = suffix_key(QStringLiteral("faileffect"), suffix);
    const QString extra_winprize_key = suffix_key(QStringLiteral("extra_winprize"), suffix);
    const QString extra_winnumber_key = suffix_key(QStringLiteral("extra_winnumber"), suffix);
    const QString extra_failprize_key = suffix_key(QStringLiteral("extra_failprize"), suffix);
    const QString extra_failnumber_key = suffix_key(QStringLiteral("extra_failnumber"), suffix);

    validate_enum_value(section, test_key, test_values(), false, report);
    validate_enum_value(section, winprize_key, prize_values(), false, report);
    validate_enum_value(section, failprize_key, prize_values(), false, report);

    validate_enum_value(section, wineffect_key, effect_values(), true, report);
    validate_enum_value(section, faileffect_key, effect_values(), true, report);
    validate_enum_value(section, extra_winprize_key, extra_prize_values(), true, report);
    validate_enum_value(section, extra_failprize_key, extra_prize_values(), true, report);

    validate_prize_payload(section, winprize_key, winnumber_key, report);
    validate_prize_payload(section, failprize_key, failnumber_key, report);

    validate_extra_prize_payload(section, extra_winprize_key, extra_winnumber_key, report);
    validate_extra_prize_payload(section, extra_failprize_key, extra_failnumber_key, report);
}

void validate_active_option_blocks(
    const woh::ito::ItoSection& section,
    int options,
    ValidationReport& report
) {
    if (options >= 1) {
        validate_option_block(section, u'a', report);
    }

    if (options >= 2) {
        validate_option_block(section, u'b', report);
    }
    else {
        validate_forbidden_option_block(section, u'b', report);
    }

    if (options >= 3) {
        validate_option_block(section, u'c', report);
    }
    else {
        validate_forbidden_option_block(section, u'c', report);
    }
}

void validate_wavy_fields(
    const woh::ito::ItoSection& section,
    ValidationReport& report
) {
    const auto* wavy_speed = find_field(section, QStringLiteral("wavy_speed"));

    if (wavy_speed == nullptr) {
        return;
    }

    if (wavy_speed->value.isEmpty()) {
        report.add_error(
            QStringLiteral("Decimal key 'wavy_speed' must not be empty"),
            event_section,
            QStringLiteral("wavy_speed"),
            code_invalid_decimal
        );

        return;
    }

    if (!is_decimal(wavy_speed->value)) {
        report.add_error(
            QStringLiteral("Invalid decimal value '%1' for key 'wavy_speed'")
            .arg(wavy_speed->value),
            event_section,
            QStringLiteral("wavy_speed"),
            code_invalid_decimal
        );
    }
}

void populate_option_block(
    const woh::ito::ItoSection& section,
    QChar suffix,
    EventOptionBlock& block
) {
    block.option = read_value_or_empty(section, suffix_key(QStringLiteral("option"), suffix));
    block.test = read_value_or_empty(section, suffix_key(QStringLiteral("test"), suffix));
    block.success = read_value_or_empty(section, suffix_key(QStringLiteral("success"), suffix));
    block.winprize = read_value_or_empty(section, suffix_key(QStringLiteral("winprize"), suffix));
    block.winnumber = read_value_or_empty(section, suffix_key(QStringLiteral("winnumber"), suffix));
    block.failure = read_value_or_empty(section, suffix_key(QStringLiteral("failure"), suffix));
    block.failprize = read_value_or_empty(section, suffix_key(QStringLiteral("failprize"), suffix));
    block.failnumber = read_value_or_empty(section, suffix_key(QStringLiteral("failnumber"), suffix));

    block.wineffect = read_value_or_empty(section, suffix_key(QStringLiteral("wineffect"), suffix));
    block.faileffect = read_value_or_empty(section, suffix_key(QStringLiteral("faileffect"), suffix));
    block.extra_winprize = read_value_or_empty(section, suffix_key(QStringLiteral("extra_winprize"), suffix));
    block.extra_winnumber = read_value_or_empty(section, suffix_key(QStringLiteral("extra_winnumber"), suffix));
    block.extra_failprize = read_value_or_empty(section, suffix_key(QStringLiteral("extra_failprize"), suffix));
    block.extra_failnumber = read_value_or_empty(section, suffix_key(QStringLiteral("extra_failnumber"), suffix));
}

} // namespace

ModType EventSchema::type() const {
    return ModType::event;
}

QString EventSchema::display_name() const {
    return QStringLiteral("Event");
}

bool EventSchema::matches_signature(const woh::ito::ItoDocument& document) const {
    return document.has_section(event_section);
}

ValidationReport EventSchema::validate(const woh::ito::ItoDocument& document) const {
    ValidationReport report;

    validate_unknown_sections(document, report);
    validate_duplicate_event_section(document, report);

    const auto* section = find_event_section(document);

    if (section == nullptr) {
        report.add_error(
            QStringLiteral("Missing required [event] section"),
            event_section,
            {},
            code_missing_section
        );

        return report;
    }

    validate_unknown_keys(*section, report);
    validate_required_base_keys(*section, report);
    validate_base_enums(*section, report);
    validate_options_value(*section, report);

    const int options = read_options_count(*section);

    if (options != -1) {
        validate_active_option_blocks(*section, options, report);
    }

    validate_wavy_fields(*section, report);

    return report;
}

std::unique_ptr<IModModel> EventSchema::build_model(
    const woh::ito::ItoDocument& document
) const {
    const auto* section = find_event_section(document);

    if (section == nullptr) {
        return nullptr;
    }

    auto model = std::make_unique<EventModModel>();

    model->name = read_value_or_empty(*section, QStringLiteral("name"));
    model->location = read_value_or_empty(*section, QStringLiteral("location"));
    model->author = read_value_or_empty(*section, QStringLiteral("author"));
    model->contact = read_value_or_empty(*section, QStringLiteral("contact"));
    model->flavor = read_value_or_empty(*section, QStringLiteral("flavor"));
    model->options = read_value_or_empty(*section, QStringLiteral("options"));

    model->image = read_value_or_empty(*section, QStringLiteral("image"));
    model->about = read_value_or_empty(*section, QStringLiteral("about"));

    model->wavy_art = read_value_or_default(
        *section,
        QStringLiteral("wavy_art"),
        QStringLiteral("0")
    );

    model->wavy_speed = read_value_or_default(
        *section,
        QStringLiteral("wavy_speed"),
        QStringLiteral("0")
    );

    populate_option_block(*section, u'a', model->option_a);
    populate_option_block(*section, u'b', model->option_b);
    populate_option_block(*section, u'c', model->option_c);

    return model;
}

} //namespace woh::mods
