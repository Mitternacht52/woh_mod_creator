#pragma once

#include "ItoDocument.h"

#include <QString>
#include <QStringView>

namespace woh::ito {

class ItoWriter {
public:
    [[nodiscard]] QString write_text(const ItoDocument& document) const;
    [[nodiscard]] bool write_file(const ItoDocument& document, const QString& filepath) const;

private:
    [[nodiscard]] static bool should_write_section(const ItoSection& section) noexcept;
    [[nodiscard]] static bool should_write_field(const ItoField& field) noexcept;

    static void write_section(QString& out, const ItoSection& section);
    static void write_field(QString& out, const ItoField& field);

    [[nodiscard]] static QString escape_value(QStringView value);
};

} // namespace woh::ito