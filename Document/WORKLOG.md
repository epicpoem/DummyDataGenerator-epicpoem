# WORKLOG - DummyDataGenerator

> **작성 규칙**
> - Agent는 커밋 후 아래 템플릿으로 작업 내용을 기록하고 WORKLOG를 커밋한다.
> - 리뷰 요청 섹션을 작성 후 사용자에게 리뷰를 요청한다.
> - 사용자는 리뷰 섹션에 피드백/요청사항을 작성 후 커밋한다.
> - Agent는 다음 작업 시작 전 반드시 최신 WORKLOG를 확인한다.

---

## 템플릿

```
## [YYYY-MM-DD] {작업 제목}

### 작업 내용
- 

### 커밋
- `{hash}` {커밋 메시지}

### 리뷰 요청
- 

---
### 리뷰 (by User)
- 

### 다음 작업 지시
- 
```

---

<!-- 실제 로그는 아래에 시간순(오래된 항목이 위, 새 항목이 아래)으로 작성 -->

## [2026-06-12] PRD 및 FEATURES 문서 작성

### 작업 내용
- `Document/PRD.md` 작성: 사용자 관점의 기능 요구사항 정의
  - 더미 데이터 생성, 기존 데이터 처리 선택, 저장 결과 확인, SampleOrderSystem 호환성
  - 생성할 시료 5건 명세 포함 (스펙 예시 기반)
  - 주문 데이터 선택 생성 요구사항 포함
- `Document/FEATURES/FEATURE-01-dummy-data-generation.md` 작성
  - 더미 데이터 생성 기능 상세: 사용자 시나리오, 입출력, 데이터 명세, 예외 처리
- `Document/FEATURES/FEATURE-02-overwrite-or-append.md` 작성
  - 기존 데이터 처리 방식 선택 기능 상세: 덮어쓰기/추가/취소 흐름, 예외 처리

### 커밋
- `69c633b` [AI-Docs] Add PRD.md (PRD.md + FEATURES 2건 포함)

### 리뷰 요청
- PRD.md 내용이 사용자 요구사항 수준에 맞게 작성되었는지 확인 부탁드립니다.
- FEATURE-01, FEATURE-02 두 기능 분류가 적절한지 검토 부탁드립니다.
- 주문(Order) 더미 데이터 생성을 선택 사항으로 처리한 것에 동의하시면, 코드 작업을 시작할 수 있습니다.
- 승인 후 다음 작업 지시를 WORKLOG에 작성 또는 구두로 알려주시면 진행하겠습니다.

---
### 리뷰 (by User)
- prd 내용 확인 : 이견 없음
- 주문 더미 테이터 생성 선택사항 처리 동의

### 다음 작업 지시
- FEATURE-01에서, seed 넣어 동일 seed의 경우 동일 데이터 생성 기능 추가 반영

---

## [2026-06-12] FEATURE-01 시드(Seed) 기능 요구사항 추가

### 작업 내용
- `Document/FEATURES/FEATURE-01-dummy-data-generation.md` 업데이트
  - 사용자 시나리오에 시드 입력 단계 추가
  - "시드(Seed) 기능" 섹션 신규 추가: 목적, 요구사항, 시드 기반 가변/고정 항목 명세
  - 입력/출력 항목에 시드 관련 내용 반영
  - 예외 처리에 시드 입력값 오류 케이스 추가

### 커밋
- `0b06fea` [AI-Docs] Add FEATURES documents

### 리뷰 요청
- 시드 기반 가변 항목(재고 수량, 주문 수량, 고객명)과 고정 항목(ID, 명칭, 생산시간, 수율) 분류가 의도에 맞는지 확인 부탁드립니다.
- 시드 미입력 시 기본 고정 데이터(스펙 예시 5건)를 사용하는 정책에 동의하시면 코드 작업을 시작할 수 있습니다.

---
### 리뷰 (by User)
- Feature 기능 업데이트 확인

### 다음 작업 지시
- 시드는 실행시 argument로 받고, 시드 없을시 기본 시드로 동일 데이터 생성 기능 추가

---

## [2026-06-12] FEATURE-01 시드 전달 방식 변경 (argument 방식)

