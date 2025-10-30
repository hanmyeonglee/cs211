# 코드 분석

## 1. save_char

gets_buf에 입력받은 char 앞 4비트, 뒤 4바이트, ' '를 세 개 element로 저장함
gets_cnt가 1024 이상이면 작동 안 함

## 2. save_term

gets_buf의 마지막에 NULL 할당

## 3. Gets

gets_cnt을 0으로 초기화
EOF 또는 \n을 만나면 종료하고, 그 전까지 반복하며 save_char로 gets_buf에 저장함
그리고 그냥 buf(stack 버퍼)에도 저장함 <- 이게 BOF 대상
이후 save_term을 실햄
gets_buf는 왜 있는지 모르겠는데 아마 답 제출용인가

## 4. getbuf

Gets의 wrapper function, size 56의 char buffer 제공
BOF 대상 함수

## 5. touch 1

첫 번째 BOF 대상
실행만 하면 validate

## 6. touch 2

인자로 val을 넘기고 그 값이 cookie와 같아야함

## 7. touch 3

인자로 cookie string addr를 넘기고 그게 cookie hex와 같아야 함

## 8. flags of ctarget

```txt
-i <infile>  Input file
-l <level>   Attack level
```

## 9. main

segfault, buserror, illegalinstruction error에 대한 핸들러 지정
is_checker는 아마 검사하는 사람인지 검사하는 목적이라 별 상관 없을듯
옵션 검사해서 각 flag에 맞춰 세팅하고
initialize_target 호출
이후 cookie의 값을 출력한 후 stable_launch 실행(bus offset을 인자로 넘김)

## 10. initialize_target

설정한 레벨과 target_id를 통해 cookie 설정하고, cookie로 authkey를 설정해 bss에 global로 처리하고
srandom을 target_id+1로 설정함

random값을 scramble을 통해 섞은 후에 bus_offset에 8 * scrambled_random + 256으로 설정
target_prefix 맨 앞을 c로 설정한 후
notify대상이면서 checker가 아니면 호스트 검사, 서버에서 실행하는지 검사함

## 11. stable_launch

뭔가 메모리 설정 하고 해제하는거 함, stack 조정해서 아예 권한 탈취하는걸 방지하는건가
그리고 launch로 받은 offset주고 실행함

## 12. launch

input 받을 공간 대충 만들고 test함수 실행

## 13. test

getbuf 실행 <- bof 직전 함수

## 14. ROPGadget

```
(base) nemo@Nemo:/mnt/c/Users/ejong/Workspace/POSTECH/cs211/Attack Lab$ ROPgadget --binary ./ctarget | grep "pop rdi"
0x00000000004028f0 : pop rdi ; ret
```