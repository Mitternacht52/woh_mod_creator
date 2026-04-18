#include "ItoParser.h"

#include <QFile>
#include <utility>

namespace woh::ito {

void ItoParser::add_error(ItoParseResult& result, qsizetype line, QString message) {
    result.errors.push_back(ItoParseError{
        .line = line,
        .message = std::move(message),
    });
}

QString ItoParser::normalize_text(QStringView text) {
    QString normalized_text = text.toString();

    if (!normalized_text.isEmpty() && normalized_text.front() == QChar(0xFEFF)) {
        normalized_text.remove(0, 1);
    }

    normalized_text.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
    normalized_text.replace(u'\r', u'\n');

    return normalized_text;
}

QStringView ItoParser::trim_view(QStringView text) noexcept {
    return text.trimmed();
}

QStringView ItoParser::cleanup_line(QStringView line) noexcept {
    return trim_view(line);
}

bool ItoParser::is_section_line(QStringView line) noexcept {
    return line.size() >= 2 && line.front() == u'[' && line.back() == u']';
}

QString ItoParser::parse_section_name(QStringView line) {
    return trim_view(line.sliced(1, line.size() - 2)).toString();
}

bool ItoParser::parse_quoted_value(QStringView value, QString& out_value, QString& error_message) {
    const QStringView trimmed_value = trim_view(value);

    if (trimmed_value.size() < 2 || trimmed_value.front() != u'"' || trimmed_value.back() != u'"') {
        error_message = QStringLiteral("Field value must be quoted");
        return false;
    }

    out_value.clear();
    out_value.reserve(trimmed_value.size() - 2);

    bool is_escaped = false;

    for (qsizetype i = 1; i < trimmed_value.size() - 1; ++i) {
        const QChar ch = trimmed_value.at(i);

        if (is_escaped) {
            if (ch == u'"' || ch == u'\\') {
                out_value.push_back(ch);
            } else {
                out_value.push_back(u'\\');
                out_value.push_back(ch);
            }

            is_escaped = false;
            continue;
        }

        if (ch == u'\\') {
            is_escaped = true;
            continue;
        }

        if (ch == u'"') {
            error_message = QStringLiteral("Unescaped quote inside field value");
            return false;
        }

        out_value.push_back(ch);
    }

    if (is_escaped) {
        error_message = QStringLiteral("Unterminated escape sequence in field value");
        return false;
    }

    return true;
}

std::optional<ItoField> ItoParser::parse_field_line(QStringView line, qsizetype line_number,
                                                    ItoParseResult& result) {
    const qsizetype equal_pos = line.indexOf(u'=');
    if (equal_pos < 0) {
        add_error(result, line_number, QStringLiteral("Expected key=\"value\""));
        return std::nullopt;
    }

    const QStringView key_view = trim_view(line.sliced(0, equal_pos));
    const QStringView value_view = line.sliced(equal_pos + 1);

    if (key_view.isEmpty()) {
        add_error(result, line_number, QStringLiteral("Field key is empty"));
        return std::nullopt;
    }

    QString parsed_value;
    QString error_message;
    if (!parse_quoted_value(value_view, parsed_value, error_message)) {
        add_error(result, line_number, std::move(error_message));
        return std::nullopt;
    }

    return ItoField{
        .key = key_view.toString(),
        .value = std::move(parsed_value),
    };
}

ItoParseResult ItoParser::parse_file(const QString& file_path) const {
    ItoParseResult result;

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        add_error(result, 0, QStringLiteral("Failed to open file: %1").arg(file_path));
        return result;
    }

    const QByteArray data = file.readAll();
    return parse_text(QString::fromUtf8(data));
}

ItoParseResult ItoParser::parse_text(QStringView text) const {
    ItoParseResult result;

    const QString normalized_text = normalize_text(text);
    const QStringView normalized_view{normalized_text};

    ItoSection* current_section = nullptr;

    qsizetype line_start = 0;
    qsizetype line_number = 1;

    while (line_start <= normalized_view.size()) {
        qsizetype line_end = normalized_view.indexOf(u'\n', line_start);
        if (line_end < 0) {
            line_end = normalized_view.size();
        }

        const QStringView raw_line = normalized_view.sliced(line_start, line_end - line_start);
        const QStringView line = cleanup_line(raw_line);

        if (!line.isEmpty()) {
            if (is_section_line(line)) {
                const QString section_name = parse_section_name(line);
                if (section_name.isEmpty()) {
                    add_error(result, line_number, QStringLiteral("Empty section name"));
                    current_section = nullptr;
                } else {
                    result.document.sections.push_back(ItoSection{
                        .name = section_name,
                        .fields = {},
                    });

                    current_section = &result.document.sections.back();
                }
            } else {
                if (current_section == nullptr) {
                    add_error(result, line_number,
                              QStringLiteral("Field is outside of any section"));
                } else {
                    std::optional<ItoField> field = parse_field_line(line, line_number, result);

                    if (field.has_value()) {
                        current_section->fields.push_back(std::move(*field));
                    }
                }
            }
        }

        if (line_end == normalized_view.size()) {
            break;
        }

        line_start = line_end + 1;
        ++line_number;
    }

    return result;
}

} // namespace woh::ito
