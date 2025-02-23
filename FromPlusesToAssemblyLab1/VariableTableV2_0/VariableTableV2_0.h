#ifndef FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H

#pragma once

//#include "Lexema.h"  // Assuming you have a Lexema class
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <limits>
#include <sstream> // For string streams

using namespace std;

enum class LexemeType {
    Undefined,
    Int,
    Float,
    Double,
    String,
    Char,
    Bool
};

struct LexemeAttributes {
    LexemeType type = LexemeType::Undefined;
    bool initialized = false;
    int lexemeCode = 30;
    int lexemeTypeSize = 0;
};

class VariableTableV2_0 {
public:
    explicit VariableTableV2_0(int initialCapacity = 100);  // Constructor

    // New method to load from file
    bool loadFromFile(const string& filename);

    // Add with explicit lexeme code (30 for identifier, 40 for constant)
    bool addLexeme(const string& name, const string& typeStr, int& lexemeCode, int& lexemeTypeSize);

    // Legacy add, automatically determines type and assumes 30
    bool addLexeme(const string& name, LexemeType type = LexemeType::Undefined);
    bool addAttribute(const string& name, LexemeAttributes attributes);
    [[nodiscard]] bool containsLexeme(const string& name) const;

    // Get attribute
    bool getAttribute(const string& name, LexemeAttributes& attributes) const;

    void printTable() const;

    string lexemeTypeToString(LexemeType type);

    static int lexemeSize(const string& typeStr, const string& symbol);

private:
    struct Entry {
        string name;
        LexemeAttributes attributes;
    };

    vector<unique_ptr<Entry>> table;  // Hash table
    size_t capacity;                            // Current capacity
    size_t size;                                // Number of elements
    double loadFactor = 0.75;                    // Load factor for rehashing

    // Hash function (simple, replace with a better one)
    [[nodiscard]] size_t hash(const string& key) const;

    // Rehash function (resize and redistribute elements)
    void rehash();

    // Helper function to determine LexemeType
    [[nodiscard]] LexemeType determineLexemeType(const string& value) const;
};


#endif //FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H
