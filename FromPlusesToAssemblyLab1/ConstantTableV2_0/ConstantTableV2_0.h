#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H

#pragma once

#include "../SymbolTableV2_0.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Enum to represent different types of language elements
enum class LanguageElementType {
    Alphabet,
    ReservedWord,
    OperatorSign,
    Delimiter,
    Other
};

struct ConstantTableEntry {
    string symbol;
    LanguageElementType type;
    int lexemeCode = 0;
    int lexemeTypeSize = 0;
    bool operator<(const ConstantTableEntry& other) const {
        return symbol < other.symbol; // For sorted search
    }
};

class ConstantTableV2_0 : public SymbolTable {
public:
    explicit ConstantTableV2_0(const string& filename); // Load from file

    bool add(const string& symbol) override;
    bool add(const string& symbol, LanguageElementType type, int lexemeCode, int lexemeTypeSize);
    [[nodiscard]] bool contains(const string& symbol) const override;
    [[nodiscard]] int getIndex(const string& symbol) const override;
    [[nodiscard]] int getLexemeCode() const override;

    // Sorted Search Algorithm
    [[nodiscard]] int binarySearch(const string& symbol) const;

    [[nodiscard]] vector<ConstantTableEntry> getTable() const { return table; }

private:
    vector<ConstantTableEntry> table;

    // Method to load the table from a file
    bool loadFromFile(const string& filename);
};

#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H
