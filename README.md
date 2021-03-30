# LaMark
A C++ header file only library to convert Markdown syntax into LaTeX syntax. It is built with extensibility in mind which means the priority will be more on code clarity than simply on performance as in C++, one can write clear code that can be efficient.

## An important note
Please note that **this library is currently a proof-of-concept.** Just to archive (and possibly show) how I got started with Markdown parsing. In case you are planning to use this library for any kind of application where Markdown are LaTeX are used, note that 
1. This library currently doesn't support
    - Tables
    - Nested Lists (from indentation level 2 onwards)
    - Nested Containers in general as the code parses one container at a time.
    - Special Characters in LaTeX including
        - `%` which means comments in LaTeX 
        - `&` which is an alignment character.
        - `$` which is used for specifying inline math blocks
2. I have plans to drastically change the code base which includes **outright changing the structure of the classes and the project** in future.  **I want to include better support for nested containers which isn't working well.**

## Another markdown parsing library, yay! Any good reason?
There are 2 major reasons for that, let's get to these

I know there are many markdown parsers (and libraries) including those written in C++. Most of them are written to convert Markdown to HTML, because let's face it, **it's hard to convince that HTML has a very writable syntax**. It's very readable but writing can be a chore which is why Markdown was built.
I want a markdown library that can convert Markdown document into its LaTeX counterpart and that too utilizing modern C++ standard and practices (meaning C++11 onwards). So, **I started building this library with the aim to convert my markdown files for LaTeX articles.** 
**I know pandoc exsits but the project too big!** It takes ~100MB of Haskell dependencies which is too much for converting Markdown to LaTeX. It's great but seems like an overkill to me due to the size of dependencies.

The other reason is that **parsing is an interesting challenge.** As a recent CS graduate, it is one of few things that looks simple on surface but the moment you start to implement it, you'll get into problems that one needs to solve conceptually first. Although this proof-of-concept was built in a week, it still was a formidable challenge for me. So, yeah I'll also use it to showcase my ability in C++.

## What does it support?
It currently supports all line elements like **bold**, *italic*, using hyperlinks like to [my website](https://pixeltrik.vercel.app/), etc. 
This supports almost all blocks, however not all which includes

- Paragraphs
- Headings
- Code Blocks
- Blockquotes (without citation)
- Lists (up to indentation level 1)

## Which Operating Systems can it support?
Almost all OS are supported as long it can utilize the most up to date GCC (4.8.1 and above) or Clang (3.3 and above), which means the OS supported are
- Linux
- MacOS
- Windows
- BSD

## And, what dependencies are required to get the most out of it?
This parser is meant to be as minimal as possible without cutting too many corners. For optimal use you need
- C++ compiler that is compliant with at least C++ 11 standard and above like GCC, Clang (if you are using MacOS or OpenBSD) or MinGW for Windows folks. Hardly matters, **I am testing this using Clang on both Arch and Gentoo Linux**.
- Base LaTeX compiler, you can find and install however your OS package manager allows (if you have). In case you don't where to get yours, go to [LaTeX project's get page](https://www.latex-project.org/get/).

## How to use it?
There is a small program I wrote called `tex-injector.cpp` in `example` directory of this project. You can study the code, to see it action, compile it using

```
clang++ -O2 tex-injector.cpp -o tex-injector
```
Or in GCC
```
g++ -O2 tex-injector.cpp -o tex-injector
```

And then run it using this command.
```
cat test.md | ./tex-injector test-template.tex
```

## Oh no! I can't compile my LaTeX file, what's wrong?
Assuming your markdown file uses only the supported elements, then in that case there could be problem either in recognizing formatting characters or the `.tex` file has insufficient packages. When writing this parser, these packages are used.
- For strikethroughs, I used `soul` package.
- For hyper links, I used `hyperref` package.
- For checklists, I used `amssymb` package.

To include these in your project, include `\usepackage{package-name}` after `\documentclass` statement and before `\begin{document}` statement.
Like this
```
\documentclass{article}

\usepackage{amssymb}
\usepackage{soul}
\usepackage{hyperref}

...

\begin{document}
...
```
