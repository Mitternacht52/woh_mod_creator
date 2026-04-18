#include <gtest/gtest.h>

#include <QDir>
#include <QFile>
#include <QTemporaryDir>

#include "ItoParser.h"
#include "ItoWriter.h"

using namespace woh::ito;

namespace {

ItoDocument make_sample_document() {
    return ItoDocument{
        .sections =
            {
                ItoSection{
                    .name = QStringLiteral("Main"),
                    .fields =
                        {
                            ItoField{
                                .key = QStringLiteral("name"),
                                .value = QStringLiteral("Alice"),
                            },
                            ItoField{
                                .key = QStringLiteral("path"),
                                .value = QStringLiteral("C:\\mods\\example"),
                            },
                        },
                },
                ItoSection{
                    .name = QStringLiteral("Other"),
                    .fields =
                        {
                            ItoField{
                                .key = QStringLiteral("quoted"),
                                .value = QStringLiteral("A \"quoted\" value"),
                            },
                        },
                },
            },
    };
}

void expect_documents_equal(const ItoDocument& actual, const ItoDocument& expected) {
    ASSERT_EQ(actual.sections.size(), expected.sections.size());

    for (qsizetype section_index = 0;
         section_index < static_cast<qsizetype>(expected.sections.size());
         ++section_index) {
        const ItoSection& actual_section = actual.sections[section_index];
        const ItoSection& expected_section = expected.sections[section_index];

        EXPECT_EQ(actual_section.name, expected_section.name);
        ASSERT_EQ(actual_section.fields.size(), expected_section.fields.size());

        for (qsizetype field_index = 0;
             field_index < static_cast<qsizetype>(expected_section.fields.size()); ++field_index) {
            const ItoField& actual_field = actual_section.fields[field_index];
            const ItoField& expected_field = expected_section.fields[field_index];

            EXPECT_EQ(actual_field.key, expected_field.key);
            EXPECT_EQ(actual_field.value, expected_field.value);
        }
    }
}

} // namespace

TEST(ItoIoTests, WriteTextProducesExpectedItoFormat) {
    const ItoWriter writer;
    const ItoDocument document = make_sample_document();

    const QString text = writer.write_text(document);

    EXPECT_EQ(text, QStringLiteral("[Main]\n"
                                   "name=\"Alice\"\n"
                                   "path=\"C:\\\\mods\\\\example\"\n"
                                   "\n"
                                   "[Other]\n"
                                   "quoted=\"A \\\"quoted\\\" value\""));
}

TEST(ItoIoTests, WriteFilePersistsDocumentContents) {
    const ItoWriter writer;
    const ItoDocument document = make_sample_document();
    QTemporaryDir temporary_dir;
    ASSERT_TRUE(temporary_dir.isValid());

    const QString file_path = QDir(temporary_dir.path()).filePath(QStringLiteral("sample.ito"));

    ASSERT_TRUE(writer.write_file(document, file_path));

    QFile file(file_path);
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));
    const QString written_text = QString::fromUtf8(file.readAll());

    EXPECT_EQ(written_text, writer.write_text(document));
}

TEST(ItoIoTests, ReadWriteReadRoundTripPreservesDocument) {
    const ItoParser parser;
    const ItoWriter writer;
    const QString source_text = QStringLiteral("[Main]\n"
                                               "name = \"Alice\"\n"
                                               "path = \"C:\\\\mods\\\\example\"\n"
                                               "\n"
                                               "[Other]\n"
                                               "quoted = \"A \\\"quoted\\\" value\"");

    const ItoParseResult first_parse = parser.parse_text(source_text);
    ASSERT_TRUE(first_parse.is_ok());

    const QString serialized_text = writer.write_text(first_parse.document);
    const ItoParseResult second_parse = parser.parse_text(serialized_text);

    ASSERT_TRUE(second_parse.is_ok());
    expect_documents_equal(second_parse.document, first_parse.document);
}

TEST(ItoIoTests, WriteFileThenReadFileRoundTripPreservesDocument) {
    const ItoParser parser;
    const ItoWriter writer;
    const ItoDocument document = make_sample_document();
    QTemporaryDir temporary_dir;
    ASSERT_TRUE(temporary_dir.isValid());

    const QString file_path = QDir(temporary_dir.path()).filePath(QStringLiteral("round_trip.ito"));

    ASSERT_TRUE(writer.write_file(document, file_path));

    const ItoParseResult parse_result = parser.parse_file(file_path);
    ASSERT_TRUE(parse_result.is_ok());
    expect_documents_equal(parse_result.document, document);
}