### 작업 내용
- `Document/FEATURES/FEATURE-01-dummy-data-generation.md` 업데이트
  - 시드 전달 방식: 대화형 입력 → CLI argument (`--seed <정수값>`) 로 변경
  - 시드 미전달 시: "기본 고정 데이터" → **기본 시드(고정값)** 로 변경 (항상 동일 데이터 생성)
  - 사용자 시나리오에 실행 예시 (`--seed 42`) 추가
  - 예외 처리: argument 값 비정수 시 기본 시드로 진행 또는 종료로 정책 명시

### 커밋
- `762ece4` [USER-Review] 사용자 리뷰 반영 - 시드 argument 수신 및 기본 시드 정책 지시
- `c5f8fea` [AI-Docs] Update FEATURE-01 - 시드 CLI argument 수신 및 기본 시드 정책 반영

### 리뷰 요청
- `--seed` argument 형식이 의도에 맞는지 확인 부탁드립니다.
- argument 값이 정수가 아닐 때 "기본 시드로 진행" vs "오류 종료" 중 어느 쪽을 원하시는지 결정 부탁드립니다.
- 위 내용 확인 후 승인해주시면 코드 작업을 시작하겠습니다.

---
### 리뷰 (by User)
- FEATURE 01 seed argument 형식 의도에 맞음
- 정수가 아니면 기본 시드로 진행

### 다음 작업 지시
- 코드 작업 시작

---

## [2026-06-12] 코드 구현 - Generator, JsonWriter, App, 테스트

### 작업 내용
- `CMakeLists.txt` 작성 (프로젝트 빌드 구성)
  - nlohmann/json: FetchContent로 v3.11.3 연동
  - Google Test/Mock: 로컬 `packages/gmock.1.11.0` 활용 (인터넷 불필요)
  - DummyDataGeneratorLib: 메인 실행 파일과 테스트가 공유하는 정적 라이브러리
- `src/models/Sample.h`, `src/models/Order.h`: 데이터 모델 정의
- `src/Generator.h`, `src/Generator.cpp`: 시드 기반 더미 데이터 생성
  - DEFAULT_SEED(0): 스펙 예시 5건과 동일한 데이터 생성 (하드코딩)
  - 커스텀 시드: mt19937로 재고/주문수량/고객명 생성
- `src/JsonWriter.h`, `src/JsonWriter.cpp`: JSON 파일 저장 (OVERWRITE/APPEND)
  - APPEND 모드: 중복 시료 ID 건너뜀
  - 대상 디렉토리 없으면 자동 생성
- `src/App.h`, `src/App.cpp`, `src/main.cpp`: CLI 진입점 및 메뉴
  - `--seed <정수>` argument 파싱, 비정수 입력 시 기본 시드로 폴백
  - Windows UTF-8 콘솔 출력 설정
- `test/GeneratorTest.cpp`: Generator 단위 테스트 11건
- `test/JsonWriterTest.cpp`: JsonWriter 단위 테스트 9건

### 커밋
- `63d03fe` [AI-Chore] CMake 프로젝트 초기 구성
- `65878fd` [AI-Feature] 더미 데이터 생성 기능 구현
- `5ee8adf` [AI-Test] Generator, JsonWriter 단위 테스트 추가

### 리뷰 요청
- 빌드 후 `./DummyDataGenerator` (시드 없음) 실행 시 스펙 예시 5건이 `data/samples.json`에 저장되는지 확인 부탁드립니다.
- `./DummyDataGenerator --seed 42` 실행 시 매번 동일한 값이 생성되는지 확인 부탁드립니다.
- 빌드 및 테스트 실행 방법:
  ```
  cmake -B build
  cmake --build build
  ctest --test-dir build
  ```

---
### 리뷰 (by User)
- 구현 내용 확인
- C:\reviewer\PersonnelProject\DummyDataGenerator\DummyDataGenerator 내부의 프로젝트로 진행하여야 하나 별도 프로젝트 생성

### 다음 작업 지시
- C:\reviewer\PersonnelProject\DummyDataGenerator\DummyDataGenerator 내부 프로젝트 사용하여 진행하도록 내용 수정
- 기존 잘못 생성된 프로젝트는 삭제

---

## [2026-06-12] 소스 구조 내부 VS 프로젝트로 재배치

