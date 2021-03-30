#pragma once

#include "container.hpp"

class OrderedList : public Container {
    public:
        OrderedList(
                function<void(string&)> lineParser, 
                function<shared_ptr<Container>(const string& line)> containerFetcher) : 
            Container(lineParser, containerFetcher) {

                this->hasStarted = false;
                this->hasFinished = false;
                this->tabWidth = 4;

            }
        
        static bool isStartingLine(const string &line) {
            return regex_match(line, regex("^\\d+\\. .*\n"));
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

            static regex lineRegex("^\\d+\\. ");
            line = regex_replace(line, lineRegex, "");

            if (not this->hasStarted) {
                line = "\\begin{enumerate}\n\t\\item " + line;
                this->hasStarted = true;
                return;
            }

            if (indentation >= tabWidth) {
                line = "\t" + line.substr(indentation - tabWidth);
                return;
            }

            if (line.empty()) {
                line = "\\end{enumerate}\n\n";
                this->hasFinished = true;
                return;
            }

            if (startingNewItem)
                line = "\t\\item " + line;
        }

    private:
        uint32_t tabWidth;
        bool hasStarted;
        bool hasFinished;
};
