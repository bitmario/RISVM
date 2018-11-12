import sys
import os
import argparse
sys.path.append("..")

import rc_parse
from rc_visitor import PrintVisitor
from rc_semantics import SemanticAnalyzer
from rc_compiler import ASMCompileVisitor

#from assembler.internals import process_file, write_bytecode

parser = argparse.ArgumentParser(
    description="Compile our Reduced C to ASM"
)
parser.add_argument("input_file", type=str, help="file to compile")
parser.add_argument("-o", "--out", type=str, help="output file")
parser.add_argument(
    "-p", "--print", action="store_true", help="print formatted code and assembly to stdout"
)

args = parser.parse_args()

if args.out:
    output_path = args.out
else:
    output_path = os.path.splitext(args.input_file)[0] + ".asm"

code = ""
with open(args.input_file, "r", encoding="utf-8") as f:
    code = f.read(-1)

program = rc_parse.parse(code)
if program:
    printer = PrintVisitor()
    printer.visit_Program(program)
    if args.print:
        print(printer.result)

    analyzer = SemanticAnalyzer()
    analyzer.visit_Program(program)
    compiler = ASMCompileVisitor()
    compiler.visit_Program(program)
    if args.print:
        print(compiler.result)

    with open(output_path, "w", encoding="utf-8") as f:
        f.write(compiler.result)
else:
    print("Error while compiling")
