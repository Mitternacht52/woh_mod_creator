#pragma once

#include <QString>
#include <QStringView>

#include <vector>

namespace woh::ito {

struct ItoField {
    QString key;
    QString value;

    [[nodiscard]] bool is_valid() const noexcept {
        return !key.isEmpty();
    }
};

struct ItoSection {
    QString name;
    std::vector<ItoField> fields;

    [[nodiscard]] bool is_valid() const noexcept {
        return !name.isEmpty();
    }

    [[nodiscard]] bool empty() const noexcept {
        return fields.empty();
    }

    ItoField* find_field(QStringView key) noexcept;
    const ItoField* find_field(QStringView key) const noexcept;

    [[nodiscard]] bool has_field(QStringView key) const noexcept {
        return find_field(key) != nullptr;
    }
};

struct ItoDocument {
    std::vector<ItoSection> sections;

    [[nodiscard]] bool empty() const noexcept {
        return sections.empty();
    }

    void clear() noexcept {
        sections.clear();
    }

    ItoSection* find_section(QStringView name) noexcept;
    const ItoSection* find_section(QStringView name) const noexcept;

    [[nodiscard]] bool has_section(QStringView name) const noexcept {
        return find_section(name) != nullptr;
    }
};

} // namespace woh::ito