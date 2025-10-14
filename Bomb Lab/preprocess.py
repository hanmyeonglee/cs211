import re

target1 = b'\xe8\x1a\xff\xff\xff' # call <send_msg> in explode_bomb
target2 = b'\xe8\x90\xfd\xff\xff' # call <send_msg> in phase_defused
target3 = b'\xe8\x26\x05\x00\x00' # call <initialize_bomb> in main
NOP = b'\x90' * 5 # NOP instruction in x86

binary = open('bomb', 'rb').read()

assert len(re.findall(target1, binary)) == 1
assert len(re.findall(target2, binary)) == 1
assert len(re.findall(target3, binary)) == 1

binary = binary.replace(target1, NOP).replace(target2, NOP).replace(target3, NOP) # NOP

with open('bomb', 'wb') as f:
    f.write(binary)
