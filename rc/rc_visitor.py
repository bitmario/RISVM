class PrintVisitor:
    def __init__(self):
        self.result = ""
        self.indent_count = 0

    def add(self, s):
        self.result += s

    def addline(self, s):
        self.result += "\n{}{}".format(" " * self.indent_count, s)

    def indent(self):
        self.indent_count += 4

    def dedent(self):
        self.indent_count -= 4

    def child_accept(self, parent, child):
        child.parent = parent
        child.accept(self)

    def visit_Program(self, node):
        for c in node.items:
            self.child_accept(node, c)

    def visit_VarDecl(self, node):
        self.addline("")
        self.child_accept(node, node.type)
        self.add(" ")
        self.child_accept(node, node.ident)
        self.add(";")

    # def visit_FuncDecl(self, node):
    #     self.addline("")
    #     self.child_accept(node, node.rtype)
    #     self.add(" ")
    #     self.child_accept(node, node.ident)
    #     self.add(" ")
    #     self.child_accept(node, node.args)
    #     self.add(";")

    def visit_FuncDef(self, node):
        self.addline("")
        self.child_accept(node, node.rtype)
        self.add(" ")
        self.child_accept(node, node.ident)
        self.child_accept(node, node.args)
        self.child_accept(node, node.body)
        self.addline("")

    def visit_FuncParam(self, node):
        self.child_accept(node, node.type)
        self.add(" ")
        self.child_accept(node, node.ident)

    def visit_FuncParams(self, node):
        self.add("(")
        for i in range(len(node.args)):
            if i > 0:
                self.add(", ")
            self.child_accept(node, node.args[i])
        self.add(")")

    def visit_FuncCall(self, node):
        self.child_accept(node, node.ident)
        self.add("(")
        self.child_accept(node, node.args)
        self.add(")")

    def visit_FuncArgs(self, node):
        for i in range(len(node.args)):
            if i > 0:
                self.add(", ")
            self.child_accept(node, node.args[i])

    def visit_StatementBlock(self, node):
        self.add(" {")
        self.indent()
        self.child_accept(node, node.statements)
        self.dedent()
        self.addline("}")
        self.addline("")

    def visit_Statements(self, node):
        for s in node.statements:
            self.child_accept(node, s)

    def visit_AssignStatement(self, node):
        self.addline("")
        self.child_accept(node, node.ident)
        self.add(" = ")
        self.child_accept(node, node.value)
        self.add(";")

    def visit_BreakStatement(self, node):
        self.addline("break;")

    def visit_ReturnStatement(self, node):
        self.addline("return ")
        self.child_accept(node, node.expr)
        self.add(";")

    def visit_IfStatement(self, node):
        self.addline("if (")
        self.child_accept(node, node.condition)
        self.add(")")
        self.child_accept(node, node.true_block)
        if node.else_block:
            self.addline("else")
            self.child_accept(node, node.else_block)

    def visit_WhileStatement(self, node):
        self.addline("while (")
        self.child_accept(node, node.condition)
        self.add(")")
        self.child_accept(node, node.body)

    def visit_UnaryOp(self, node):
        self.add(node.op)
        self.child_accept(node, node.right)

    def visit_BinaryOp(self, node):
        self.child_accept(node, node.left)
        self.add(" ")
        self.add(node.op)
        self.add(" ")
        self.child_accept(node, node.right)

    def visit_ComparisonOp(self, node):
        self.child_accept(node, node.left)
        self.add(" ")
        self.add(node.comp)
        self.add(" ")
        self.child_accept(node, node.right)

    def visit_LogicOp(self, node):
        self.child_accept(node, node.left)
        self.add(" ")
        self.add(node.op)
        self.add(" ")
        self.child_accept(node, node.right)

    def visit_BoolConst(self, node):
        self.add(str(node.value))

    def visit_IntConst(self, node):
        self.add(str(node.value))

    def visit_Identifier(self, node):
        self.add(str(node.name))

    def visit_IdentifierExp(self, node):
        self.child_accept(node, node.identifier)

    def visit_ExpGroup(self, node):
        self.add("(")
        self.child_accept(node, node.expression)
        self.add(")")

    def visit_Type(self, node):
        self.add(node.name)
