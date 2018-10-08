from data import Opcodes, labels
from internals import process_instruction, replace_label_instances

bytecode = bytearray()
data = []

f = open("test.asm", "r", encoding="utf-8")

line = f.readline()
line_count = 0

while line != "":
    line_count += 1
    line = line.lstrip().rstrip()

    if line.startswith(";") or len(line) == 0: # comment or empty
        pass
    elif line.startswith(">"): # data
        pass
    elif line.startswith(".") and line.endswith(":") and len(line) > 2: # labels
        label = line[1:-1]
        if label in labels:
            raise ValueError("Label {} is already defined".format(label))
        labels[label] = len(bytecode)
    else: # regular opcode
        process_instruction(bytecode, line)

    line = f.readline()

f.close()

replace_label_instances(bytecode)

bytecodeHex = ", ".join("0x{:02X}".format(b) for b in bytecode)

print(bytecodeHex)
