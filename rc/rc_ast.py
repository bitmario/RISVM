class Node:
    def __init__(self):
        self.parent = None


class Program(Node):
    def __init__(self, items=None):
        super().__init__()
        self.items = items or []

    def accept(self, visitor):
        visitor.visit_Program(self)


class VarDecl(Node):
    def __init__(self, _type, ident):
        super().__init__()
        self.type = _type
        self.ident = ident

    def accept(self, visitor):
        visitor.visit_VarDecl(self)


# class FuncDecl(Node):
#     def __init__(self, rtype, ident, args):
#         super().__init__()
#         self.rtype = rtype
#         self.ident = ident
#         self.args = args

#     def accept(self, visitor):
#         visitor.visit_FuncDecl(self)


class FuncDef(Node):
    def __init__(self, rtype, ident, args, body):
        super().__init__()
        self.rtype = rtype
        self.ident = ident
        self.args = args
        self.body = body

    def accept(self, visitor):
        visitor.visit_FuncDef(self)


class FuncParam(Node):
    def __init__(self, _type, ident):
        super().__init__()
        self.type = _type
        self.ident = ident

    def accept(self, visitor):
        visitor.visit_FuncParam(self)


class FuncParams(Node):
    def __init__(self, args=None):
        super().__init__()
        self.args = args or []

    def accept(self, visitor):
        visitor.visit_FuncParams(self)


class FuncArgs(Node):
    def __init__(self, args=None):
        super().__init__()
        self.args = args or []

    def accept(self, visitor):
        visitor.visit_FuncArgs(self)


class FuncCall(Node):
    def __init__(self, ident, args):
        super().__init__()
        self.ident = ident
        self.args = args

    def accept(self, visitor):
        visitor.visit_FuncCall(self)


class StatementBlock(Node):
    def __init__(self, statements=None):
        super().__init__()
        self.statements = statements or Statements()

    def accept(self, visitor):
        visitor.visit_StatementBlock(self)


class Statements(Node):
    def __init__(self, statements=None):
        super().__init__()
        self.statements = statements or []

    def accept(self, visitor):
        visitor.visit_Statements(self)


class AssignStatement(Node):
    def __init__(self, ident, value):
        super().__init__()
        self.ident = ident
        self.value = value

    def accept(self, visitor):
        visitor.visit_AssignStatement(self)


class BreakStatement(Node):
    def __init__(self, expr):
        super().__init__()

    def accept(self, visitor):
        visitor.visit_BreakStatement(self)


class ReturnStatement(Node):
    def __init__(self, expr):
        super().__init__()
        self.expr = expr

    def accept(self, visitor):
        visitor.visit_ReturnStatement(self)


class IfStatement(Node):
    def __init__(self, condition, true_block, else_block=None):
        super().__init__()
        self.condition = condition
        self.true_block = true_block
        self.else_block = else_block

    def accept(self, visitor):
        visitor.visit_IfStatement(self)


class WhileStatement(Node):
    def __init__(self, condition, body):
        super().__init__()
        self.condition = condition
        self.body = body

    def accept(self, visitor):
        visitor.visit_WhileStatement(self)


class UnaryOp(Node):
    def __init__(self, op, right):
        super().__init__()
        self.op = op
        self.right = right

    def accept(self, visitor):
        visitor.visit_UnaryOp(self)


class BinaryOp(Node):
    def __init__(self, left, op, right):
        super().__init__()
        self.left = left
        self.op = op
        self.right = right

    def accept(self, visitor):
        visitor.visit_BinaryOp(self)


class ComparisonOp(Node):
    def __init__(self, left, comp, right):
        super().__init__()
        self.left = left
        self.comp = comp
        self.right = right

    def accept(self, visitor):
        visitor.visit_ComparisonOp(self)


class LogicOp(Node):
    def __init__(self, left, op, right):
        super().__init__()
        self.left = left
        self.op = op
        self.right = right

    def accept(self, visitor):
        visitor.visit_LogicOp(self)


class IntConst(Node):
    def __init__(self, value):
        super().__init__()
        self.value = int(value)

    def accept(self, visitor):
        visitor.visit_IntConst(self)


class Identifier(Node):
    def __init__(self, name):
        super().__init__()
        self.name = name

    def accept(self, visitor):
        visitor.visit_Identifier(self)


class IdentifierExp(Node):
    def __init__(self, identifier):
        super().__init__()
        self.identifier = identifier

    def accept(self, visitor):
        visitor.visit_IdentifierExp(self)

    
class ExpGroup(Node):
    def __init__(self, expression):
        super().__init__()
        self.expression = expression

    def accept(self, visitor):
        visitor.visit_ExpGroup(self)


class Type(Node):
    def __init__(self, name):
        super().__init__()
        self.name = name

    def accept(self, visitor):
        visitor.visit_Type(self)


# class For(Node):
#     def __init__(self, assignement, max_, body):
#         self.type = "for"
#         self.assignement = assignement
#         self.max = max_
#         self.body = body

#     def children(self):
#         return (self.assignement, self.max, self.body)

# class FuncCall(Node):
#     def __init__(self, identifier, args):
#         self.type = "funccall"
#         self.identifier = identifier
#         self.args = args

#     def children(self):
#         return tuple([self.type] + self.args)
