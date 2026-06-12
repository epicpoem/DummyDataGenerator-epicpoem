# DummyDataGenerator

반도체 시료 생산주문관리 시스템(SampleOrderSystem) 개발 및 테스트용 **Dummy 데이터 생성 Tool**

> PoC(Proof of Concept) 프로젝트 — Mission 1

---

## 프로젝트 개요

SampleOrderSystem이 사용하는 JSON 저장소에 테스트 데이터를 빠르게 세팅하기 위한 CLI 도구입니다.

- 시드(Seed) 기반 재현 가능한 데이터 생성
- `--seed 0` (기본 시드): 스펙 예시 5건과 동일한 고정 데이터 생성
- 커스텀 시드: 매번 동일 시드 → 동일 데이터 (재현성 보장)
- 기존 데이터 **덮어쓰기 / 추가** 선택 가능

---

## 개발 내용

### 구현 모듈

| 모듈 | 파일 | 역할 |
|------|------|------|
| Generator | `src/Generator.h/.cpp` | 시드 기반 시료·주문 Dummy 데이터 생성 |
| JsonWriter | `src/JsonWriter.h/.cpp` | JSON 파일 저장 (OVERWRITE / APPEND) |
| App | `src/App.h/.cpp` | CLI 진입점, 메뉴, `--seed` 인자 파싱 |
| Models | `src/models/Sample.h` `src/models/Order.h` | 시료·주문 데이터 구조체 |

### 데이터 스키마 (DataPersistence 호환)

**시료 (samples.json)**
```json
{
  "id": "S-001",
  "name": "실리콘 웨이퍼-8인치",
  "avgProdTime": 0.5,
  "yield": 0.92,
  "stock": 480
}
```

**주문 (orders.json)**
```json
{
  "id": "ORD-20260612-0001",
  "sampleId": "S-001",
  "customerName": "삼성전자 파운드리",
  "quantity": 300,
  "status": "RESERVED",
  "actualProductionQuantity": 0,
  "productionStartTime": "",
  "totalProductionTime": 0.0
}
```

### 기본 시드(seed=0) 생성 데이터

| ID | 명칭 | 생산시간(min/ea) | 수율 | 재고 |
|----|------|-----------------|------|------|
| S-001 | 실리콘 웨이퍼-8인치 | 0.5 | 0.92 | 480 |
| S-002 | GaN 에피택셀-4인치 | 0.3 | 0.78 | 220 |
| S-003 | SiC 파워기판-6인치 | 0.8 | 0.92 | 30 |
| S-004 | 포토레지스트-PR7 | 0.2 | 0.95 | 910 |
| S-005 | 산화막 웨이퍼-SiO2 | 0.6 | 0.88 | 0 |

### 테스트

| 파일 | TC 수 | 범위 |
|------|-------|------|
| GeneratorTest.cpp | 14 | 데이터 생성 정확성, 시드 재현성, 경계값 |
| JsonWriterTest.cpp | 14 | 파일 생성, OVERWRITE/APPEND, 스키마 검증, 오류 복구 |
| AppTest.cpp | 6 | `--seed` 인자 파싱 정상/오류 케이스 |
| **합계** | **34** | |

---

## 디렉토리 구조

```
DummyDataGenerator/           ← 이 저장소 루트
├── CLAUDE.md
├── README.md
├── Document/
│   ├── PRD.md
│   ├── WORKLOG.md
│   └── FEATURES/
│       ├── FEATURE-01-dummy-data-generation.md
│       └── FEATURE-02-overwrite-or-append.md
└── DummyDataGenerator/       ← Visual Studio 솔루션 폴더
    ├── DummyDataGenerator.slnx
    ├── CMakeLists.txt
    ├── include/
    │   └── nlohmann/json.hpp (v3.11.3)
    ├── packages/
    │   └── gmock.1.11.0/
    ├── src/
    │   ├── main.cpp
    │   ├── App.h / App.cpp
    │   ├── Generator.h / Generator.cpp
    │   ├── JsonWriter.h / JsonWriter.cpp
    │   └── models/
    │       ├── Sample.h
    │       └── Order.h
    ├── test/
    │   ├── GeneratorTest.cpp
    │   ├── JsonWriterTest.cpp
    │   └── AppTest.cpp
    └── DummyDataGenerator/   ← VS 프로젝트
        └── DummyDataGenerator.vcxproj
```

---

## 빌드 방법

### Visual Studio (권장)

1. `DummyDataGenerator/DummyDataGenerator.slnx` 열기
2. 구성: `Debug | x64`
3. `Ctrl+Shift+B` 빌드

출력: `DummyDataGenerator/DummyDataGenerator/x64/Debug/DummyDataGenerator.exe`

### CMake

```bash
cd DummyDataGenerator/DummyDataGenerator
cmake -B build
cmake --build build
```

출력: `build/Debug/DummyDataGenerator.exe`

---

## 사용법

### 기본 실행 (기본 시드 — 스펙 예시 데이터 생성)

```bash
DummyDataGenerator.exe
```

### 커스텀 시드 지정

```bash
DummyDataGenerator.exe --seed 42
```

- 동일 시드 → 항상 동일한 데이터 생성
- 비정수 또는 누락 시 기본 시드(0)로 자동 폴백

---

## 실행 결과 예시

```
=== DummyDataGenerator ===
사용 시드: 0 (기본 시드)

[1] 시료 데이터만 생성
[2] 시료 + 주문 데이터 생성
[0] 종료
선택: 1

저장 방식을 선택하세요:
[1] 덮어쓰기 (기존 데이터 삭제 후 저장)
[2] 추가    (기존 데이터 유지하고 추가)
[0] 취소
선택: 1

[완료]
시료 5건 저장: data/samples.json
```

생성된 `data/samples.json`:

```json
[
  {"avgProdTime": 0.5, "id": "S-001", "name": "실리콘 웨이퍼-8인치", "stock": 480, "yield": 0.92},
  {"avgProdTime": 0.3, "id": "S-002", "name": "GaN 에피택셀-4인치",  "stock": 220, "yield": 0.78},
  {"avgProdTime": 0.8, "id": "S-003", "name": "SiC 파워기판-6인치",  "stock": 30,  "yield": 0.92},
  {"avgProdTime": 0.2, "id": "S-004", "name": "포토레지스트-PR7",    "stock": 910, "yield": 0.95},
  {"avgProdTime": 0.6, "id": "S-005", "name": "산화막 웨이퍼-SiO2", "stock": 0,   "yield": 0.88}
]
```

---

## 테스트 실행

```bash
cd DummyDataGenerator/DummyDataGenerator
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 개발 환경

| 항목 | 내용 |
|------|------|
| 언어 | C++17 |
| IDE | Visual Studio 18 (2022) Community |
| 빌드 | MSBuild / CMake 3.14+ |
| 테스트 | Google Test / Google Mock 1.11.0 |
| JSON | nlohmann/json v3.11.3 |
| OS | Windows 11 |
