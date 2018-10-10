from data import REGISTERS, Opcodes, labels, label_instances

def process_file(path):
    bytecode = bytearray()
    f = open(path, "r", encoding="utf-8")

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

    return bytecode

def print_bytecode(bytecode):
    print(", ".join("0x{:02X}".format(b) for b in bytecode))

def write_bytecode(bytecode, path):
    f = open(path, "wb")
    f.write(bytecode)
    f.close()

def str_to_int(s, bytecode, bytes_ahead=0):
    if len(s) == 3 and s.startswith("'") and s.endswith("'"):
        return ord(s[1])
    elif s.startswith("."):
        label = s[1:]
        location = len(bytecode) + bytes_ahead
        if label in label_instances:
            label_instances[label].append(location)
        else:
            label_instances[label] = [location]
        return 0 # to be replaced later
    elif s.startswith("0x"):
        return int(s, 16)
    else:
        return int(s, 10)

def int_to_bytes(val, nbytes):
    if val < 0:
        return val.to_bytes(nbytes, 'little', signed=True)
    else:
        return val.to_bytes(nbytes, 'little', signed=False)

def register_from_name(s):
    s = s.lower()
    if s not in REGISTERS:
        raise ValueError("Invalid register '{}'".format(s))
    return REGISTERS[s]

def replace_label_instances(bytecode):
    for l, instances in label_instances.items():
        if l not in labels:
            raise ValueError("Invalid label {}".format(l))

        label_val = int_to_bytes(labels[l], 2)
        for instance in instances:
            bytecode[instance] = label_val[0]
            bytecode[instance+1] = label_val[1]

def singleop(bytecode, params, opcode):
    if params:
        raise ValueError("Operation '{}' expects 0 arguments, got {}".format(opcode, len(params)))
    bytecode.append(opcode)

def unop(bytecode, params, opcode):
    if len(params) != 1:
        raise ValueError("Operation '{}' expects 1 argument, got {}".format(opcode, len(params)))
    reg = register_from_name(params[0])
    bytecode.append(opcode)
    bytecode.append(reg)

def unop_c(bytecode, params, opcode, nbytes):
    if len(params) != 1:
        raise ValueError("Operation '{}' expects 1 argument, got {}".format(opcode, len(params)))
    val = str_to_int(params[0], bytecode, 1)
    bytecode.append(opcode)
    bytecode.extend(int_to_bytes(val, nbytes))

def binop(bytecode, params, opcode):
    if len(params) != 2:
        raise ValueError("Operation '{}' expects 2 arguments, got {}".format(opcode, len(params)))
    reg1 = register_from_name(params[0])
    reg2 = register_from_name(params[1])
    bytecode.append(opcode)
    bytecode.append(reg1)
    bytecode.append(reg2)

def binop_rc(bytecode, params, opcode, nbytes):
    if len(params) != 2:
        raise ValueError("Operation '{}' expects 2 arguments, got {}".format(opcode, len(params)))
    reg = register_from_name(params[0])
    val = str_to_int(params[1], bytecode, 2)
    bytecode.append(opcode)
    bytecode.append(reg)
    bytecode.extend(int_to_bytes(val, nbytes))

def binop_cr(bytecode, params, opcode, nbytes):
    if len(params) != 2:
        raise ValueError("Operation '{}' expects 2 arguments, got {}".format(opcode, len(params)))
    val = str_to_int(params[0], bytecode, 1)
    reg = register_from_name(params[1])
    bytecode.append(opcode)
    bytecode.extend(int_to_bytes(val, nbytes))
    bytecode.append(reg)

def binop_cc(bytecode, params, opcode, nbytes1, nbytes2):
    if len(params) != 2:
        raise ValueError("Operation '{}' expects 2 arguments, got {}".format(opcode, len(params)))
    val1 = str_to_int(params[0], bytecode, 1)
    val2 = str_to_int(params[1], bytecode, 1 + nbytes1)
    bytecode.append(opcode)
    bytecode.extend(int_to_bytes(val1, nbytes1))
    bytecode.extend(int_to_bytes(val2, nbytes2))

