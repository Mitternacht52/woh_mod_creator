#pragma once

#include "ItoDiagnostics.h"
#include "ItoDocument.h"

#include <QString>
#include <QStringView>

namespace woh::ito {

class ItoWriter {
public:
    [[nodiscard]] QString write_text(const ItoDocument& document) const;
    [[nodiscard]] bool write_file(const ItoDocument& document, const QString& filepath) const;

private:
    static void collect_section_diagnostics(const ItoSection& section, qsizetype section_index,
                                            ItoDiagnostics& diagnostics);
    static void collect_field_diagnostics(const ItoField& field, qsizetype section_index,
                                          qsizetype field_index, QStringView section_name,
                                          ItoDiagnostics& diagnostics);

    [[nodiscard]] static bool should_write_section(const ItoSection& section) noexcept;
    [[nodiscard]] static bool should_write_field(const ItoField& field) noexcept;

    static void write_section(QString& out, const ItoSection& section);
    static void write_field(QString& out, const ItoField& field);

    [[nodiscard]] static QString escape_value(QStringView value);
};

} // namespace woh::ito
