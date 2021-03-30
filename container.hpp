#pragma once

#include <functional>
#include <sstream>
#include <memory>
#include <cctype>
#include <algorithm>

// To avoid re-including in derivatives
#include <regex>

using namespace std;

class Container {
    public:
        Container(function<void(string&)> lineParserCaller,
                    function<shared_ptr<Container>(const string& line)> getContainerForLineCaller) :
            result("", ios_base::ate | ios_base::in | ios_base::out),
            childParser(nullptr),
            lineParser(lineParserCaller),
            containerFetcher(getContainerForLineCaller)
        {}

        virtual ~Container() {}

        virtual void addLine(string& line) {
            this->parseBlock(line);

            if (this->hasInlineBlocks() and not this->childParser)
                this->childParser = this->getContainerForLine(line);

            if (this->childParser) {
                this->childParser->addLine(line);

                if (this->childParser->isFinished()) {
                    this->result << this->childParser->getResult().str();
                    this->childParser = nullptr;
                }

                return;
            }

            if (this->canParseLines())
                this->parseLines(line);

            this->result << line;
        }

        virtual bool isFinished() const = 0;
        
        stringstream& getResult() { 
            return this->result;
        }
        
        void clear() {
            this->result.str(""); 
        }
    protected:
        stringstream result;
        shared_ptr<Container> childParser;

        virtual bool hasInlineBlocks() const = 0;
        virtual bool canParseLines() const = 0;
        virtual void parseBlock(string& line) = 0;
        
        void parseLines(string& line) {
            if (lineParser) 
                lineParser(line);
        }

        shared_ptr<Container> getContainerForLine(const string& line) {
            if (containerFetcher)
                return containerFetcher(line);
            return nullptr;
        }

        uint32_t getIndentationWidth(const string& line) const {
            bool metNonSpace = false;
            return static_cast<uint32_t>(count_if(
                    line.begin(), line.end(),
                    [&metNonSpace](unsigned char ch) {
                        if (metNonSpace)
                            return false;

                        if (isspace(ch))
                            return true;

                        metNonSpace = true;
                        return false;
                    }));
        }
    private:
        /* Contains the reference to functions for following operations
         *
         * 1. lineParser => to parseLines, will refer to ==Parser::parseLine==
         * 2. containerFetcher => to find the type of container the input string belongs to, refers to ==Parser::getContainerForLine==
         */
        function<void(string&)> lineParser;
        function<shared_ptr<Container>(const string& line)> containerFetcher;
};
