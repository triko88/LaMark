#pragma once

#include "container.hpp"

class CodeBlock : public Container {
    public:
        CodeBlock(function<void(string&)> lineParser, function<shared_ptr<Container>(const string &line)> containerFetcher) :
            Container(lineParser, containerFetcher) {

                this->hasStarted = false;
                this->hasFinished = false;
        }

        /* 
         *  In method Parser.parse(), we are adding '\n' at the end of every line. As a result, the line is morphed as
         *  [```]   =>  [```\n]
         *  And therefore, equated in this way.
         */

        static bool isStartingLine(const string &line) {
            return line == "```\n";
        }

        bool isFinished() const override {
            return this->hasFinished;
        }

    protected:
        
        bool hasInlineBlocks() const override {
            return false;
        }

        bool canParseLines() const override {
            return false;
        }

        void parseBlock(string &line) override {
            if (line == "```\n") {
                if (not this->hasStarted) {
                    line = "\\begin{verbatim}\n";
                    this->hasStarted = true;
                    this->hasFinished = false;
                    return;
                }

                if (not this->hasFinished) {
                    line = "\\end{verbatim}";
                    this->hasStarted = false;
                    this->hasFinished = true;
                    return;
                }
            }
        }

    private:
        bool hasStarted;
        bool hasFinished;
};
