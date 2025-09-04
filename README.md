# Bomb Lab

## Bomb Lab이란?

Bomb Lab은 주어진 바이너리 파일(폭탄)을 분석하여, 단계별로 숨겨진 비밀번호(phase)를 찾아 입력함으로써 폭탄이 터지지 않도록 해제하는 역공학(Reverse Engineering) 실습 과제입니다.

## 내가 해야 할 일

1. **bomb 바이너리 파일을 실행**하고, 각 phase에서 요구하는 입력값을 찾아야 합니다.
2. **GDB, objdump, strings** 등 디버깅/분석 도구를 사용해 어셈블리 코드를 분석합니다.
3. 각 phase를 성공적으로 해제할 때마다, **해제 방법과 과정을 README 또는 별도 문서에 기록**합니다.
4. 모든 phase를 해제한 후, **결과 보고서**를 제출합니다.

## 실습 환경
- 운영체제: Linux (권장)
- 필수 도구: GDB, objdump, strings 등

## 참고 자료
- [GDB 사용법](https://www.gnu.org/software/gdb/documentation/)
- [Reverse Engineering for Beginners](https://beginners.re/)
- 수업 강의자료 및 실습 가이드

## 주의사항
- 타인의 입력값이나 해설을 복사/공유하지 마세요.
- 모든 해제 과정은 반드시 **직접** 수행해야 합니다.
- 질문이 있을 경우, 조교 또는 교수님께 문의하세요.