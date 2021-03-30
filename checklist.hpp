#pragma once

#include "container.hpp"

class CheckList : public Container {
    public:
        CheckList(
                function<void(string&)> lineParser, 
                function<shared_ptr<Container>(const string& line)> containerFetcher) : 
            Container(lineParser, containerFetcher) {

                this->hasStarted = false;
                this->hasFinished = false;
                this->tabWidth = 4;

            }
        
        static bool isStartingLine(const string &line) {
            return regex_match(line, regex("^- \\[[X| ]\\] .*\n"));
        }
        
        bool isFinished() const override {
            return this->hasFinished;
        }

    protected:
        bool hasInlineBlocks() const override {
            return true;
        }

        bool canParseLines() const override {
            return true;
        }

        void parseBlock(string &line) override {
            bool startingNewItem = isStartingLine(line);
            uint32_t indentation = getIndentationWidth(line);

            static regex lineRegex("^(- )"),
                         emptyBox("^\\[ \\]"),
                         checkedBox("^\\[X\\]");
            
            static string emptyBoxReplacement = "[$\\square$] ",
                          checkedBoxReplacement = "[$\\boxtimes$] ";

            line = regex_replace(line, lineRegex, "");
            line = regex_replace(line, emptyBox, emptyBoxReplacement);
            line = regex_replace(line, checkedBox, checkedBoxReplacement);


            if (not this->hasStarted) {
                line = "\\begin{itemize}\n\t\\item" + line;
                this->hasStarted = true;
                return;
            }

            if (indentation >= tabWidth) {
                line = "\t" + line.substr(indentation - tabWidth);
                return;
            }

            if (line.empty()) {
                line = "\\end{itemize}\n\n";
                this->hasFinished = true;
                return;
            }

            if (startingNewItem)
                line = "\t\\item" + line;
        }

    private:
        uint32_t tabWidth;
        bool hasStarted;
        bool hasFinished;
};
