import re
import tqdm

from pwn import disasm, asm
from Crypto.Util.number import long_to_bytes

with open('farm.c', 'r') as f:
    code = f.read()

constants = re.findall(r' \d+U;', code)
hex_constants = list(map(lambda x: long_to_bytes(int(x.strip(' ;U'))).hex(), constants))

ret = asm('ret')
for const, hex_const in tqdm.tqdm(list(zip(constants, hex_constants))):
    fixed_const = ' 0x' + hex_const + 'U; // ' + bytes.fromhex(hex_const)[::-1].hex()

    if ret.hex() in hex_const:
        bytes_const = bytes.fromhex(hex_const).partition(ret)[1:]
        full_bytes = b''.join(bytes_const)[::-1]
        search_space = [full_bytes[i:] for i in range(len(full_bytes) - 1)]

        for seq in search_space:
            asm_code = disasm(seq, arch='amd64')

            if 'ret' not in asm_code:
                continue

            fixed_const += '\n\n    // ' + seq.hex()
            fixed_const += '\n    // ' + asm_code.replace('\n', '\n    // ') + '\n'

    code = code.replace(const, fixed_const)

with open('farm_fixed.c', 'w') as f:
    f.write(code)