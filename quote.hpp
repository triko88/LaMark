#pragma once

#include "container.hpp"

using namespace std;

class Quote : public Container {
    public:
        
        Quote(
                function<void(string&)> lineParser,
                function<shared_ptr<Container>(const string &line)> containerFetcher
                ):
            Container(lineParser, containerFetcher) {

                this->hasStarted = this->hasFinished = false;

            }

        static bool isStartingLine(const string& line) {
            return regex_match(line, regex("^\\> .*\n"));
        }

        bool isFinished() const override {
            return this->hasFinished;
        }

        void addLine(string &line) override {
            /*
             *  Overriding addLine() method as on testing this parser was clearing out lines.
             */

            if (not this->hasStarted) {
                this->result << "\\begin{quote}\n";
                this->hasStarted = true;
            }

            if (line.empty()) {
                this->result << "\\end{quote}\n\n";
                this->hasFinished = true;
                return;
            }

            this->parseBlock(line);

            this->result << line;
        }
    protected:
        void parseBlock(string &line) override {
            
            /* 
             *  We'll just remove '>' character, with or without space
             *  [> with space]   =>  [with space]
             *  [>without space] =>  [without space]
             */
            
            line = regex_replace(line, regex("^\\> "), "");
            line = regex_replace(line, regex("^\\>"), "");
        }

        bool hasInlineBlocks() const override {
            return false;
        }

        bool canParseLines() const override {
            return true;
        }
    private:
        bool hasStarted;
        bool hasFinished;
};
