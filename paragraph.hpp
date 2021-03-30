#pragma once

#include "container.hpp"

using namespace std;

class Paragraph : public Container {
    public:
        Paragraph (function<void(string&)> lineParser, function<shared_ptr<Container>(const string& line)> containerFetcher) :
            Container(lineParser, containerFetcher) {

                this->hasStarted = false;
                this->hasFinished = false;
        }

        static bool isStartingLine(const string& line) {
            return not line.empty();
        }

        bool isFinished() const override {
            return this->hasFinished;
        }

    protected:
        bool hasInlineBlocks() const override {
            return false;
        }

        bool canParseLines() const override {
            return true;
        }

        /*  
         *  For parsing paragraphs, nothing fancy is going on. 
         *  The lines will be parsed and the result will be collected.
         *  The only role of this function is to detect paragraphs.
         */

        void parseBlock(string& line) override {
            if (not this->hasStarted) {
                this->hasStarted = true;
                return;
            }

            if (line.empty()) {
                line = "\n";
                this->hasFinished = true;
                return;
            }
        }

        
    private:
        bool hasStarted;
        bool hasFinished;
};