def ternop_ccc(bytecode, params, opcode, nbytes1, nbytes2, nbytes3):
    if len(params) != 3:
        raise ValueError("Operation '{}' expects 3 arguments, got {}".format(opcode, len(params)))
    val1 = str_to_int(params[0], bytecode, 1)
    val2 = str_to_int(params[1], bytecode, 1 + nbytes1)
    val3 = str_to_int(params[2], bytecode, 1 + nbytes1 + nbytes2)
    bytecode.append(opcode)
    bytecode.extend(int_to_bytes(val1, nbytes1))
    bytecode.extend(int_to_bytes(val2, nbytes2))
    bytecode.extend(int_to_bytes(val3, nbytes3))

def ternop_rrc(bytecode, params, opcode, nbytes):
    if len(params) != 3:
        raise ValueError("Operation '{}' expects 3 arguments, got {}".format(opcode, len(params)))
    reg1 = register_from_name(params[0])
    reg2 = register_from_name(params[1])
    val = str_to_int(params[2], bytecode, 3)
    bytecode.append(opcode)
    bytecode.append(reg1)
    bytecode.append(reg2)
    bytecode.extend(int_to_bytes(val, nbytes))

def process_instruction(bytecode, line):
    opcode, sep, params = line.partition(" ")
    opcode = opcode.lower()

    if sep == " ":
        params = [p.lstrip().rstrip() for p in params.split(",")]
    else:
        params = []

    if opcode == "nop":
        singleop(bytecode, params, Opcodes.NOP)
    elif opcode == "lcons":
        binop_rc(bytecode, params, Opcodes.LCONS, 4)
    elif opcode == "lconsw":
        binop_rc(bytecode, params, Opcodes.LCONSW, 2)
    elif opcode == "lconsb":
        binop_rc(bytecode, params, Opcodes.LCONSB, 1)
    elif opcode == "mov":
        binop(bytecode, params, Opcodes.MOV)
    elif opcode == "push":
        unop(bytecode, params, Opcodes.PUSH)
    elif opcode == "pop":
        unop(bytecode, params, Opcodes.POP)
    elif opcode == "pop2":
        binop(bytecode, params, Opcodes.POP2)
    elif opcode == "dup":
        singleop(bytecode, params, Opcodes.DUP)
    elif opcode == "call":
        unop_c(bytecode, params, Opcodes.CALL, 2)
    elif opcode == "ret":
        singleop(bytecode, params, Opcodes.RET)
    elif opcode == "stor":
        binop_cr(bytecode, params, Opcodes.STOR, 2)
    elif opcode == "storw":
        binop_cr(bytecode, params, Opcodes.STORW, 2)
    elif opcode == "storb":
        binop_cr(bytecode, params, Opcodes.STORB, 2)
    elif opcode == "load":
        binop_rc(bytecode, params, Opcodes.LOAD, 2)
    elif opcode == "loadw":
        binop_rc(bytecode, params, Opcodes.LOADW, 2)
    elif opcode == "loadb":
        binop_rc(bytecode, params, Opcodes.LOADB, 2)
    elif opcode == "memcpy":
        ternop_ccc(bytecode, params, Opcodes.MEMCPY, 2, 2, 2)
    elif opcode == "inc":
        unop(bytecode, params, Opcodes.INC)
    elif opcode == "incf":
        unop(bytecode, params, Opcodes.INCF)
    elif opcode == "dec":
        unop(bytecode, params, Opcodes.DEC)
    elif opcode == "decf":
        unop(bytecode, params, Opcodes.DECF)
    elif opcode == "add":
        binop(bytecode, params, Opcodes.ADD)
    elif opcode == "addf":
        binop(bytecode, params, Opcodes.ADDF)
    elif opcode == "sub":
        binop(bytecode, params, Opcodes.SUB)
    elif opcode == "subf":
        binop(bytecode, params, Opcodes.SUBF)
    elif opcode == "mul":
        binop(bytecode, params, Opcodes.MUL)
    elif opcode == "imul":
        binop(bytecode, params, Opcodes.IMUL)
    elif opcode == "fmul":
        binop(bytecode, params, Opcodes.FMUL)
    elif opcode == "div":
        binop(bytecode, params, Opcodes.DIV)
    elif opcode == "idiv":
        binop(bytecode, params, Opcodes.IDIV)
    elif opcode == "fdiv":
        binop(bytecode, params, Opcodes.FDIV)
    elif opcode == "shl":
        binop(bytecode, params, Opcodes.SHL)
    elif opcode == "shr":
        binop(bytecode, params, Opcodes.SHR)
    elif opcode == "ishr":
        binop(bytecode, params, Opcodes.ISHR)
    elif opcode == "mod":
        binop(bytecode, params, Opcodes.MOD)
    elif opcode == "and":
        binop(bytecode, params, Opcodes.AND)
    elif opcode == "or":
        binop(bytecode, params, Opcodes.OR)
    elif opcode == "xor":
        binop(bytecode, params, Opcodes.XOR)
    elif opcode == "not":
        unop(bytecode, params, Opcodes.NOT)
    elif opcode == "u2i":
        unop(bytecode, params, Opcodes.U2I)
    elif opcode == "i2u":
        unop(bytecode, params, Opcodes.I2U)
    elif opcode == "i2f":
        unop(bytecode, params, Opcodes.I2F)
    elif opcode == "f2i":
        unop(bytecode, params, Opcodes.F2I)
    elif opcode == "jmp":
        unop_c(bytecode, params, Opcodes.JMP, 2)
    elif opcode == "jr":
        unop(bytecode, params, Opcodes.JR)
    elif opcode == "jz":
        binop_rc(bytecode, params, Opcodes.JZ, 2)
    elif opcode == "jnz":
        binop_rc(bytecode, params, Opcodes.JNZ, 2)
    elif opcode == "je":
        ternop_rrc(bytecode, params, Opcodes.JE, 2)
    elif opcode == "jne":
        ternop_rrc(bytecode, params, Opcodes.JNE, 2)
    elif opcode == "ja":
        ternop_rrc(bytecode, params, Opcodes.JA, 2)
    elif opcode == "jg":
        ternop_rrc(bytecode, params, Opcodes.JG, 2)
    elif opcode == "jae":
        ternop_rrc(bytecode, params, Opcodes.JAE, 2)
    elif opcode == "jge":
        ternop_rrc(bytecode, params, Opcodes.JGE, 2)
    elif opcode == "jb":
        ternop_rrc(bytecode, params, Opcodes.JB, 2)
    elif opcode == "jl":
        ternop_rrc(bytecode, params, Opcodes.JL, 2)
    elif opcode == "jbe":
        ternop_rrc(bytecode, params, Opcodes.JBE, 2)
    elif opcode == "jle":
        ternop_rrc(bytecode, params, Opcodes.JLE, 2)
    elif opcode == "print":
        unop(bytecode, params, Opcodes.PRINT)
    elif opcode == "printi":
        unop(bytecode, params, Opcodes.PRINTI)
    elif opcode == "printf":
        unop(bytecode, params, Opcodes.PRINTF)
    elif opcode == "printp":
        unop(bytecode, params, Opcodes.PRINTP)
    elif opcode == "println":
        singleop(bytecode, params, Opcodes.PRINTLN)
    elif opcode == "i2s":
        binop_cr(bytecode, params, Opcodes.I2S, 2)
    elif opcode == "s2i":
        binop_rc(bytecode, params, Opcodes.S2I, 2)
    elif opcode == "a_dr":
        binop_rc(bytecode, params, Opcodes.A_DR, 1)
    elif opcode == "a_ar":
        binop_rc(bytecode, params, Opcodes.A_AR, 1)
    elif opcode == "a_dw":
        binop_cc(bytecode, params, Opcodes.A_DW, 1, 1)
    elif opcode == "a_aw":
        binop_cc(bytecode, params, Opcodes.A_AW, 1, 2)
    elif opcode == "a_dwr":
        binop_cr(bytecode, params, Opcodes.A_DWR, 1)
    elif opcode == "a_awr":
        binop_cr(bytecode, params, Opcodes.A_AWR, 1)
    elif opcode == "a_pm":
        binop_cc(bytecode, params, Opcodes.A_PM, 1, 1)
    elif opcode == "halt":
        singleop(bytecode, params, Opcodes.HALT)
    else:
        raise ValueError("Unknown opcode")

    return bytecode
