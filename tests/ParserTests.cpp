#include <gtest/gtest.h>

#include "HeaderParserLib/Parser.h"


TEST(ParserTests, EmptySource)
{
    std::string source = "";
    Parser parser;
    parser.Parse(source);

    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetNamespaces().empty());
}

TEST(ParserTests, ParseEmptyNamespace)
{
    std::string source = R"(
        namespace Namespace {

        }

    )";

    Parser parser;
    parser.Parse(source);

    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    ASSERT_TRUE(!parser.GetGblobalNamespace().GetNamespaces().empty());
}