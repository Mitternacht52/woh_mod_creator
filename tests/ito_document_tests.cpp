#include <gtest/gtest.h>

#include "ItoDocument.h"

using namespace woh::ito;

TEST(ItoDocumentTests, FindFieldAndHasFieldWorkForMutableAndConstSection) {
    ItoSection section{
        .name = QStringLiteral("Main"),
        .fields = {
            ItoField{.key = QStringLiteral("KeyA"), .value = QStringLiteral("ValueA")},
            ItoField{.key = QStringLiteral("KeyB"), .value = QStringLiteral("ValueB")},
        },
    };

    ItoField* mutable_field = section.find_field(QStringLiteral("KeyB"));
    ASSERT_NE(mutable_field, nullptr);
    EXPECT_EQ(mutable_field->value, QStringLiteral("ValueB"));
    EXPECT_TRUE(section.has_field(QStringLiteral("KeyA")));
    EXPECT_FALSE(section.has_field(QStringLiteral("Missing")));

    const ItoSection& const_section = section;
    const ItoField* const_field = const_section.find_field(QStringLiteral("KeyA"));
    ASSERT_NE(const_field, nullptr);
    EXPECT_EQ(const_field->value, QStringLiteral("ValueA"));
    EXPECT_EQ(const_section.find_field(QStringLiteral("Missing")), nullptr);
}

TEST(ItoDocumentTests, FindSectionAndHasSectionWorkForMutableAndConstDocument) {
    ItoDocument document{
        .sections = {
            ItoSection{.name = QStringLiteral("First"), .fields = {}},
            ItoSection{.name = QStringLiteral("Second"), .fields = {}},
        },
    };

    ItoSection* mutable_section = document.find_section(QStringLiteral("Second"));
    ASSERT_NE(mutable_section, nullptr);
    EXPECT_EQ(mutable_section->name, QStringLiteral("Second"));
    EXPECT_TRUE(document.has_section(QStringLiteral("First")));
    EXPECT_FALSE(document.has_section(QStringLiteral("Missing")));

    const ItoDocument& const_document = document;
    const ItoSection* const_section = const_document.find_section(QStringLiteral("First"));
    ASSERT_NE(const_section, nullptr);
    EXPECT_EQ(const_section->name, QStringLiteral("First"));
    EXPECT_EQ(const_document.find_section(QStringLiteral("Missing")), nullptr);
}

TEST(ItoDocumentTests, ClearRemovesAllSectionsAndMarksDocumentAsEmpty) {
    ItoDocument document{
        .sections = {
            ItoSection{
                .name = QStringLiteral("Section"),
                .fields = {ItoField{.key = QStringLiteral("K"), .value = QStringLiteral("V")}},
            },
        },
    };

    ASSERT_FALSE(document.empty());
    ASSERT_TRUE(document.has_section(QStringLiteral("Section")));

    document.clear();

    EXPECT_TRUE(document.empty());
    EXPECT_EQ(document.find_section(QStringLiteral("Section")), nullptr);
}