### 작업 내용
- 루트 레벨에 잘못 생성된 `src/`, `test/`, `CMakeLists.txt`를 `DummyDataGenerator/DummyDataGenerator/` 하위로 이전 (git mv)
- `DummyDataGenerator/DummyDataGenerator/include/nlohmann/json.hpp` 추가 (단일 헤더, v3.11.3)
  - CMakeLists.txt의 FetchContent → 로컬 헤더 방식으로 변경 (인터넷 불필요)
- `DummyDataGenerator/DummyDataGenerator/CMakeLists.txt` 위치 조정 및 경로 수정
  - gmock 패키지 경로: `packages/gmock.1.11.0/lib/native` (솔루션 폴더 내 패키지 참조)
- VS 프로젝트 `DummyDataGenerator.vcxproj` 업데이트
  - 기존 빈 `main.cpp` 제거
  - 모든 소스 파일(`../src/*.cpp`) ClCompile 항목에 등록
  - 헤더 파일(`../src/**/*.h`) ClInclude 항목에 등록
  - 모든 구성에 `AdditionalIncludeDirectories`: `../src;../include` 추가
- VS 솔루션 파일(`DummyDataGenerator.slnx`) 신규 추가 (기존 파일 추적)

### 최종 구조
```
DummyDataGenerator/DummyDataGenerator/
├── CMakeLists.txt
├── include/nlohmann/json.hpp
├── src/  (Generator, JsonWriter, App, main, models/)
├── test/ (GeneratorTest, JsonWriterTest)
├── DummyDataGenerator/ (VS 프로젝트)
│   └── DummyDataGenerator.vcxproj
└── packages/gmock.1.11.0/
```

### 빌드 방법
```
# CMake (권장)
cd DummyDataGenerator/DummyDataGenerator
cmake -B build
cmake --build build
ctest --test-dir build

# Visual Studio
DummyDataGenerator/DummyDataGenerator.slnx 열기 후 빌드
```

### 커밋
- `2c95777` [USER-Review] 사용자 리뷰 반영 - 내부 프로젝트 재배치 요청
- `6fb47c9` [AI-Chore] 소스 구조 내부 VS 프로젝트로 이전

### 리뷰 요청
- 재배치된 구조가 의도에 맞는지 확인 부탁드립니다.
- CMake 빌드 또는 VS에서 빌드 후 동작 확인 부탁드립니다.

---
### 리뷰 (by User)
- VS에서 열었을때 main.cpp를 열수 없다고 표시됨

### 다음 작업 지시
- 솔루션 수정 및 시작 지점 main.cpp로 변경

---

## [2026-06-12] VS .vcxproj.filters 수정 - main.cpp 열기 오류 해결

### 작업 내용
- 원인: `.vcxproj.filters`가 소스 재배치 후에도 이전 빈 `main.cpp` (프로젝트 폴더 내) 를 참조하고 있어 VS에서 파일을 찾지 못함
- `DummyDataGenerator.vcxproj.filters` 전면 재작성
  - 기존 `main.cpp` 참조 제거
  - `../src/main.cpp`, `../src/App.cpp`, `../src/Generator.cpp`, `../src/JsonWriter.cpp` → **소스 파일** 필터 등록
  - `../src/App.h`, `../src/Generator.h`, `../src/JsonWriter.h` → **헤더 파일** 필터 등록
  - `../src/models/Sample.h`, `../src/models/Order.h` → **헤더 파일\models** 서브 필터 등록

### 커밋
- `2da5554` [AI-Fix] VS .vcxproj.filters 수정 - 이전 main.cpp 참조 제거 및 src/ 하위 파일 경로 등록

### 리뷰 요청
- VS에서 솔루션 재열기 후 Solution Explorer에서 소스 파일들이 정상 표시되고 열리는지 확인 부탁드립니다.
- 빌드(Ctrl+Shift+B) 후 오류 없이 빌드되는지 확인 부탁드립니다.

---
### 리뷰 (by User)
- 빌드 오류 발생
- 솔루션 오픈시 파일 오픈 관련 에러는 없음

### 다음 작업 지시
- 직접 MSVC이용, 빌드 수행하며 빌드 오류 해결

---

