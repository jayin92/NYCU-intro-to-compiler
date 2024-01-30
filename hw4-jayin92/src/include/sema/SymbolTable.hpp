#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "AST/PType.hpp"

#include <memory>
#include <stack>
#include <string>

enum class Kind {
    PROGRAM,
    FUNCTION,
    PARAMETER,
    VARIABLE,
    LOOP_VAR,
    CONSTANT
};

class SymbolEntry {
private:
    std::string name;
    Kind kind;
    PTypeSharedPtr type;
    std::string attribute;
    bool has_error;
public:
    SymbolEntry(
        std::string p_name,
        Kind p_kind,
        PTypeSharedPtr p_type,
        std::string p_attribute,
        bool p_has_error
    ) : name(p_name),
        kind(p_kind),
        type(p_type), 
        attribute(p_attribute),
        has_error(p_has_error) {}
    const char* getNameCString() { return name.c_str(); }
    std::string getName() { return name; }
    const char* getKindCString();
    Kind getKind() { return kind; }
    PTypeSharedPtr getType() { return type; }
    const char* getAttributeCString() { return attribute.c_str(); }
    void setKind(Kind p_kind) { kind = p_kind; }
    void setAttribute(std::string p_attribute) { attribute = p_attribute; }
    void setHasError(bool p_has_error) { has_error = p_has_error; }
    bool getHasError() { return has_error; }
    bool isValidRef() { return kind != Kind::PROGRAM && kind != Kind::FUNCTION; }
};

class SymbolTable {
public:
    SymbolTable() { entries = std::vector<SymbolEntry>(); }
    void setLevel(int new_level) { level = new_level; }
    void addSymbol(SymbolEntry entry) { entries.push_back(entry);}
    bool hasSymbol(std::string name);
    std::shared_ptr<SymbolEntry> getSymbol(std::string name);
    SymbolEntry* getLastSymbol() { return &entries.back(); }
    void printDemarcation(const char chr) {
        for (size_t i = 0; i < 110; ++i) {
            printf("%c", chr);
        }
        puts("");
    }
    void printTable();
private:
    std::vector<SymbolEntry> entries;
    int level;
};

class SymbolManager {
public:
    SymbolManager() { tables = std::vector<SymbolTable *>(); cur_level = 0; }
    void pushScope(SymbolTable *new_scope) {
        new_scope->setLevel(cur_level++);
        tables.push_back(new_scope); 
    }
    void popScope() { tables.pop_back(); cur_level--; }
    SymbolTable* getTopTable() {
        return tables.back(); 
    }
    bool hasLoopVar(std::string name);
    std::shared_ptr<SymbolEntry> getSymbol(std::string name);
    int tableSize() { return tables.size(); }
    // other methods
private:
    std::vector <SymbolTable *> tables;
    int cur_level;
};


#endif  