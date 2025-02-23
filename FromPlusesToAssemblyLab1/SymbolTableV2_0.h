#ifndef FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H

#pragma once

#include <string>

using namespace std;

// Abstract base class for symbol tables
class SymbolTable {
public:
    virtual ~SymbolTable() = default; // Virtual destructor

    // Pure virtual functions (must be implemented by derived classes)
    virtual bool add(const string& symbol) = 0;
    virtual bool contains(const string& symbol) const = 0;
    virtual int getIndex(const string& symbol) const = 0;
    virtual int getLexemeCode() const = 0; // Return lexeme code
};

#endif //FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H