## [2026-06-12] MSVC 빌드 오류 수정 - 빌드 성공

### 작업 내용
수정한 오류 3종:

1. **소스 파일 경로 오류** (`../src/` → `src/`)
   - `src/`와 `test/`가 vcxproj와 같은 폴더에 위치하는데 `../src/`로 잘못 설정되어 "No such file or directory" 발생
   - `DummyDataGenerator.vcxproj`, `.vcxproj.filters` 내 경로 및 `AdditionalIncludeDirectories` 수정

2. **한글 문자열 인코딩 오류** (`Generator.cpp`, `App.cpp`)
   - 시스템 코드 페이지(CP949)와 소스 파일(UTF-8) 불일치로 C2001 발생
   - `vcxproj` 모든 구성에 `<AdditionalOptions>/utf-8</AdditionalOptions>` 추가

3. **`max` 매크로 충돌** (`App.cpp`)
   - `<windows.h>`의 `max` 매크로가 `std::numeric_limits<std::streamsize>::max()` 호출을 방해 (C2589/C2059)
   - `#include <windows.h>` 앞에 `#define NOMINMAX` 추가

### 빌드 결과
```
DummyDataGenerator.vcxproj -> x64\Debug\DummyDataGenerator.exe  ✓
```

### 커밋
- `778c026` [AI-Fix] MSVC 빌드 오류 3종 수정 - 소스 경로(../src→src), 인코딩(/utf-8), NOMINMAX

### 리뷰 요청
- VS에서 빌드(Ctrl+Shift+B) 후 성공 여부 확인 부탁드립니다.
- `x64\Debug\DummyDataGenerator.exe` 실행 후 메뉴가 정상 출력되는지 확인 부탁드립니다.

---
### 리뷰 (by User)
- 정상 빌드 및 콘솔 정상 출력 확인
- 시료, 주문 이외에도 필요 기능에 대해서도 기능 구현 필요
- 자체적 구현이 아니라 ../DataPersistence 내부의 CRUD 기능을 활용하여 모니터링이 가능해야 함

### 다음 작업 지시
- ../DataPersistence 개발 진행 상황 파악
- ../DataPersistence 개발 문서 내용 파악
- ../DataPersistence 개발 문서 내 FEATURE에 기초하여 내용 리팩토링
- ../DataPersistence 에서 구현된 CRUD 기반 필요 Dummy generator 기능 추가

---

## [2026-06-12] DataPersistence 스키마 동기화 - 모델 필드 및 JSON 키 정합성 확보

