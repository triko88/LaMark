#pragma once

#include "codeblock.hpp"
#include "heading.hpp"
#include "paragraph.hpp"
#include "checklist.hpp"
#include "orderedlist.hpp"
#include "unorderedlist.hpp"
#include "quote.hpp"

using namespace std;

class Parser {
    public:
        string parse(istream &markdown) {
            
        string result = "", line;
            shared_ptr<Container> currentContainer = nullptr;

            while (getline(markdown, line) or currentContainer) {

                if (not line.empty())
                    line += "\n";

                if (not currentContainer)
                    currentContainer = this->getContainerForLine(line);

                if (currentContainer) {
                    currentContainer->addLine(line);

                    if (currentContainer->isFinished()) {
                        result += currentContainer->getResult().str();
                        currentContainer = nullptr;
                    }
                }
            }

            return result;
       }

    private:
        shared_ptr<Container> getContainerForLine(const string &line) {
            shared_ptr<Container> container = nullptr;

            if (CodeBlock::isStartingLine(line)) {
                container = make_shared<CodeBlock>(nullptr, nullptr);
            }

            else if (Heading::isStartingLine(line)) {
                container = make_shared<Heading>(nullptr, nullptr);
            }

            else if (Quote::isStartingLine(line)) {
                container = make_shared<Quote>(
                        [this](string &line) { this->parseLine(line); },
                        nullptr
                        );
            }

            else if (CheckList::isStartingLine(line)) {
                container = this->createCheckList();
            }

            else if (OrderedList::isStartingLine(line)) {
                container = this->createOrderedList();
            }

            else if (UnorderedList::isStartingLine(line)) {
                container = this->createUnorderedList();
            }

            else if (Paragraph::isStartingLine(line)) {
                container = make_shared<Paragraph>([this](string &line){ this->parseLine(line); }
                        ,nullptr);
            }

            return container;
        }

        void parseLine(string &line) const {

            line = regex_replace(line, regex("\\!\\[([^\\]]*)\\]\\(([^\\]]*)\\)"), 
                    "\\begin{figure}\n\t\\includegraphics{$2}\n\t\\caption{$1}\n\\end{figure}");

            line = regex_replace(line, regex("\\[([^\\]]*)\\]\\(([^\\]]*)\\)"), "\\href{$2}{$1}");
            line = regex_replace(line, regex("(?!.*`.*)\\*\\*([^\\*\\*]+)\\*\\*(?!.*`.*)"), "\\textbf{$1}");
            line = regex_replace(line, regex("(?!.*`.*)__([^__]+)__(?!.*`.*)"), "\\textbf{$1}");
            line = regex_replace(line, regex("(?!.*`.*)\\*([^\\*].+)\\*(?!.*`.*)"), "\\textit{$1}");
            line = regex_replace(line, regex("(?!.*`.*)_([^_]+)_(?!.*`.*)"), "\\textit{$1}");
            line = regex_replace(line, regex("(?!.*`.*)~~([^~~]+)~~(?!.*`.*)"), "\\st{$1}");
            line = regex_replace(line, regex("`([^`]*)`"), "\\texttt{$1}");
        }

        shared_ptr<Container> createCheckList() const {
            return make_shared<CheckList>(
                    [this](string &line) { this->parseLine(line); },
                    [this](const string& line) {

                        shared_ptr<Container> container = nullptr;

                        if (CheckList::isStartingLine(line))
                            container = this->createCheckList();

                        return container;
                    }
                    );
        }

        shared_ptr<Container> createOrderedList() const {
            return make_shared<OrderedList>(
                    [this](string &line) { this->parseLine(line); },
                    [this](const string& line) {

                        shared_ptr<Container> container = nullptr;

                        if (OrderedList::isStartingLine(line))
                            container = this->createOrderedList();

                        return container;
                    }
                    );
        }

        shared_ptr<Container> createUnorderedList() const {
            return make_shared<UnorderedList>(
                    [this](string &line) { this->parseLine(line); },
                    [this](const string& line) {

                        shared_ptr<Container> container = nullptr;

                        if (UnorderedList::isStartingLine(line))
                            container = this->createUnorderedList();

                        return container;
                    }
                    );
        }
};
