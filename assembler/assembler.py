from data import Opcodes
from internals import *

bytecode = bytearray()
data = []

f = open("test.asm", "r", encoding="utf-8")

line = f.readline()
line_count = 0

while line != "":
    line_count += 1
    line = line.lstrip().rstrip()

    if line.startswith(";"): # comment
        line = f.readline()
        continue
    elif line.startswith(">"): # data
        line = f.readline()
        continue
    else: # regular opcode
        process_instruction(bytecode, line)

    line = f.readline()

f.close()
bytecodeHex = ""

for b in bytecode:
    bytecodeHex += "0x{:02X}, ".format(b)

print(bytecodeHex)
