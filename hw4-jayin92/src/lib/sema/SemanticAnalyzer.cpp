#define indent fprintf(stderr, "    ")

#include "sema/SemanticAnalyzer.hpp"
#include "sema/SymbolTable.hpp"
#include "visitor/AstNodeInclude.hpp"

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolTable *global_table = new SymbolTable();
    global_table->addSymbol(
        SymbolEntry(
            p_program.getNameCString(),
            Kind::PROGRAM,
            p_program.getRetTypeSharedPtr(), 
            "",
            false
        )
    );
    symbol_manager.pushScope(global_table);
    p_program.visitChildNodes(*this);
    if(opt_dump)
        global_table->printTable();
    symbol_manager.popScope();
    // delete global_table;
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    auto current_table = symbol_manager.getTopTable();
    if(current_table->hasSymbol(p_variable.getNameCString())){
        //. output error to stderr
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: symbol '%s' is redeclared\n",
                p_variable.getLocation().line,
                p_variable.getLocation().col,
                p_variable.getNameCString()
        );
        indent; listSourceErr(p_variable.getLocation().line);
        indent; cursorErr(p_variable.getLocation().col);
        return;
    }

    if(symbol_manager.hasLoopVar(p_variable.getNameCString())){
        //. output error to stderr
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: symbol '%s' is redeclared\n",
                p_variable.getLocation().line,
                p_variable.getLocation().col,
                p_variable.getNameCString()
        );
        indent; listSourceErr(p_variable.getLocation().line);
        indent; cursorErr(p_variable.getLocation().col);
        return;
    }

    Kind var_kind = this->function_para ? Kind::PARAMETER : Kind::VARIABLE;
    current_table->addSymbol(
        SymbolEntry(
            p_variable.getNameCString(),
            var_kind,
            p_variable.getTypeSharedPtr(),
            "",
            false
        )
    );

    if(p_variable.getTypeSharedPtr()->isValidArray() == false){
        //. output error to stderr
        has_error = true;
        current_table->getLastSymbol()->setHasError(true);
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: '%s' declared as an array with an index that is not greater than 0\n",
                p_variable.getLocation().line,
                p_variable.getLocation().col,
                p_variable.getNameCString()
        );
        indent; listSourceErr(p_variable.getLocation().line);
        indent; cursorErr(p_variable.getLocation().col);
        return;
    }
    set_constant = true;
    p_variable.visitChildNodes(*this);
    set_constant = false;
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if(set_constant) {
        auto current_table = symbol_manager.getTopTable();
        auto last_symbol = current_table->getLastSymbol();
        last_symbol -> setKind(Kind::CONSTANT);
        last_symbol -> setAttribute(
            p_constant_value.getConstantValueCString()
        );
    } 
    
    
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    /*
     * TODO:
     *
     * v 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    auto current_table = symbol_manager.getTopTable();
    if(current_table->hasSymbol(p_function.getNameCString())){
        //. output error to stderr
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: symbol '%s' is redeclared\n",
                p_function.getLocation().line,
                p_function.getLocation().col,
                p_function.getNameCString()
        );
        indent; listSourceErr(p_function.getLocation().line);
        indent; cursorErr(p_function.getLocation().col);
    } else {
        current_table->addSymbol(
            SymbolEntry(
                p_function.getNameCString(),
                Kind::FUNCTION,
                p_function.getRetTypeSharedPtr(),
                p_function.getParametersCString(),
                false
            )
        );
    }

    SymbolTable *func_table = new SymbolTable();
    symbol_manager.pushScope(func_table);
    this->function_para = true;
    // printf("test\n");
    func_type = p_function.getRetTypeCString();
    // printf("%s\n", ret_type.c_str());
    if(func_type == "void") in_procedure = true;
    p_function.visitChildNodes(*this);
    in_procedure = false;
    func_type = "";
    // printf("%s %s\n", p_function.getNameCString(), p_function.getParametersCString());
    if(opt_dump)
        func_table->printTable();
    symbol_manager.popScope();
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    /*
     * TODO:
     *
     * v 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if(this->function_para){
        this->function_para = false;
        p_compound_statement.visitChildNodes(*this);
        
    } else {
        SymbolTable *compound_table = new SymbolTable();
        symbol_manager.pushScope(compound_table);
        p_compound_statement.visitChildNodes(*this);
        if(opt_dump)
            compound_table->printTable();
        symbol_manager.popScope();
    }
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    
    p_print.visitChildNodes(*this);
    if(p_print.getTargetType() == nullptr){
        return;
    }
    std::string type = p_print.getTargetType()->getPTypeCString();
    if(
        type != "integer" &&
        type != "real" &&
        type != "boolean" &&
        type != "string"
    ) {
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: expression of print statement must be scalar type\n",
                p_print.getTarget().getLocation().line,
                p_print.getTarget().getLocation().col
        );
        indent; listSourceErr(p_print.getTarget().getLocation().line);
        indent; cursorErr(p_print.getTarget().getLocation().col);
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table i1f this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_bin_op.visitChildNodes(*this);
    if(p_bin_op.getLeftType() == nullptr || p_bin_op.getRightType() == nullptr){
        return;
    }
    std::string op = p_bin_op.getOpString();
    std::string left_type = p_bin_op.getLeftType()->getPTypeCString();
    std::string right_type = p_bin_op.getRightType()->getPTypeCString();

    if(
        op == "+" ||
        op == "-" ||
        op == "*" ||
        op == "/"
    ) {
        if(
            op == "+" &&
            left_type == "string" &&
            right_type == "string"
        ) {
            p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kStringType)));
            return;
        }

        if(
            (
                left_type != "integer" &&
                left_type != "real"
            ) || (
                right_type != "integer" &&
                right_type != "real"
            )
        ) {
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operands to binary operator '%s' ('%s' and '%s')\n",
                    p_bin_op.getLocation().line,
                    p_bin_op.getLocation().col,
                    op.c_str(),
                    left_type.c_str(),
                    right_type.c_str()
            );
            indent; listSourceErr(p_bin_op.getLocation().line);
            indent; cursorErr(p_bin_op.getLocation().col);
            return;
        } else {
            if(right_type == "integer" && left_type == "integer"){
                p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kIntegerType)));
            } else {
                p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kRealType)));
            }
        }
    } else if(op == "mod"){
        if(left_type != "integer" || right_type != "integer"){
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operands to binary operator '%s' ('%s' and '%s')\n",
                    p_bin_op.getLocation().line,
                    p_bin_op.getLocation().col,
                    op.c_str(),
                    left_type.c_str(),
                    right_type.c_str()
            );
            indent; listSourceErr(p_bin_op.getLocation().line);
            indent; cursorErr(p_bin_op.getLocation().col);
            return;
        } else {
            p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kIntegerType)));
        }
    } else if (
        op == "and" ||
        op == "or"
    ) {
        if(left_type != "boolean" || right_type != "boolean"){
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operands to binary operator '%s' ('%s' and '%s')\n",
                    p_bin_op.getLocation().line,
                    p_bin_op.getLocation().col,
                    op.c_str(),
                    left_type.c_str(),
                    right_type.c_str()
            );
            indent; listSourceErr(p_bin_op.getLocation().line);
            indent; cursorErr(p_bin_op.getLocation().col);
            return;
        } else {
            p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kBoolType)));
        }
    } else if (
        op == ">" ||
        op == ">=" ||
        op == "<" ||
        op == "<=" ||
        op == "=" ||
        op == "<>"
    ) {
        if((left_type == "real" || left_type == "integer") && (right_type == "real" || right_type == "integer")){
            p_bin_op.setType(PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kBoolType)));
        } else {
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operands to binary operator '%s' ('%s' and '%s')\n",
                    p_bin_op.getLocation().line,
                    p_bin_op.getLocation().col,
                    op.c_str(),
                    left_type.c_str(),
                    right_type.c_str()
            );
            indent; listSourceErr(p_bin_op.getLocation().line);
            indent; cursorErr(p_bin_op.getLocation().col);
            return;
        }
    }
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_un_op.visitChildNodes(*this);
    if(p_un_op.getOperandType() == nullptr){
        return;
    }
    std::string op = p_un_op.getOpCString();
    std::string type = p_un_op.getOperandType()->getPTypeCString();
    if(op == "-"){
        if(type != "real" && type != "integer"){
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operand to unary operator '%s' ('%s')\n",
                    p_un_op.getLocation().line,
                    p_un_op.getLocation().col,
                    op.c_str(),
                    type.c_str()
            );
            indent; listSourceErr(p_un_op.getLocation().line);
            indent; cursorErr(p_un_op.getLocation().col);
            return;
        }
        p_un_op.setType(p_un_op.getOperandType());
    } else if(op == "not"){
        if(type != "boolean"){
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: invalid operand to unary operator '%s' ('%s')\n",
                    p_un_op.getLocation().line,
                    p_un_op.getLocation().col,
                    op.c_str(),
                    type.c_str()
            );
            indent; listSourceErr(p_un_op.getLocation().line);
            indent; cursorErr(p_un_op.getLocation().col);
            return;
        }
        p_un_op.setType(p_un_op.getOperandType());
    }


}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    auto symbol = symbol_manager.getSymbol(p_func_invocation.getNameCString());
    p_func_invocation.visitChildNodes(*this);
    if(symbol != nullptr){
        if(symbol->getKind() == Kind::FUNCTION){
            std::string args = symbol->getAttributeCString();
            // turn args into vector
            std::vector<std::string> args_vec;
            std::string arg;
            for(auto it=args.begin(); it!=args.end(); it++){
                if(*it == ','){
                    // remove space 
                    // printf("%s\n", arg.c_str());

                    while(arg.back() == ' '){
                        arg.pop_back();
                    }
                    // remove prefix space
                    while(arg.front() == ' '){
                        arg.erase(arg.begin());
                    }
                    args_vec.push_back(arg);
                    arg = "";
                } else {
                    // if(arg == "" && *it == ' ') continue;
                    arg += *it;
                }
            }
            if(arg != ""){
                while(arg.back() == ' '){
                    arg.pop_back();
                }
                while(arg.front() == ' '){
                    arg.erase(arg.begin());
                }
                args_vec.push_back(arg);
            }
            if(args_vec.size() != p_func_invocation.getArgs().size()){
                // printf("%s %d %d\n", p_func_invocation.getNameCString(), args_vec.size(), p_func_invocation.getArgs().size());
                has_error = true;
                fprintf(stderr, 
                        "<Error> Found in line %d, column %d: too few/much arguments provided for function '%s'\n",
                        p_func_invocation.getLocation().line,
                        p_func_invocation.getLocation().col,
                        p_func_invocation.getNameCString()
                );
                indent; listSourceErr(p_func_invocation.getLocation().line);
                indent; cursorErr(p_func_invocation.getLocation().col);
                return;
            }
            int args_size = args_vec.size();
            // printf("last: %s\n", p_func_invocation.getArgs()[args_size-1]->getTypeSharedPtr()->getPTypeCString());
            for(int i=0;i<args_size;i++){
                // printf("%s %s\n", p_func_invocation.getArgs()[i]-, args_vec[i].c_str());
                std::string func_invocation_type = p_func_invocation.getArgs()[i]->getTypeSharedPtr()->getPTypeCString();
                // printf("%s %s\n", func_invocation_type.c_str(), args_vec[i].c_str());
                if(func_invocation_type != args_vec[i]){
                    has_error = true;
                    fprintf(stderr, 
                            "<Error> Found in line %d, column %d: incompatible type passing '%s' to parameter of type '%s'\n",
                            p_func_invocation.getArgs()[i]->getLocation().line,
                            p_func_invocation.getArgs()[i]->getLocation().col,
                            func_invocation_type.c_str(),
                            args_vec[i].c_str()
                    );
                    indent; listSourceErr(p_func_invocation.getLocation().line);
                    indent; cursorErr(p_func_invocation.getArgs()[i]->getLocation().col);
                    return;
                }
            }
            p_func_invocation.setType(symbol->getType());
        } else {
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: call of non-function symbol '%s'\n",
                    p_func_invocation.getLocation().line,
                    p_func_invocation.getLocation().col,
                    p_func_invocation.getNameCString()
            );
            indent; listSourceErr(p_func_invocation.getLocation().line);
            indent; cursorErr(p_func_invocation.getLocation().col);
        }
    } else {
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: use of undeclared symbol '%s'\n",
                p_func_invocation.getLocation().line,
                p_func_invocation.getLocation().col,
                p_func_invocation.getNameCString()
        );
        indent; listSourceErr(p_func_invocation.getLocation().line);
        indent; cursorErr(p_func_invocation.getLocation().col);
        return;
    }

}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    auto symbol = symbol_manager.getSymbol(p_variable_ref.getNameCString());
    if(symbol == nullptr){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: use of undeclared symbol '%s'\n",
                p_variable_ref.getLocation().line,
                p_variable_ref.getLocation().col,
                p_variable_ref.getNameCString()
        );
        indent; listSourceErr(p_variable_ref.getLocation().line);
        indent; cursorErr(p_variable_ref.getLocation().col);
    } else if(symbol->getKind() == Kind::FUNCTION || symbol->getKind() == Kind::PROGRAM){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: use of non-variable symbol '%s'\n",
                p_variable_ref.getLocation().line,
                p_variable_ref.getLocation().col,
                p_variable_ref.getNameCString()
        );
        indent; listSourceErr(p_variable_ref.getLocation().line);
        indent; cursorErr(p_variable_ref.getLocation().col);
    } else if(symbol->getHasError() == false){
        p_variable_ref.visitChildNodes(*this);
        if(p_variable_ref.getDimension() > symbol->getType()->getDimension()){
            has_error = true;
            fprintf(stderr, 
                    "<Error> Found in line %d, column %d: there is an over array subscript on '%s'\n",
                    p_variable_ref.getLocation().line,
                    p_variable_ref.getLocation().col,
                    p_variable_ref.getNameCString()
            );
            indent; listSourceErr(p_variable_ref.getLocation().line);
            indent; cursorErr(p_variable_ref.getLocation().col);
        } else {
            if(reading){
                if(symbol->getKind() == Kind::CONSTANT || symbol->getKind() == Kind::LOOP_VAR){
                    has_error = true;
                    fprintf(stderr, 
                            "<Error> Found in line %d, column %d: variable reference of read statement cannot be a constant or loop variable\n",
                            p_variable_ref.getLocation().line,
                            p_variable_ref.getLocation().col
                    );
                    indent; listSourceErr(p_variable_ref.getLocation().line);
                    indent; cursorErr(p_variable_ref.getLocation().col);
                    return;
                }                
            }
            for(auto &expr : p_variable_ref.getIndices()){
                if(expr->getTypeSharedPtr() == nullptr) return;
                std::string type = expr->getTypeSharedPtr()->getPTypeCString();
                if(type != "integer"){
                    has_error = true;
                    fprintf(stderr, 
                            "<Error> Found in line %d, column %d: index of array reference must be an integer\n",
                            expr->getLocation().line,
                            expr->getLocation().col
                    );
                    indent; listSourceErr(expr->getLocation().line);
                    indent; cursorErr(expr->getLocation().col);
                    return;
                }
            }
            // set primitive type
            // printf("%s %d\n", p_variable_ref.getNameCString(), p_variable_ref.getDimension());
            PTypeSharedPtr type = std::shared_ptr<PType>(new PType(symbol->getType()->getPrimitiveType()));
            // auto new_dims = std::vector<uint64_t>(symbol->getType()->getDimensions().begin() + p_variable_ref.getDimension(), symbol->getType()->getDimensions().end());
            std::vector<u_int64_t>* new_dims = new std::vector<u_int64_t>();
            for(auto it=symbol->getType()->getDimensions().begin()+p_variable_ref.getDimension(); it!=symbol->getType()->getDimensions().end(); it++){
                new_dims->push_back(*it);
                // printf("%d\n", *it);
            }
            type->setDimensions(*new_dims);
            p_variable_ref.setType(type);
        }
    } 
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_assignment.visitChildNodes(*this);
    if(p_assignment.getLvalueType() == nullptr){
        return;
    }
    std::string lvalue_type = p_assignment.getLvalueType()->getPTypeCString();
    if(
        lvalue_type != "integer" &&
        lvalue_type != "real" &&
        lvalue_type != "boolean" &&
        lvalue_type != "string"
    ) {
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: array assignment is not allowed\n",
                p_assignment.getLvalue().getLocation().line,
                p_assignment.getLvalue().getLocation().col
        );
        indent; listSourceErr(p_assignment.getLvalue().getLocation().line);
        indent; cursorErr(p_assignment.getLvalue().getLocation().col);
        return;
    }
    std::string lval_name = p_assignment.getLvalue().getNameCString();
    auto lval_symbol = symbol_manager.getSymbol(lval_name);
    if(lval_symbol->getKind() == Kind::CONSTANT){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: cannot assign to variable '%s' which is a constant\n",
                p_assignment.getLvalue().getLocation().line,
                p_assignment.getLvalue().getLocation().col,
                lval_name.c_str()
        );
        indent; listSourceErr(p_assignment.getLvalue().getLocation().line);
        indent; cursorErr(p_assignment.getLvalue().getLocation().col);
        return;
    }
    if(lval_symbol->getKind() == Kind::LOOP_VAR){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: the value of loop variable cannot be modified inside the loop body\n",
                p_assignment.getLvalue().getLocation().line,
                p_assignment.getLvalue().getLocation().col
        );
        indent; listSourceErr(p_assignment.getLvalue().getLocation().line);
        indent; cursorErr(p_assignment.getLvalue().getLocation().col);
        return;
    }
    if(p_assignment.getExprType() == nullptr){
        return;
    }
    std::string expr_type = p_assignment.getExprType()->getPTypeCString();
    if(
        expr_type != "integer" &&
        expr_type != "real" &&
        expr_type != "boolean" &&
        expr_type != "string"
    ) {
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: array assignment is not allowed\n",
                p_assignment.getExpr().getLocation().line,
                p_assignment.getExpr().getLocation().col
        );
        indent; listSourceErr(p_assignment.getExpr().getLocation().line);
        indent; cursorErr(p_assignment.getExpr().getLocation().col);
        return;
    }
    if(lvalue_type != expr_type){
        if((lvalue_type == "real" && expr_type == "integer") || (lvalue_type == "integer" && expr_type == "real")){
            return;
        } 
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: assigning to '%s' from incompatible type '%s'\n",
                p_assignment.getLocation().line,
                p_assignment.getLocation().col,
                lvalue_type.c_str(),
                expr_type.c_str()
        );
        indent; listSourceErr(p_assignment.getLocation().line);
        indent; cursorErr(p_assignment.getLocation().col);
        return;
    }


}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    reading = true;
    p_read.visitChildNodes(*this);
    reading = false;
    if(p_read.getTargetType() == nullptr){
        return;
    }
    std::string type = p_read.getTargetType()->getPTypeCString();
    if(
        type != "integer" &&
        type != "real" &&
        type != "boolean" &&
        type != "string"
    ) {
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: variable reference of read statement must be scalar type\n",
                p_read.getTarget().getLocation().line,
                p_read.getTarget().getLocation().col
        );
        indent; listSourceErr(p_read.getTarget().getLocation().line);
        indent; cursorErr(p_read.getTarget().getLocation().col);
        return;
    }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_if.visitChildNodes(*this);
    if(p_if.getConditionType() == nullptr){
        return;
    }
    std::string cond_type = p_if.getConditionType()->getPTypeCString();
    if(cond_type != "boolean"){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: the expression of condition must be boolean type\n",
                p_if.getCondition().getLocation().line,
                p_if.getCondition().getLocation().col
        );
        indent; listSourceErr(p_if.getCondition().getLocation().line);
        indent; cursorErr(p_if.getCondition().getLocation().col);
        return;
    }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);
    if(p_while.getConditionType() == nullptr){
        return;
    }
    std::string cond_type = p_while.getConditionType()->getPTypeCString();
    if(cond_type != "boolean"){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: the expression of condition must be boolean type\n",
                p_while.getCondition().getLocation().line,
                p_while.getCondition().getLocation().col
        );
        indent; listSourceErr(p_while.getCondition().getLocation().line);
        indent; cursorErr(p_while.getCondition().getLocation().col);
        return;
    }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    /*
     * TODO:
     *
     * v 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolTable *for_table = new SymbolTable();
    symbol_manager.pushScope(for_table);
    auto &loop_var = p_for.m_loop_var_decl->getVariables()[0];
    if(symbol_manager.hasLoopVar(loop_var->getNameCString())){
        //. output error to stderr
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: symbol '%s' is redeclared\n",
                loop_var->getLocation().line,
                loop_var->getLocation().col,
                loop_var->getNameCString()
        );
        indent; listSourceErr(loop_var->getLocation().line);
        indent; cursorErr(loop_var->getLocation().col);
    } else {
        for_table->addSymbol(
            SymbolEntry(
                loop_var->getNameCString(),
                Kind::LOOP_VAR,
                loop_var->getTypeSharedPtr(),
                "",
                false
            )
        );
    }
    // TODO: check begin and end condition
    // p_for.m_init_stmt->accept(*this);
    // p_for.m_end_condition->accept(*this);
    ConstantValueNode *begin = dynamic_cast<ConstantValueNode*>(&p_for.m_init_stmt->getExpr());
    ConstantValueNode *end = dynamic_cast<ConstantValueNode*>(&(*p_for.m_end_condition));
    int begin_val = std::stoi(begin->getConstantValueCString());
    int end_val = std::stoi(end->getConstantValueCString());
    if(begin_val > end_val){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: the lower bound and upper bound of iteration count must be in the incremental order\n",
                p_for.getLocation().line,
                p_for.getLocation().col
        );
        indent; listSourceErr(p_for.getLocation().line);
        indent; cursorErr(p_for.getLocation().col);
    }
    p_for.m_body->accept(*this);
    if(opt_dump)
        for_table->printTable();
    symbol_manager.popScope();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if(func_type == ""){
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: program/procedure should not return a value\n",
                p_return.getLocation().line,
                p_return.getLocation().col
        );
        indent; listSourceErr(p_return.getLocation().line);
        indent; cursorErr(p_return.getLocation().col);
        return;
    }
    p_return.visitChildNodes(*this);
    if(p_return.getReturnType() == nullptr){
        return;
    }
    // printf("return type: %s\n", p_return.getReturnType()->getPTypeCString());
    std::string ret_type = p_return.getReturnType()->getPTypeCString();
    // func_type = symbol_manager.getTopTable()->getLastSymbol()->getType()->getPTypeCString();
    if(ret_type != func_type){
        if((ret_type == "real" && func_type == "integer") || (ret_type == "integer" && func_type == "real")){
            return;
        } 
        has_error = true;
        fprintf(stderr, 
                "<Error> Found in line %d, column %d: return '%s' from a function with return type '%s'\n",
                p_return.getRetVal().getLocation().line,
                p_return.getRetVal().getLocation().col,
                ret_type.c_str(),
                func_type.c_str()
        );
        indent; listSourceErr(p_return.getRetVal().getLocation().line);
        indent; cursorErr(p_return.getRetVal().getLocation().col);
        return;
    }


}
