#pragma once

#include <QString>
#include <QStringView>

#include <vector>

namespace woh::ito {

struct ItoField {
    QString key;
    QString value;

    [[nodiscard]] bool isValid() const noexcept {
        return !key.isEmpty();
    }
};

struct ItoSection {
    QString name;
    std::vector<ItoField> fields;

    [[nodiscard]] bool isValid() const noexcept {
        return !name.isEmpty();
    }

    [[nodiscard]] bool empty() const noexcept {
        return fields.empty();
    }

    ItoField* findField(QStringView key) noexcept;
    const ItoField* findField(QStringView key) const noexcept;

    [[nodiscard]] bool hasField(QStringView key) const noexcept {
        return findField(key) != nullptr;
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

    ItoSection* findSection(QStringView name) noexcept;
    const ItoSection* findSection(QStringView name) const noexcept;

    [[nodiscard]] bool hasSection(QStringView name) const noexcept {
        return findSection(name) != nullptr;
    }
};

}  // namespace woh::ito