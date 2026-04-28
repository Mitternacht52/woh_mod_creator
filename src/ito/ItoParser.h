#pragma once

#include "ItoDiagnostics.h"
#include "ItoDocument.h"

#include <QString>
#include <QStringView>

namespace woh::ito {

struct ItoParseResult {
    ItoDocument document;
    ItoDiagnostics diagnostics;

    [[nodiscard]] bool is_ok() const noexcept {
        return !diagnostics.has_errors();
    }
};

class ItoParser {
public:
    [[nodiscard]] ItoParseResult parse_text(QStringView text) const;
    [[nodiscard]] ItoParseResult parse_file(const QString& file_path) const;

private:
    static void add_error(ItoParseResult& result, qsizetype line, ItoParserDiagnosticCode code,
                          QString message);

    [[nodiscard]] static QString normalize_text(QStringView text);
    [[nodiscard]] static QStringView trim_view(QStringView text) noexcept;
    [[nodiscard]] static QStringView cleanup_line(QStringView line) noexcept;

    [[nodiscard]] static bool is_section_line(QStringView line) noexcept;
    [[nodiscard]] static QString parse_section_name(QStringView line);

    [[nodiscard]] static std::optional<ItoField>
    parse_field_line(QStringView line, qsizetype line_number, ItoParseResult& result);

    [[nodiscard]] static bool parse_quoted_value(QStringView value, QString& out_value,
                                                 ItoParserDiagnosticCode& error_code,
                                                 QString& error_message);
};

} // namespace woh::ito