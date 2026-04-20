#include "ItoWriter.h"

#include <QFile>

namespace woh::ito {

void ItoWriter::collect_section_diagnostics(const ItoSection& section, qsizetype section_index,
                                            ItoDiagnostics& diagnostics) {
    if (section.name.isEmpty()) {
        diagnostics.add(ItoDiagnosticSeverity::error, ItoDiagnosticSource::writer,
                        ItoWriterDiagnosticCode::empty_section_name,
                        QStringLiteral("Section name is empty"),
                        ItoDiagnosticLocation{
                            .section_index = section_index,
                        });
    }

    if (section.fields.empty()) {
        diagnostics.add(ItoDiagnosticSeverity::error, ItoDiagnosticSource::writer,
                        ItoWriterDiagnosticCode::empty_section,
                        QStringLiteral("Section has no fields"),
                        ItoDiagnosticLocation{
                            .section_index = section_index,
                            .section_name = section.name,
                        });
    }

    for (qsizetype field_index = 0; field_index < static_cast<qsizetype>(section.fields.size());
         ++field_index) {
        collect_field_diagnostics(section.fields[static_cast<std::size_t>(field_index)],
                                  section_index, field_index, section.name, diagnostics);
    }
}

void ItoWriter::collect_field_diagnostics(const ItoField& field, qsizetype section_index,
                                          qsizetype field_index, QStringView section_name,
                                          ItoDiagnostics& diagnostics) {
    if (!field.key.isEmpty()) {
        return;
    }

    diagnostics.add(ItoDiagnosticSeverity::error, ItoDiagnosticSource::writer,
                    ItoWriterDiagnosticCode::empty_field_key,
                    QStringLiteral("Field key is empty"),
                    ItoDiagnosticLocation{
                        .section_index = section_index,
                        .field_index = field_index,
                        .section_name = section_name.toString(),
                    });
}

QString ItoWriter::write_text(const ItoDocument& document) const {
    QString out;

    for (const auto& section : document.sections) {
        if (!should_write_section(section)) {
            continue;
        }

        if (!out.isEmpty()) {
            out += QStringLiteral("\n\n");
        }

        write_section(out, section);
    }

    return out;
}

bool ItoWriter::write_file(const ItoDocument& document, const QString& filepath) const {
    const QString text = write_text(document);

    if (text.isEmpty() && !document.empty()) {
        return false;
    }

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    const QByteArray data = text.toUtf8();
    const qint64 written = file.write(data);

    return written == data.size();
}

bool ItoWriter::should_write_section(const ItoSection& section) noexcept {
    ItoDiagnostics diagnostics;
    collect_section_diagnostics(section, -1, diagnostics);

    for (const auto& field : section.fields) {
        if (!should_write_field(field)) {
            return false;
        }
    }

    return !diagnostics.has_errors();
}

bool ItoWriter::should_write_field(const ItoField& field) noexcept {
    ItoDiagnostics diagnostics;
    collect_field_diagnostics(field, -1, -1, {}, diagnostics);
    return !diagnostics.has_errors();
}

void ItoWriter::write_section(QString& out, const ItoSection& section) {
    out += u'[';
    out += section.name;
    out += QStringLiteral("]\n");

    auto it = section.fields.cbegin();
    auto end = section.fields.cend();

    write_field(out, *it);
    ++it;

    for (; it != end; ++it) {
        out += u'\n';
        write_field(out, *it);
    }
}

void ItoWriter::write_field(QString& out, const ItoField& field) {
    out += field.key;
    out += QStringLiteral("=\"");
    out += escape_value(field.value);
    out += u'"';
}

QString ItoWriter::escape_value(QStringView value) {
    QString escaped;
    escaped.reserve(value.size());

    for (const QChar ch : value) {
        if (ch == u'\\' || ch == u'"') {
            escaped += u'\\';
        }

        escaped += ch;
    }

    return escaped;
}

} // namespace woh::ito
