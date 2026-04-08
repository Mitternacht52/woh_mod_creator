#pragma once

#include "ItoDocument.h"

#include <QString>
#include <QStringView>
#include <vector>

namespace woh::ito {

struct ItoParseError {
    qsizetype line = 0;
    QString message;

    [[nodiscard]] bool is_valid() const noexcept {
        return line >= 0 && !message.isEmpty();
    }
};

struct ItoParseResult {
    ItoDocument document;
    std::vector<ItoParseError> errors;

    [[nodiscard]] bool is_ok() const noexcept {
        return errors.empty();
    }
};

class ItoParser {
public:
    [[nodiscard]] ItoParseResult parse_text(QStringView text) const;
    [[nodiscard]] ItoParseResult parse_file(const QString& file_path) const;

private:
    static void add_error(ItoParseResult& result, qsizetype line, QString message);

    [[nodiscard]] static QString normalize_text(QStringView text);
    [[nodiscard]] static QStringView trim_view(QStringView text) noexcept;
    [[nodiscard]] static QStringView cleanup_line(QStringView line) noexcept;

    [[nodiscard]] static bool is_section_line(QStringView line) noexcept;
    [[nodiscard]] static QString parse_section_name(QStringView line);

    [[nodiscard]] static std::optional<ItoField> parse_field_line(
        QStringView line, 
        qsizetype line_number, 
        ItoParseResult& result);

    [[nodiscard]] static bool parse_quoted_value(
        QStringView value, 
        QString& out_value,
        QString& error_message);
};

} // namespace woh::ito