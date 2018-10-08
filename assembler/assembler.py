import os
import argparse
from internals import process_file, print_bytecode, write_bytecode

parser = argparse.ArgumentParser(description='Assemble the specified file into bytecide')
parser.add_argument('input_file', type=str, help='file to assemble')
parser.add_argument('-o', '--out', type=str, help="output file")
parser.add_argument('-p', '--print', action='store_true', help="print bytecode to stdout")

args = parser.parse_args()

if args.out:
    output_path = args.out
else:
    output_path = os.path.splitext(args.input_file)[0] + '.bin'

bytecode = process_file(args.input_file)
write_bytecode(bytecode, output_path)

if args.print:
    print_bytecode(bytecode)
