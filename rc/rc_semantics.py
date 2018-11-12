from collections import OrderedDict


class Symbol:
    def __init__(self, name, type=None):
        self.name = name
        self.type = type


class FuncSymbol(Symbol):
    def __init__(self, name, rtype="int", arg_types=None):
        self.name = name
        self.rtype = rtype
        self.arg_types = arg_types or []

    def args_size(self):
        sizes = {"int": 4}
        return sum(sizes[t] for t in self.arg_types)


class VarSymbol(Symbol):
    SIZE_MAP = {"int": 4}

    def __init__(self, name, type="int", offset=None):
        super().__init__(name, type)
        self.offset = None

    def type_size(self):
        return self.SIZE_MAP[self.type]


class LocalVarSymbol(VarSymbol):
    pass


class ArgVarSymbol(VarSymbol):
    def __init__(self, name, type="int", offset=None, func_symbol=None):
        if not func_symbol:
            raise ValueError
        super().__init__(name, type)
        self.func_symbol = func_symbol


class SymbolTable(object):
    def __init__(self, scope_name, scope_level, enclosing_scope=None):
        self._symbols = OrderedDict()
        self.scope_name = scope_name
        self.scope_level = scope_level
        self.stack_offset = 0
        self.arg_offset = 0
        self.enclosing_scope = enclosing_scope

    def insert(self, symbol):
        if isinstance(symbol, LocalVarSymbol):
            self.stack_offset += symbol.type_size()
            symbol.offset = self.stack_offset
        elif isinstance(symbol, ArgVarSymbol):
            self.arg_offset += symbol.type_size()
            symbol.offset = self.arg_offset
        self._symbols[symbol.name] = symbol

    def lookup(self, name, current_scope_only=False):
        symbol = self._symbols.get(name)

        if symbol:
            return symbol
        if current_scope_only:
            return None

        if self.enclosing_scope is not None:
            return self.enclosing_scope.lookup(name)


class SemanticAnalyzer:
    def __init__(self):
        self.current_scope = None

    def child_accept(self, parent, child):
        child.parent = parent
        child.accept(self)

    def visit_Program(self, node):
        global_scope = SymbolTable(
            scope_name="global",
            scope_level=1,
            enclosing_scope=self.current_scope,  # None
        )
        self.current_scope = global_scope

        for c in node.items:
            self.child_accept(node, c)

        self.current_scope = self.current_scope.enclosing_scope

    def visit_VarDecl(self, node):
        var_name = node.ident.name
        if self.current_scope.lookup(var_name, current_scope_only=True):
            raise Exception("{} already declared".format(var_name))

        node.symbol = LocalVarSymbol(var_name, node.type.name)
        self.current_scope.insert(node.symbol)

    def visit_FuncDef(self, node):
        func_name = node.ident.name
        if self.current_scope.lookup(func_name, current_scope_only=True):
            raise Exception("{} already declared".format(func_name))

        node.symbol = FuncSymbol(
            func_name, node.rtype.name, [p.type.name for p in node.args.args]
        )
        self.current_scope.insert(node.symbol)

        self.child_accept(node, node.rtype)
        self.child_accept(node, node.ident)

        node.scope = SymbolTable(
            scope_name=node.ident.name,
            scope_level=self.current_scope.scope_level + 1,
            enclosing_scope=self.current_scope,
        )
        self.current_scope = node.scope

        self.child_accept(node, node.args)
        self.child_accept(node, node.body)

        self.current_scope = self.current_scope.enclosing_scope

    def visit_FuncParam(self, node):
        var_name = node.ident.name
        if self.current_scope.lookup(var_name, current_scope_only=True):
            raise Exception("{} already declared".format(var_name))

        node.symbol = ArgVarSymbol(var_name, node.type.name, func_symbol=node.parent.parent.symbol)
        self.current_scope.insert(node.symbol)

    def visit_FuncParams(self, node):
        for i in range(len(node.args)):
            self.child_accept(node, node.args[i])

    def visit_FuncCall(self, node):
        node.symbol = self.current_scope.lookup(node.ident.name)
        if not node.symbol or not isinstance(node.symbol, FuncSymbol):
            raise Exception("Invalid symbol {}".format(node.ident.name))
        if len(node.args.args) != len(node.symbol.arg_types):
            raise Exception("Args number mismatch for {}".format(node.ident.name))

        self.child_accept(node, node.ident)
        self.child_accept(node, node.args)

    def visit_FuncArgs(self, node):
        for i in range(len(node.args)):
            self.child_accept(node, node.args[i])

    def visit_StatementBlock(self, node):
        self.child_accept(node, node.statements)

    def visit_Statements(self, node):
        for s in node.statements:
            self.child_accept(node, s)

    def visit_AssignStatement(self, node):
        node.symbol = self.current_scope.lookup(node.ident.name)
        if not node.symbol:
            raise Exception("Invalid symbol {}".format(node.ident.name))

        self.child_accept(node, node.ident)
        self.child_accept(node, node.value)

    def visit_ReturnStatement(self, node):
        self.child_accept(node, node.expr)

    def visit_BreakStatement(self, node):
        pass

    def visit_IfStatement(self, node):
        self.child_accept(node, node.condition)
        self.child_accept(node, node.true_block)
        if node.else_block:
            self.child_accept(node, node.else_block)

    def visit_WhileStatement(self, node):
        self.child_accept(node, node.condition)
        self.child_accept(node, node.body)

    def visit_UnaryOp(self, node):
        self.child_accept(node, node.right)

    def visit_BinaryOp(self, node):
        self.child_accept(node, node.left)
        self.child_accept(node, node.right)

    def visit_ComparisonOp(self, node):
        self.child_accept(node, node.left)
        self.child_accept(node, node.right)

    def visit_LogicOp(self, node):
        self.child_accept(node, node.left)
        self.child_accept(node, node.right)

    def visit_BoolConst(self, node):
        pass

    def visit_IntConst(self, node):
        pass

    def visit_Identifier(self, node):
        pass

    def visit_IdentifierExp(self, node):
        node.symbol = self.current_scope.lookup(node.identifier.name)
        if not node.symbol:
            raise Exception("Invalid symbol {}".format(node.identifier.name))

        self.child_accept(node, node.identifier)

    def visit_ExpGroup(self, node):
        self.child_accept(node, node.expression)

    def visit_Type(self, node):
        pass
