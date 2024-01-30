#include "sema/SymbolTable.hpp"
#include "visitor/AstNodeVisitor.hpp"

const char* SymbolEntry::getKindCString(){
    switch(kind){
        case Kind::PROGRAM:
            return "program";
        case Kind::FUNCTION:
            return "function";
        case Kind::PARAMETER:
            return "parameter";
        case Kind::VARIABLE:
            return "variable";
        case Kind::LOOP_VAR:
            return "loop_var";
        case Kind::CONSTANT:
            return "constant";
    }
}

bool SymbolTable::hasSymbol(std::string name){
    for(auto &entry : entries){
        if(entry.getName() == name){
            return true;
        }
    }
    
    return false;
}

void SymbolTable::printTable() {
    SymbolTable::printDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                            "Attribute");
    SymbolTable::printDemarcation('-');
    for (auto &entry : entries) {
        printf("%-33s", entry.getNameCString());
        printf("%-11s", entry.getKindCString());
        printf("%d%-10s", this->level, this->level == 0 ? "(global)" : "(local)");
        printf("%-17s", entry.getType()->getPTypeCString());
        printf("%-11s", entry.getAttributeCString());
        puts("");
    }
    SymbolTable::printDemarcation('-');
}

std::shared_ptr<SymbolEntry> SymbolTable::getSymbol(std::string name){
    for(auto &entry : entries){
        if(entry.getName() == name){
            return std::make_shared<SymbolEntry>(entry);
        }
    }
    
    return nullptr;
}

bool SymbolManager::hasLoopVar(std::string name){
    for(auto &table : tables){
        if(table -> hasSymbol(name)){
            if(table -> getSymbol(name)->getKind() == Kind::LOOP_VAR){
                return true;
            }
        }
    }
    
    return false;
}

std::shared_ptr<SymbolEntry> SymbolManager::getSymbol(std::string name){
    for(auto it=tables.rbegin(); it!=tables.rend(); it++){
        if((*it) -> hasSymbol(name)){
            return (*it) -> getSymbol(name);
        }
    }
    return nullptr;
}