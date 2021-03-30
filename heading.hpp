#pragma once

#include "container.hpp"

using namespace std;

class Heading : public Container {
    public:
        Heading(
                function<void(string&)> lineParser,
                function<shared_ptr<Container>(const string &line)> containerFetcher
                ) : Container(lineParser, containerFetcher) { }

        static bool isStartingLine(const string &line) {
            return line.front() == '#';
        }

        bool isFinished() const override {
            return true;
        }

    protected:
        bool hasInlineBlocks() const override {
            return false;
        }

        bool canParseLines() const override {
            return false;
        }

        void parseBlock(string& line) override {
            /*
             * The vector for replacements are used as the number of "#" symbol decides the level.
             * Level (1) == # => \section
             * Level (2) == ## => \subsection
             * Level (3) == ### => \subsubsection
             *
             * From Level (4) onwards, the paragraph begins. So, it'll be good as marking as bold.
             */
            vector<string> replacements = {
                "\\section{$1}",
                "\\subsection{$1}",
                "\\subsubsection{$1}",
                "\\paragraph{$1}",
                "\\subparagraph{$1}"
            };

            for (int x = 0; x < replacements.size(); x++) {
                line = regex_replace(line, 
                        regex("^(?:#){" + to_string(x+1) + "} (.*)"), 
                        replacements[x]);
            }
        }
};
