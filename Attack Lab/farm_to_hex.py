from pwn import ELF

elf = ELF("./rtarget")

startFarm_VA = elf.symbols['start_farm']
endFarm_VA = elf.symbols['end_farm']

startFarm_offset = elf.vaddr_to_offset(startFarm_VA)
endFarm_offset = elf.vaddr_to_offset(endFarm_VA) + 6 # include the 'end_farm' instruction

with open("./rtarget", "rb") as f:
    f.seek(startFarm_offset)
    farm_bytes = f.read(endFarm_offset - startFarm_offset)

with open("farm.hex", "w") as f:
    f.write(farm_bytes.hex().upper())