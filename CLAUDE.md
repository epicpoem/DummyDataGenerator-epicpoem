# DummyDataGenerator - Dummy 데이터 생성 Tool PoC Agent

## 역할
테스트용 **Dummy 데이터를 생성하여 JSON 저장소에 저장**하는 Tool을 구현하는 PoC.
SampleOrderSystem 개발 및 테스트 시 초기 데이터를 빠르게 세팅하는 용도다.

## 상위 Orchestrator
`../CLAUDE.md` 참조. 스펙 문서: `../Document/spec/spec_reference.txt`

---

## 구현 목표

실행 시 미리 정의된 Dummy 데이터를 생성하여 JSON 파일에 저장한다.
SampleOrderSystem이 사용할 데이터 형식에 맞춰 생성해야 한다.

### 정책 결정 사항
- 저장 대상: JSON 저장소 (Questions.txt Q8 → "저장소에 추가"로 해석)
- 저장 방식: JSON 파일 (전체 프로젝트 공통 정책)

---

## 생성할 Dummy 데이터

### 시료 (Sample) - 최소 5건
스펙 예시 데이터를 기반으로 생성:
```
S-001 | 실리콘 웨이퍼-8인치  | 0.5 min/ea | 수율 0.92 | 재고 480
S-002 | GaN 에피택셀-4인치   | 0.3 min/ea | 수율 0.78 | 재고 220
S-003 | SiC 파워기판-6인치   | 0.8 min/ea | 수율 0.92 | 재고 30
S-004 | 포토레지스트-PR7     | 0.2 min/ea | 수율 0.95 | 재고 910
S-005 | 산화막 웨이퍼-SiO2   | 0.6 min/ea | 수율 0.88 | 재고 0
```

### 주문 (Order) - 선택 사항
RESERVED 상태의 주문 데이터 수 건 생성 가능

---

## 구현 요구사항

- CLI 실행 또는 메뉴 선택으로 Dummy 데이터 생성 트리거
- 기존 데이터 덮어쓰기 여부 선택 가능하도록 (또는 append 방식)
- SampleOrderSystem의 JSON 스키마와 동일한 포맷으로 저장

---

## 제출 기준

- [ ] 실행 시 JSON 파일에 Dummy 데이터 저장
- [ ] SampleOrderSystem이 읽을 수 있는 포맷으로 저장
- [ ] CLAUDE.md 존재
- [ ] 의미있는 커밋 이력

---

## 개발 워크플로우 (WORKLOG 기반)

모든 작업은 아래 사이클을 반드시 따른다.
WORKLOG 파일 위치: `Document/WORKLOG.md`

```
1. 작업 시작 전     → Document/WORKLOG.md 열어 최신 [USER-Review] 내용 확인
2. 작업 및 커밋     → 기능 단위로 [AI-xxx] 헤더로 커밋
3. WORKLOG 업데이트 → 작업 내용 요약, 커밋 해시, 리뷰 요청사항 기록 (커밋 안 함)
4. 사용자 리뷰      → 사용자가 WORKLOG에 피드백/다음 지시 작성
5. 커밋 요청        → 사용자가 커밋 요청 시, Agent가 [USER-Review] 헤더로 커밋 및 푸시
6. 다음 작업        → [USER-Review] 내용 기반으로 작업 후 [AI-xxx] 커밋
7. 3번으로 반복
```

### 커밋 메시지 컨벤션

| 헤더 | 사용 주체 | 사용 시점 |
|------|-----------|-----------|
| `[AI-Feature]` | Agent | 새 기능 구현 |
| `[AI-Fix]` | Agent | 버그 수정 (리뷰 피드백 반영 포함) |
| `[AI-Test]` | Agent | 테스트 코드 작성/수정 |
| `[AI-Refactoring]` | Agent | 리팩토링 (기능 변경 없음) |
| `[AI-Docs]` | Agent | 문서 작성/수정 (PRD, README 등) |
| `[AI-Chore]` | Agent | 빌드 설정, 의존성 추가, 프로젝트 초기 구성 |
| `[USER-Review]` | Agent | 사용자가 WORKLOG 업데이트 후 커밋 요청 시 |

예시:
```
[AI-Feature] 시료 등록 및 조회 기능 구현
[AI-Fix] 중복 ID 허용되던 버그 수정
[AI-Test] 시료 등록 중복 ID 검증 테스트 추가
[AI-Refactoring] SampleController 메서드 분리
[AI-Docs] PRD.md 작성
[AI-Chore] Gradle 프로젝트 초기 구성
[USER-Review] 사용자 리뷰 반영 - 시료 검색 기능 수정 요청
```

### WORKLOG 작성 형식
```markdown
## [YYYY-MM-DD] {작업 제목}

### 작업 내용
- 구현한 내용 요약

### 커밋
- `{hash}` {커밋 메시지}

### 리뷰 요청
- 확인 또는 판단이 필요한 사항

---
### 리뷰 (by User)
- (사용자 작성)

### 다음 작업 지시
- (사용자 작성)
```

---

## 공통 제약
- 모델: **Sonnet / Effort: Medium** 만 허용 (Opus 사용 금지)
- Repository: Public 유지
