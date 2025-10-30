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