#include "ItoWriter.h"

#include <QFile>

namespace woh::ito 
{

QString ItoWriter::write_text(const ItoDocument& document) const {
    QString out;

    for (const auto& section : document.sections) {
        if (!should_write_section(section)) {
            continue;
        }

        if (!out.isEmpty()) {
            out += QStringLiteral("\n");
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
    if (!section.is_valid() || section.fields.empty()) {
        return false;
    }

    for (const auto& field : section.fields) {
        if (!should_write_field(field)) {
            return false;
        }
    }

    return true;
}

bool ItoWriter::should_write_field(const ItoField& field) noexcept {
    return field.is_valid();
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