### 작업 내용
- `../DataPersistence` 프로젝트 구조 및 문서(PRD.md, FEATURES/*.md, 헤더 파일) 전체 파악
- **DataPersistence와 DummyDataGenerator 모델 필드 불일치 수정:**

| 항목 | 수정 전 | 수정 후 |
|------|---------|---------|
| `Sample.productionTime` | `productionTime` | `avgProdTime` |
| `Order.orderId` | `orderId` | `id` |
| `Order.customer` | `customer` | `customerName` |
| Order 누락 필드 | (없음) | `actualProductionQuantity`, `productionStartTime`, `totalProductionTime` 추가 |

- `JsonWriter.cpp`: JSON 직렬화 키 DataPersistence와 동일하게 정렬
- `Generator.cpp`: 주문 생성 시 신규 필드 기본값 추가 (각각 0, "", 0.0)
- `GeneratorTest.cpp`, `JsonWriterTest.cpp`: 변경된 필드명 반영

### DataPersistence 현황 파악 요약
- Repository 패턴 기반 C++17 구현 (SampleRepository, OrderRepository 인터페이스)
- JsonSampleRepository, JsonOrderRepository로 JSON 파일 영속성 구현
- 시료 CRUD 5종 + 주문 CRUD 5종 + 재고 직접 수정 (총 11기능) 완료
- 주문 상태: RESERVED / REJECTED / PRODUCING / CONFIRMED / RELEASE

### 커밋
- `7864ad5` [USER-Review] 사용자 리뷰 반영 - DataPersistence CRUD 기반 Dummy generator 기능 추가 지시
- `04bb782` [AI-Fix] DataPersistence JSON 스키마와 모델 필드 동기화

### 리뷰 요청
- DataPersistence와 JSON 스키마 정합성이 맞는지 확인 부탁드립니다.
- DummyDataGenerator가 생성하는 `data/samples.json`, `data/orders.json`을 DataPersistence에서 그대로 읽을 수 있는지 테스트 부탁드립니다.
- 다음으로 "DataPersistence CRUD 기반 Dummy generator 기능 추가" 작업이 남아 있습니다. 현재는 DummyDataGenerator가 직접 JSON을 작성합니다. DataPersistence의 Repository 클래스를 직접 사용하도록 변경할지, 아니면 JSON 포맷만 맞추는 현재 방식을 유지할지 방향 결정 부탁드립니다.

---
### 리뷰 (by User)
- 스키마 정합성은 문제 없는 것으로 리뷰, 파일 로드 확인
- 포맷만 맞추는 수준으로 POC 진행 예정 (클래스 사용은 이후 통합시 사용)

### 다음 작업 지시
- 현재 수준에서 POC 마무리 이후 본 개발 진행 예정
- 에러처리 가능하도록 네거티브 TC 추가

---

## [2026-06-12] 네거티브 TC 추가 - PoC 테스트 완성

### 작업 내용
- `App::parseSeed` public static으로 변경 → 단위 테스트 가능하게 리팩토링
- `test/AppTest.cpp` 신규 추가 (6개 TC):
  - `--seed` 인자 없음 → DEFAULT_SEED 반환
  - 유효한 정수 seed → 파싱 성공
  - 비정수 seed (`abc`) → DEFAULT_SEED 폴백
  - `--seed` 뒤에 값 없음 → DEFAULT_SEED 폴백
  - 알 수 없는 플래그 무시 → DEFAULT_SEED 반환
  - seed 0 (DEFAULT_SEED 값 자체) → DEFAULT_SEED 반환
- `test/GeneratorTest.cpp` 네거티브 TC 3개 추가:
  - `MaxUintSeedProducesValidSamples`: 최대 seed 값에서도 유효한 데이터 생성
  - `AllOrderQuantitiesInValidRange`: 주문 수량이 50~500 범위 내 보장
  - `NewOrderFieldsHaveDefaultValues`: 신규 Order 필드 기본값 (0, "", 0.0) 검증
- `test/JsonWriterTest.cpp` 네거티브 TC 4개 추가:
  - `WriteEmptySamplesCreatesEmptyArray`: 빈 벡터 → `[]` JSON 생성
  - `WriteEmptyOrdersCreatesEmptyArray`: 빈 벡터 → `[]` JSON 생성
  - `AppendWithCorruptJsonFallsBackToEmptyAndAddsEntries`: 손상된 기존 JSON에 append 시 복구
  - `OverwriteSamplesFieldsMatchDataPersistenceSchema`: JSON 키가 DataPersistence 스키마와 일치 확인
  - `OverwriteOrdersFieldsMatchDataPersistenceSchema`: Order JSON 키 8종 전체 확인
- `CMakeLists.txt`: `test/AppTest.cpp` 테스트 타겟에 추가

### TC 현황
| 파일 | 기존 | 추가 | 합계 |
|------|------|------|------|
| GeneratorTest.cpp | 11 | 3 | 14 |
| JsonWriterTest.cpp | 9 | 5 | 14 |
| AppTest.cpp | 0 | 6 | 6 |
| **합계** | **20** | **14** | **34** |

### 커밋
- `04b790a` [USER-Review] 사용자 리뷰 반영 - 네거티브 TC 추가 지시
- `b5575dd` [AI-Test] 네거티브 TC 추가

### 리뷰 요청
- CMake 빌드 후 테스트 전체 실행 확인 부탁드립니다:
  ```
  cd DummyDataGenerator/DummyDataGenerator
  cmake -B build && cmake --build build
  ctest --test-dir build --output-on-failure
  ```
- PoC 마무리 후 다음 작업 지시 부탁드립니다.

---
### 리뷰 (by User)
- 빌드수행 및 테스트 완료

### 다음 작업 지시
- 리포지토리 최상위에 readme.md 파일 추가, 프로젝트 개요/개발내용/사용법/실행 결과 예시 작성, 이외 필요 내용 작성
