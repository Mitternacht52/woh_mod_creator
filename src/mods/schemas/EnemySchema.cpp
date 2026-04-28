#include "EnemySchema.h"

#include "mods/models/EnemyModModel.h"

#include <QSet>
#include <QStringList>

namespace woh::mods {

namespace {

const auto enemy_section = QStringLiteral("enemy");

const auto code_missing_section = QStringLiteral("mods.enemy.missing_section");
const auto code_duplicate_section = QStringLiteral("mods.enemy.duplicate_section");
const auto code_unknown_section = QStringLiteral("mods.enemy.unknown_section");
const auto code_missing_key = QStringLiteral("mods.enemy.missing_key");
const auto code_empty_required_key = QStringLiteral("mods.enemy.empty_required_key");
const auto code_unknown_key = QStringLiteral("mods.enemy.unknown_key");
const auto code_invalid_enum = QStringLiteral("mods.enemy.invalid_enum");
const auto code_invalid_integer = QStringLiteral("mods.enemy.invalid_integer");

const QSet<QString>& required_keys() {
	static const QSet<QString> keys = {
		QStringLiteral("name"),
		QStringLiteral("type"),
		QStringLiteral("location"),
		QStringLiteral("intro"),
		QStringLiteral("exp"),
		QStringLiteral("hit01"),
		QStringLiteral("hit02"),
		QStringLiteral("hit03"),
		QStringLiteral("art01"),
		QStringLiteral("art02"),
		QStringLiteral("artfreq"),
	};

	return keys;
}

const QSet<QString>& optional_keys() {
	static const QSet<QString> keys = {
		QStringLiteral("subtitle"),
		QStringLiteral("author"),
		QStringLiteral("can_run"),
		QStringLiteral("health"),
		QStringLiteral("power"),
		QStringLiteral("damagevalue"),
		QStringLiteral("damagetype"),
		QStringLiteral("prize_type"),
		QStringLiteral("prize_name"),
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

const QSet<QString>& enemy_types() {
	static const QSet<QString> values = {
		QStringLiteral("human"),
		QStringLiteral("monster"),
		QStringLiteral("ghost"),
		QStringLiteral("eldritch"),
		QStringLiteral("undead"),
		QStringLiteral("plant"),
		QStringLiteral("vampire"),
		QStringLiteral("circle"),
	};

	return values;
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
	};

	return values;
}

const QSet<QString>& damage_types() {
	static const QSet<QString> values = {
		QStringLiteral("STA"),
		QStringLiteral("REA"),
		QStringLiteral("ALL"),
		QStringLiteral("DOOM"),
	};

	return values;
}

const QSet<QString>& can_run_values() {
	static const QSet<QString> values = {
		QStringLiteral("0"),
		QStringLiteral("1"),
	};

	return values;
}

int count_enemy_sections(const woh::ito::ItoDocument& document) {
	int count = 0;

	for (const auto& section : document.sections) {
		if (section.name == enemy_section) {
			++count;
		}
	}

	return count;
}

const woh::ito::ItoSection* find_enemy_section(const woh::ito::ItoDocument& document) {
    return document.find_section(enemy_section);
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
        if (section.name != enemy_section) {
            report.add_error(
                QStringLiteral("Unknown section '%1' in enemy mod").arg(section.name),
                section.name,
                {},
                code_unknown_section
            );
        }
    }
}

void validate_duplicate_enemy_section(
    const woh::ito::ItoDocument& document,
    ValidationReport& report
) {
    if (count_enemy_sections(document) > 1) {
        report.add_error(
            QStringLiteral("Duplicate [enemy] section"),
            enemy_section,
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
                QStringLiteral("Unknown key '%1' in [enemy] section").arg(field.key),
                enemy_section,
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
            enemy_section,
            key_string,
            code_missing_key
        );

        return;
    }

    if (field->value.isEmpty()) {
        report.add_error(
            QStringLiteral("Required key '%1' must not be empty").arg(key_string),
            enemy_section,
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
        report.add_error(
            QStringLiteral("Enum key '%1' must not be empty").arg(key_string),
            enemy_section,
            key_string,
            code_invalid_enum
        );

        return;
    }

    if (!allowed_values.contains(field->value)) {
        report.add_error(
            QStringLiteral("Invalid enum value '%1' for key '%2'")
            .arg(field->value, key_string),
            enemy_section,
            key_string,
            code_invalid_enum
        );
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
            enemy_section,
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
            enemy_section,
            key_string,
            code_invalid_integer
        );
    }
}

void validate_enums(
    const woh::ito::ItoSection& section,
    ValidationReport& report
) {
    validate_enum_value(section, QStringLiteral("type"), enemy_types(), report);
    validate_enum_value(section, QStringLiteral("location"), locations(), report);
    validate_enum_value(section, QStringLiteral("can_run"), can_run_values(), report);
    validate_enum_value(section, QStringLiteral("damagetype"), damage_types(), report);
}

void validate_integers(
    const woh::ito::ItoSection& section,
    ValidationReport& report
) {
    validate_integer_value(section, QStringLiteral("health"), report);
    validate_integer_value(section, QStringLiteral("power"), report);
    validate_integer_value(section, QStringLiteral("damagevalue"), report);
    validate_integer_value(section, QStringLiteral("exp"), report);
    validate_integer_value(section, QStringLiteral("artfreq"), report);
}

} // namespace

ModType EnemySchema::type() const {
    return ModType::enemy;
}

QString EnemySchema::display_name() const {
    return QStringLiteral("Enemy");
}

bool EnemySchema::matches_signature(const woh::ito::ItoDocument& document) const {
    return document.has_section(enemy_section);
}

ValidationReport EnemySchema::validate(const woh::ito::ItoDocument& document) const {
    ValidationReport report;

    validate_unknown_sections(document, report);
    validate_duplicate_enemy_section(document, report);

    const auto* section = find_enemy_section(document);

    if (section == nullptr) {
        report.add_error(
            QStringLiteral("Missing required [enemy] section"),
            enemy_section,
            {},
            code_missing_section
        );

        return report;
    }

    validate_unknown_keys(*section, report);
    validate_required_keys(*section, report);
    validate_enums(*section, report);
    validate_integers(*section, report);

    return report;
}

std::unique_ptr<IModModel> EnemySchema::build_model(
    const woh::ito::ItoDocument& document
) const {
    const auto* section = find_enemy_section(document);

    if (section == nullptr) {
        return nullptr;
    }

    auto model = std::make_unique<EnemyModModel>();

    model->name = read_value_or_default(*section, QStringLiteral("name"), {});
    model->subtitle = read_value_or_default(*section, QStringLiteral("subtitle"), {});
    model->type_value = read_value_or_default(*section, QStringLiteral("type"), {});
    model->location = read_value_or_default(*section, QStringLiteral("location"), {});
    model->author = read_value_or_default(*section, QStringLiteral("author"), {});
    model->intro = read_value_or_default(*section, QStringLiteral("intro"), {});

    model->can_run = read_value_or_default(*section, QStringLiteral("can_run"), QStringLiteral("1"));
    model->health = read_value_or_default(*section, QStringLiteral("health"), QStringLiteral("10"));
    model->power = read_value_or_default(*section, QStringLiteral("power"), QStringLiteral("10"));
    model->damagevalue = read_value_or_default(*section, QStringLiteral("damagevalue"), QStringLiteral("1"));
    model->damagetype = read_value_or_default(*section, QStringLiteral("damagetype"), QStringLiteral("STA"));

    model->exp = read_value_or_default(*section, QStringLiteral("exp"), {});
    model->prize_type = read_value_or_default(*section, QStringLiteral("prize_type"), {});
    model->prize_name = read_value_or_default(*section, QStringLiteral("prize_name"), {});
    model->hit01 = read_value_or_default(*section, QStringLiteral("hit01"), {});
    model->hit02 = read_value_or_default(*section, QStringLiteral("hit02"), {});
    model->hit03 = read_value_or_default(*section, QStringLiteral("hit03"), {});
    model->art01 = read_value_or_default(*section, QStringLiteral("art01"), {});
    model->art02 = read_value_or_default(*section, QStringLiteral("art02"), {});
    model->artfreq = read_value_or_default(*section, QStringLiteral("artfreq"), {});

    return model;
}

} //namespace woh::mods
