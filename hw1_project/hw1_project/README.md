# HW1 - Sorting Algorithm Comparison

2026-2 고급알고리즘 HW1

## Algorithms

본 과제에서는 다음 세 가지 정렬 알고리즘을 비교한다.

- Insertion Sort
- Quick Sort
- Heap Sort

Quick Sort는 pivot 선택 방법에 따른 성능 차이도 추가로 비교한다.

- Random Pivot
- First-element Pivot
- Middle-element Pivot
- Median-of-three Pivot

Heap Sort는 수업에서 다루지 않은 알고리즘으로 선정하여 AI와 참고 자료를 통해 원리를 학습하고 구현하였다.

## Build

```bash
gcc -std=c17 -Wall -Wextra -O2 \
src/main.c \
src/insertion_sort.c \
src/quick_sort.c \
src/quick_sort_first.c \
src/quick_sort_middle.c \
src/quick_sort_median3.c \
src/heap_sort.c \
src/random.c \
-o hw1
```

## Run

```bash
./hw1
```

프로그램은 각 정렬 알고리즘의 정확성을 확인한 후, 다음 항목을 측정한다.

- CPU 실행 시간
- 비교 횟수
- 이동 횟수
- Quick Sort 재귀 깊이
- 정렬 정확성
- 안정성

입력 데이터는 다음 네 가지 유형을 사용한다.

- Random
- Sorted
- Reverse
- Duplicates

또한 입력 크기를 `1,000`, `5,000`, `10,000`으로 증가시키면서 알고리즘의 성장률을 비교한다.

## Quick Sort Pivot Experiment

Quick Sort의 partition 구조는 동일하게 유지하고 pivot 선택 방법만 변경하여 성능을 비교한다.

| Pivot | Description |
|---|---|
| Random | 각 partition에서 난수로 pivot 위치 선택 |
| First | 현재 구간의 첫 번째 원소를 pivot으로 선택 |
| Middle | 현재 구간의 가운데 원소를 pivot으로 선택 |
| Median-of-three | 첫 번째, 가운데, 마지막 원소 중 중앙값을 pivot으로 선택 |

Random Pivot은 수업에서 학습한 난수 생성 방식을 이용한다.

## Measurement

실험에서는 모든 알고리즘에 동일한 측정 기준을 적용한다.

- 정렬 함수의 실행 시간만 측정
- 입력 배열을 복사하는 시간은 측정에서 제외
- `key` 비교 1회당 1 comparison
- `Record` 대입 1회당 1 move
- 동일한 `key`를 가진 `Record`의 `id` 순서를 이용하여 안정성 검사
- Quick Sort의 최대 재귀 깊이 측정
- 각 조건을 3회 반복하여 실행 시간의 평균을 사용

## Folder Structure

```text
hw1/
├── src/
│   ├── sort.h
│   ├── random.c
│   ├── insertion_sort.c
│   ├── quick_sort.c
│   ├── quick_sort_first.c
│   ├── quick_sort_middle.c
│   ├── quick_sort_median3.c
│   ├── heap_sort.c
│   └── main.c
│
├── report/
│   └── REPORT.md
│
├── images/
│
└── README.md
```

## References

- Course lecture materials - Topic 04: Random and Quick Sort
- Wikipedia - Sorting Algorithm
- Wikipedia - Heapsort

## Report

자세한 알고리즘 설명과 실험 결과는 다음 보고서에서 확인할 수 있다.

`report/REPORT.md`