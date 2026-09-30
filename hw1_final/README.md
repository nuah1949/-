# 고급알고리즘 HW1 - 정렬 알고리즘 비교

2026-2 고급알고리즘 (SIT2001-01)

## 비교 알고리즘

- Insertion Sort: 수업에서 학습한 정렬 알고리즘
- Quick Sort: 강의안의 `partition` 구조를 사용하고 pivot 선택 방법을 비교
  - Random pivot
  - First-element pivot
  - Middle-element pivot
  - Median-of-three pivot
- Heap Sort: 수업에서 다루지 않은 알고리즘으로 선정하고 AI를 통해 원리를 학습

## 프로젝트 구조

```text
src/
├── sort.h
├── random.c
├── insertion_sort.c
├── quick_sort.c
├── quick_sort_first.c
├── quick_sort_middle.c
├── quick_sort_median3.c
├── heap_sort.c
└── main.c
```

## 컴파일

```sh
gcc -std=c17 -Wall -Wextra -O2 src/*.c -o hw1
```

## 실행

```sh
./hw1
```

실행하면 입력 크기 `1,000 / 5,000 / 10,000`과 `random / sorted / reverse / duplicates` 입력에 대해 각 알고리즘의 실행 시간, 비교 횟수, 이동 횟수, 재귀 깊이, 정확성, 안정성을 출력한다. 결과는 `results_growth.csv`에도 저장된다.

난수 pivot은 강의안의 minstd 방식과 같은 재현 가능한 생성기를 사용하며 seed는 `12345`로 고정하였다.
