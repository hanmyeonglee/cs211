from pwn import ELF

binary_name = 'ctarget'
with open(binary_name, 'rb') as f:
    binary = f.read()

elf = ELF(binary_name)

global_variable_name = 'notify'

assert global_variable_name in elf.symbols, f"{global_variable_name} not found in binary symbols."

VA = elf.symbols[global_variable_name]
OFFSET = elf.vaddr_to_offset(VA)

assert binary[OFFSET] == 1, f"Expected byte at offset {OFFSET} to be 0x01 but found {binary[OFFSET]}."

modified_binary = binary[:OFFSET] + b'\x00' + binary[OFFSET + 1:]

with open(binary_name + '.back', 'wb') as backup_file:
    backup_file.write(binary)

with open(binary_name, 'wb') as modified_file:
    modified_file.write(modified_binary)