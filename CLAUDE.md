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

## 공통 제약
- 모델: **Sonnet / Effort: Medium** 만 허용 (Opus 사용 금지)
- Repository: Public 유지
