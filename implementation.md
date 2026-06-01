# Implementation Status — KENTECH Dungeon Explorer

데이터 구조 PBL 과제(`project_code/docs/Assignment_Handout.docx`, `Rubric_and_Checkpoints.docx`)
대비 현재 코드(`include/`, `src/`)의 분석 기록.

- 분석 일자: 2026-05-31
- 빌드: `make` 성공 (경고 없음, `-std=c++17 -Wall -Wextra -pedantic`)
- 실행: 정상 (help / look / move / take / inventory / undo / map / scores / sortscores 확인)

---

## 1. 코드 개요

KENTECH 캠퍼스를 배경으로 한 텍스트 던전 탐험 게임. 제안서(`Student_Project_Proposal_Team3.pdf`)의
세계관(교수님=몬스터, 강의실=방)을 코드로 옮긴 상태. starter code 철학대로
자료구조(`include/ds/`, `src/`)와 게임 로직(`Game.cpp`)을 분리했다.

### 엔티티 / 게임 로직
- `Game.cpp` — 월드 구성(`buildKentechWorld`), 명령 루프(`processCommand`),
  미션 4종(`DataStructureQuiz`, `BinaryGame`, `EnglishDefinition`, `FinalRC`).
- `Player / Room / Item / Monster / GameEvent` — 게임 엔티티.
  `Room`은 `DynamicArray`를, `Player`는 `Inventory`를 멤버로 가져 자료구조를 실제로 사용.
- `MissionType` enum으로 방마다 미니게임을 선언적으로 연결.

---

## 2. 필수 자료구조 7종 (핵심 채점 영역)

| 파일 | 자료구조 | 게임에서의 역할 | 상태 |
|---|---|---|---|
| `DynamicArray.h` | 동적 배열 (2배 증가, Rule of Three) | Room의 아이템·몬스터 보관 | 완성 |
| `Inventory.h/.cpp` | 단일 연결 리스트 | 플레이어 인벤토리 (head 삽입 O(1), 검색·삭제 O(n)) | 완성 |
| `Stack.h` | 스택 (연결노드, 템플릿) | 이동 기록 → `undo` | 완성 |
| `Queue.h` | 큐 (front/rear 연결노드, 템플릿) | `event` 순차 처리 | 완성 |
| `ScoreTree.h/.cpp` | 이진 탐색 트리 | 점수 랭킹, 역중위순회 내림차순 출력 | 완성 |
| `DungeonGraph.h/.cpp` | 그래프 (무방향 인접 리스트) | 방 연결 구조 + 이동/맵 출력 | 완성 |
| `Sorting.h/.cpp` | 선택 정렬 (수동 구현) | 아이템·점수 내림차순 정렬 | 완성 |

### 잘 된 점
- **메모리 관리**: 모든 자료구조에 소멸자 존재. 복사 방지가 필요한 곳
  (`Inventory`, `Stack`, `Queue`, `ScoreTree`, `DungeonGraph`)은 복사생성자/대입 `= delete`,
  복사가 필요한 `DynamicArray`는 Rule of Three 준수.
- **STL 금지 규칙 준수**: `vector/list/stack/queue/map` 미사용. 허용된 `string`,
  `iostream/sstream`, `cctype`만 사용.
- 각 헤더 상단에 "왜 이 자료구조인가" 주석 → 루브릭의 "각 자료구조가 의미 있는 기능에 연결" 항목에 직접 기여.

---

## 3. 마일스톤 / 루브릭 대비 진행도

### Milestone 2 (선형 구조) — 완료
연결 리스트 추가/삭제/빈 리스트, 스택 push/pop/빈 스택, 큐 enqueue/dequeue 모두 구현 + 게임 연결.

### Milestone 3 (게임플레이 통합) — 대부분 완료
빌드 성공, help/look/이동/잘못된 이동 처리/아이템 획득/인벤토리/undo/이벤트 동작 확인.

### Milestone 4 (고급 구조) — 코드상 완료
BST 삽입·내림차순 출력, 그래프 연결·맵 출력, 수동 정렬 모두 게임에서 노출됨.

### 채점 항목별 예상 위치 (100점)
| 루브릭 항목 | 상태 |
|---|---|
| 자료구조 정확성 (30) | 강함 — 7종 완성 + 메모리 안전 |
| 자료구조 통합 (25) | 강함 — 전부 실제 기능에 연결 |
| 완성도/견고성 (20) | 양호 — 플레이 가능, 잘못된 입력 처리 |
| 코드 품질/모듈화 (15) | 강함 — 헤더/소스 분리, 주석 양호 |
| 창의적 확장/발표 (10) | 양호 — KENTECH 테마 |

---

## 4. 남은 작업 / 점검 항목

1. **최종 보고서 (미작성)** — 자료구조 설명, 복잡도 분석, 테스트 전략, 기여도. 별도 비중 큼.
2. **스모크 테스트** — starter에는 `tests/ds_smoke_tests.cpp`와 `make test`가 있으나,
   현재 작업 폴더 Makefile에는 `test` 타깃이 없음. 루브릭 "smoke tests attempted" 대응 필요.
3. **`showSortedScores()` 하드코딩** (`Game.cpp:267-270`) — BST에서 점수를 꺼내오지 못해
   `Ada/Grace/Linus`를 손으로 다시 적어둠. `seedScores`를 바꾸면 정렬 결과와 불일치.
   BST 순회 API를 추가해 실제 트리에서 뽑아오면 개선됨 (코드 주석에 본인도 동일 지적).
4. **제안서 빈칸** — PDF의 팀명/게임 제목/위험 목록/역할 분담 표가 비어 있음 (Milestone 1 체크리스트).
5. **아이템 불일치** — 제안서에는 Map/Note/Stun gun/시간표 등이 있으나 코드엔 4종만 배치
   (Vocabulary, Map, Coffee, Financier). 채점 영향은 작으나 보고서 매핑 표와 일치시키면 좋음.

---

## 요약
자료구조 7종과 게임 통합(Milestone 2·3·4)은 사실상 코드 완료. 남은 핵심은
**최종 보고서 작성, 테스트 타깃 정리, `sortscores` 하드코딩 개선**.

---

# 작업 로그 (Change Log)

이후 수행하는 모든 작업을 시간순으로 여기에 누적 기록한다.

## 2026-05-31 — 몬스터 인카운터 문제 풀(pool) 추가

**요청**: 몬스터를 만났을 때 푸는 문제를 종류별로 더 추가 (비슷한 문제 다양화).

**변경 파일**: `src/Game.cpp` 만 수정 (헤더·다른 파일 변경 없음, 추가 파일 없음).

**변경 내용**
1. `#include <cstdlib>`, `#include <ctime>` 추가 (핸드아웃 허용 헤더).
2. `Game` 생성자에 `std::srand((unsigned)std::time(nullptr));` 추가 — 매 실행마다 출제 문제가 달라지도록 시드.
3. 공통 헬퍼 추가:
   - `struct ChoiceQuestion { prompt, options[3], answer }` — 3지선다 문제 표현.
   - `static int pickIndex(int n)` — `[0, n)` 랜덤 인덱스.
   - `static bool askChoice(pool, n)` — 풀에서 무작위 1문제 출제 후 정오 판정.
4. 미션 핸들러를 단일 문제 → **문제 풀(랜덤 출제)** 로 교체:
   - `runDataStructureQuiz` : 1문제 → **5문제** (LIFO=Stack, FIFO=Queue, BST 중위순회, 연결리스트 head 삽입 O(1), 그래프 적합성).
   - `runEnglishDefinition` : 1문제 → **5문제** (algorithm, variable, recursion, compile, iterate).
   - `runBinaryGame` : 1문제 → **6문제** (10·7·12·5·15·9의 10진→2진 변환, `BinPair` 풀).
   - `runFinalRC` : **변경 안 함**. 사유 — `libraryHint()`가 "campus radio call sign"으로 정답 "RC"를 안내하므로, 랜덤화하면 힌트와 어긋남. 스토리상 고정 키워드 유지.

**검증**
- `make clean && make` 성공 (경고 없음).
- A205 인카운터: 랜덤 문제(FIFO) 출제 → 정답 "2" → 클리어(+30) 확인.
- C404 인카운터: 오답 입력 시 "You lost the challenge" 정상.
- IoT 바이너리 게임: 풀에서 출제 확인.

**알려진 한계**
- `srand(time())`는 초 단위 시드라, **같은 초에 시작된 별도 프로세스**는 동일 문제를 출제함(테스트 시 관찰됨). 단일 게임 세션 내에서는 `rand()`가 호출마다 진행되어 재도전·다른 인카운터에서 다른 문제가 나오므로 실제 플레이에는 영향 없음.

## 2026-05-31 — C404 문제를 일반 영어 단어 문제로 교체 + Vocabulary List 미리보기

**요청**: C404(George) 문제를 CS 용어 정의가 아니라 **단순 영어 단어 문제**로 바꾸고, 그 단어들을 **Vocabulary List 아이템으로 미리 볼 수 있게** 할 것.

**변경 파일**: `include/Game.h`, `src/Game.cpp`.

**변경 내용**
1. `runEnglishDefinition`의 CS 용어 풀(algorithm/variable/...)을 제거하고, 일반 영어 단어 풀로 교체.
2. 단어 풀을 파일 스코프 `static const VocabQuestion VOCAB_POOL[]`로 분리 (퀴즈와 미리보기가 **같은 데이터를 공유**하도록).
   - 단어 6개: abandon, ancient, fragile, generous, rapid, brave (각 3지선다, 정답 위치 분산).
   - `struct VocabQuestion { word, options[3], answer }`, `askVocab()` 헬퍼 추가.
   - `runEnglishDefinition()`은 `return askVocab();`로 단순화.
3. **Vocabulary List 미리보기** 기능:
   - `Game::studyVocabulary()` 추가 (`Game.h`에 선언). 플레이어 인벤토리에 `VocabularyList`가 있으면 단어+정답 뜻을 모두 출력, 없으면 Library에서 획득하라고 안내.
   - 새 명령 `vocab`을 `processCommand`에 연결, `printHelp`에 한 줄 추가.
   - 아이템은 이미 Library에 `VocabularyList`로 존재 → 별도 배치 변경 불필요.

**설계 결정**
- 단어 풀을 파일 스코프 static으로 둔 이유: 퀴즈 출제와 미리보기가 **단일 출처(single source of truth)**를 공유해야 둘이 어긋나지 않음. 제안서의 "Use it to select the correct meaning against George" 의도와 일치.
- DS 퀴즈용 `ChoiceQuestion`/`askChoice`는 그대로 두고 영어용은 별도 구조로 분리 → 기존 기능 회귀 위험 없음.

**검증**
- `make clean && make` 성공 (경고 없음).
- 아이템 없이 `vocab`: "find it in the Library" 안내 정상.
- Library에서 `take VocabularyList` 후 `vocab`: 단어 6개+뜻 목록 출력 정상.
- C404 `event`: "What does the English word 'abandon' mean?" 식 단어 문제 출제, 오답 시 "lost" 정상.

## 2026-05-31 — map 아이템 게이팅 + map 명령 시 이미지 표시

**요청**: (1) `map`을 Library에서 Map 아이템을 줍기 전엔 못 쓰게 막기. (2) `map` 입력 시 이미지가 뜨도록.

**사용자 결정 (AskUserQuestion)**: "이미지"는 **실제 이미지 파일을 OS 기본 뷰어로 열기** 방식으로 구현.

**변경 파일**: `include/Game.h`, `src/Game.cpp`, 신규 `assets/map.png`.

**변경 내용**
1. **이미지 생성**: PIL로 KENTECH 던전 지도 `assets/map.png` (1060x540) 생성.
   방 7개를 박스로, 간선 7개를 선으로 그림. KENTECH Hall=노랑(시작), Bus Station=빨강(최종 보스).
   사용자는 이 파일을 본인 이미지로 교체 가능.
2. **게이팅**: `map` 명령 → 새 메서드 `Game::showMap()` 호출.
   인벤토리에 `Map` 아이템이 없으면 "Find it in the Library and 'take Map'." 안내 후 종료.
   (Map 아이템은 이미 Library에 존재.)
3. **이미지 오픈**: `static void openImage(path)` 헬퍼 추가. 플랫폼별 분기:
   - Windows: `start`, macOS: `open`,
   - Linux/WSL: `xdg-open` 우선, 실패 시 `wslpath -w`로 Windows 경로 변환 후 `explorer.exe`.
   - `<cstdlib>`의 `std::system` 사용 (핸드아웃 허용 헤더).
4. `printHelp`의 `map` 설명을 "그래프 + 이미지 (needs Map item)"로 갱신.

**설계 결정**
- `showMap()`은 **그래프 인접 리스트(텍스트)도 그대로 출력**한 뒤 이미지를 연다.
  사유: 루브릭의 "Map or adjacency display works"는 그래프 자료구조의 텍스트 출력이 증거이고,
  GUI 없는 채점 터미널에서도 동작해야 하므로 텍스트를 유지. 이미지는 그 위에 추가한 시각 자료.
  (이미지 전용으로 바꾸길 원하면 텍스트 출력 제거 가능.)

**검증**
- `make clean && make` 성공 (경고 없음).
- Map 없이 `map`: 차단 메시지 정상.
- Library에서 `take Map` 후 `map`: 그래프 7개 방 인접 리스트 출력 + "(Opening the map image...)" 후 이미지 오픈 명령 실행 확인.
- `wslpath -w assets/map.png` → `C:\Users\user\COS_prac\assets\map.png` 변환 확인.

**알려진 한계 / 주의**
- 이미지 팝업은 GUI라 헤드리스 환경에선 시각 확인 불가. WSL에서 직접 실행 시 Windows 기본 뷰어로 열림.
- `explorer.exe`는 성공해도 종료코드 1을 반환하므로 종료코드로 성공 여부 판단 불가(정상 동작).
- 채점 환경이 GUI 없는 순수 터미널이면 이미지는 안 뜨고 그래프 텍스트만 보임(의도된 폴백).
- 경로가 상대경로(`assets/map.png`)라 **반드시 프로젝트 루트에서 `./dungeon_explorer` 실행**해야 이미지가 열림.

## 2026-05-31 — 외부 뷰어 방식 → 터미널 인라인 이미지(Sixel)로 교체

**요청**: 지도가 외부 파일을 외부 뷰어로 여는 게 아니라 **코드 내에서** 열렸으면 함.
(이전 방식은 `explorer.exe`로 디스크의 `assets/map.png`를 별도 창으로 여는 것이었음.)

**사용자 결정 (AskUserQuestion)**: 콘솔 C++의 제약을 설명하고 두 안 제시 → **터미널 인라인 이미지(Sixel)** 선택. 이미지 데이터를 코드에 내장.

**변경 파일**: `src/Game.cpp`, 신규 `include/MapImage.h`(자동 생성), 보조 `assets/map.six`.

**변경 내용**
1. **Sixel 인코딩**: 환경에 img2sixel/ImageMagick/libsixel가 없어 **PIL로 Sixel 인코더를 직접 작성**.
   `assets/map.png`를 640px로 축소·16색 양자화 후 Sixel로 인코딩(RLE 적용), 28KB.
2. **코드 내장**: Sixel 바이트를 이스케이프된 C++ 문자열 리터럴로 `include/MapImage.h`에
   `static const char* MAP_SIXEL`로 내장. **런타임에 디스크를 읽지 않음**(바이너리에 컴파일됨).
   - 이스케이프: `"`→`\"`, `\`→`\\`, ESC 등 비출력문자→3자리 8진, `?`→`\?`(C++ trigraph 회피).
3. **`showMap()` 수정**: 외부 뷰어 호출 제거 → 그래프 텍스트 출력 후 `std::cout << MAP_SIXEL`로 인라인 출력.
4. **`openImage()` 헬퍼 삭제**(미사용). `std::system` 미사용이 됐지만 `<cstdlib>`는 `rand/srand`로 계속 필요 → 유지.

**설계 결정**
- 이미지 데이터를 **헤더에 내장**한 이유: 사용자의 "코드 내에서" 요구 충족 + 런타임 파일 의존 제거.
  `assets/map.png`/`map.six`는 이제 **재생성용 소스**일 뿐 실행에는 불필요.
- 그래프 텍스트 출력은 그대로 유지(루브릭 증거 + Sixel 미지원 터미널 폴백).
- Makefile은 수정 불필요(`include/`는 이미 `-Iinclude` 대상).

**검증**
- `make clean && make` 성공, **경고 없음**(trigraph 경고는 `\?` 이스케이프로 제거).
- `map` 출력에 Sixel 시작(`ESC P q`)·종료(`ESC \`) 프레이밍 정상 포함 확인, 그래프 텍스트 동반 출력 확인.
- Map 아이템 게이팅 정상 유지.

**알려진 한계 / 주의**
- **Sixel 지원 터미널에서만** 그림으로 보임(최신 Windows Terminal v1.22+ 등). 미지원 터미널·헤드리스에선 깨진 문자열로 출력됨 → 그래프 텍스트가 폴백.
- 개발 셸(xterm-256color)에선 렌더 확인 불가하여 데이터 프레이밍만 검증함. 실제 렌더는 Sixel 터미널에서 사용자 확인 필요.
- 지도 이미지를 바꾸려면 `assets/map.png` 교체 후 위 인코딩 스크립트로 `include/MapImage.h` 재생성.

## 2026-05-31 — move 번호 선택(인터랙티브 이동) 추가

**요청**: 현재 위치에서 `move`만 치면 갈 수 있는 방 목록이 뜨고, 번호로 골라 이동.

**변경 파일**: `include/Game.h`, `src/Game.cpp`.

**변경 내용**
1. 공통 이동 로직을 `Game::moveToRoom(int)`으로 추출 (이동 기록 push, 위치 설정, +1점, 방문 표시, Library 힌트, look).
   - `moveByRoomName`과 인터랙티브 이동이 **동일 로직 공유** → undo 스택 등 동작 일관.
2. `Game::moveInteractive()` 추가: 현재 방의 이웃을 `1) ... N)`로 출력 → 번호 입력 받기.
   - 빈 입력 → "Move cancelled.", 범위 밖/비숫자 → "Invalid selection." (잘못된 입력 안전 처리).
   - 그래프의 `neighborCount`/`getNeighbor`로 목록 구성 → 번호=이웃 인덱스로 매핑.
3. `moveByRoomName`: 인자가 비면 `moveInteractive()` 호출. **기존 `move <방이름>` 직접 입력은 그대로 유지**.
4. `printHelp`의 move 설명을 "type 'move' alone to pick by number"로 갱신.

**검증**
- `make clean && make` 성공 (경고 없음).
- `move` 단독 → 이웃 5개 번호 목록 → 2번 선택 → IoT Laboratory 이동 확인.
- 잘못된 번호(99) → "Invalid selection.", 빈 입력 → "Move cancelled." 확인.
- 번호 이동 후 `undo` → 이전 방 복귀 확인(이동 기록 스택 정상).
- 기존 `move Library` 직접 이동도 정상.

## 2026-05-31 — 시작 시 이름 입력 + 점수 영속화(리더보드 누적) + sortscores 하드코딩 수정

**요청**: 게임 실행 시 이름을 입력하고, 지금껏 플레이한 사람들이 rank에 뜨도록(점수 순위).

**변경 파일**: `include/Player.h`, `src/Player.cpp`, `include/ds/ScoreTree.h`, `src/ScoreTree.cpp`,
`include/Game.h`, `src/Game.cpp`. (런타임 생성: `scores.txt`)

**변경 내용**
1. **이름 입력**: `Player::setName()` 추가. `run()` 시작 시 "Enter your name:"로 입력받아 설정(빈 입력이면 기본 "Explorer").
2. **점수 영속화** (`fstream`, 허용 헤더):
   - 파일 `scores.txt`, 한 줄 = `"<score> <name>"` (이름의 공백 허용 — score 먼저 읽고 나머지 줄을 이름으로).
   - `loadScores()`: 시작 시 파일 → BST 적재(생성자에서 `seedScores()` 대신 호출). 파일 없으면 빈 리더보드.
   - `saveScores()`: 종료 시 BST 전체를 파일에 **다시 씀**(현재 플레이 기록 포함, 중복 누적 방지). `run()` 끝의 insert 직후 호출.
   - `run()` 시작 시 비어있지 않으면 "Leaderboard so far"로 과거 플레이어 순위 출력 → "실행 시 rank에 뜸" 요구 충족.
3. **BST 열거 API**: `ScoreTree::copyInto(out, capacity)` 추가(중위순회 복사). 영속화·정렬에서 실제 트리 데이터를 꺼내는 데 사용.
4. **`showSortedScores` 하드코딩 제거**(이전 로그에서 지적한 버그): `Ada/Grace/Linus` 손코딩 →
   `scoreTree.copyInto()`로 실제 기록 + 현재 플레이어 점수를 합쳐 선택 정렬. **단일 출처**가 됨.
5. 하드코딩 시드(`seedScores`의 Ada/Grace/Linus) 제거 → 리더보드는 실제 플레이어로만 누적.

**설계 결정**
- 저장 시 **append가 아니라 BST 전체 재기록**: load→insert(1명)→save로 정확히 N+1 누적, append 중복 위험 없음.
- 같은 이름 재플레이는 별도 기록으로 누적(BST 중복 정책: 동점은 오른쪽). 루브릭 "Duplicate-score policy is defined"에 부합.
- `scores`(BST 중위순회)는 플레이 중엔 현재 플레이어 미포함(종료 시 insert), `sortscores`는 현재 플레이어 포함. 최종 랭킹엔 전원 포함.

**검증**
- `make clean && make` 성공 (경고 없음).
- 1회차 Jinju(2점) → `scores.txt`에 `2 Jinju` 저장.
- 2회차 시작 시 "Leaderboard so far"에 Jinju 표시, 종료 후 Jinju+Chaemin 누적 확인.
- 공백 이름 "Su Been Choi" 저장·로드·표시 정상.
- `sortscores`가 하드코딩 없이 실제 누적 점수+현재 플레이어로 정렬됨 확인.
- 테스트용 `scores.txt`는 삭제(첫 실제 플레이 때 생성).

**주의**
- `scores.txt`는 상대경로라 **프로젝트 루트에서 실행**해야 동일 리더보드가 유지됨.

## 2026-05-31 — 누적 점수 초기화 실행 옵션(--reset) 추가

**요청**: 누적 스코어를 초기화하는 실행 옵션 추가.

**변경 파일**: `include/Game.h`, `src/Game.cpp`, `src/main.cpp`.

**변경 내용**
1. `Game::resetScores()` **정적 메서드** 추가: `std::remove(SCORES_FILE)`로 리더보드 파일 삭제.
   - 삭제 성공 → "Leaderboard reset (...)", 파일 없음 → "Nothing to reset (...)" 안내.
   - 정적으로 둔 이유: Game 인스턴스 생성(=loadScores 등) 없이 초기화만 하고 끝내기 위함.
   - `SCORES_FILE` 단일 출처 유지(파일명을 main이 따로 알 필요 없음).
2. `main()`을 `int main(int argc, char** argv)`로 변경: 첫 인자가 `--reset`이면 `Game::resetScores()` 호출 후 즉시 종료. 그 외에는 평소대로 게임 시작.
3. `<cstdio>` 추가(`std::remove`).

**사용법**
- 초기화: `./dungeon_explorer --reset`
- 게임 실행: `./dungeon_explorer` (인자 없음)

**검증**
- `make clean && make` 성공 (경고 없음).
- 점수 파일 존재 시 `--reset` → 삭제 + "Leaderboard reset" 확인.
- 파일 없을 때 `--reset` → "Nothing to reset" 확인.
- 인자 없이 실행 → 정상적으로 이름 입력 프롬프트부터 시작 확인.

## 2026-05-31 — README 빌드/실행 안내에 --reset 옵션 추가

**요청**: README의 빌드/실행 안내에 `--reset` 옵션 추가.

**변경 파일**: `README.md`.

**변경 내용**
- `Build & Run`의 명령 블록에 `./dungeon_explorer --reset`(리더보드 초기화 후 종료) 줄 추가, 주석 정렬 정리.
- 점수가 `scores.txt`에 누적되며 `--reset`으로 초기화한다는 설명 한 줄 추가.

**참고 (이번엔 수정 안 함, 별도 요청 시 갱신 가능)**
- README의 일부 안내가 코드 변경으로 오래됨: `move`는 번호 선택 지원, `vocab` 명령 누락,
  `status`는 HP 없이 점수만 표시, C404는 "English-definition quiz"가 아니라 영어 단어 문제,
  `map`은 Map 아이템 필요. 요청 시 Commands 표 일괄 갱신 가능.

## 2026-05-31 — return 명령어(KENTECH Hall로 바로 이동) 추가

**요청**: `return` 명령으로 어디서든 바로 KENTECH Hall로 이동.

**사실 확인**: 사용자는 "모든 방이 Hall과 연결됨"을 근거로 들었으나, 실제 그래프상
**Bus Station은 Hall과 직접 연결 안 됨**(Cafe STAY 경유). 따라서 그래프 간선 이동이 아니라
**어디서든 Hall로 점프하는 편의(텔레포트) 명령**으로 구현(의도와 일치).

**변경 파일**: `include/Game.h`, `src/Game.cpp`.

**변경 내용**
1. `Game::returnToHall()` 추가: `findRoomIdByName("KENTECH Hall")`로 Hall id를 찾아 `moveToRoom()` 호출.
   - 이미 Hall이면 "You are already at KENTECH Hall." 안내.
   - `moveToRoom` 재사용 → 이동 기록 push(undo 연동), +1점, 방문 표시, look()까지 일관 동작.
2. `processCommand`에 `return` 분기 추가, `printHelp`에 한 줄 추가.

**검증**
- `make clean && make` 성공 (경고 없음).
- C404(인접)·Bus Station(비인접) 모두에서 `return` → Hall 이동 확인(텔레포트 포함).
- 이미 Hall일 때 안내 메시지, `return` 후 `undo`로 직전 방 복귀 확인.

## 2026-05-31 — 몬스터 처치 순서/아이템 게이팅 + 열쇠조각·합성 + timetable

**요청**: 몬스터 처치 순서(George→Hyunwoo Lee→SeungHyeon Yoon) 구현. 각 몬스터 처치 시
(1) 다음 몬스터에 도전 가능한 아이템 획득, (2) 열쇠 조각 드롭. EunJeong Kim 제외 전원 처치 후
조각을 합치면 하나의 Key. 순서를 알려주는 timetable 아이템도 Library에서 획득.

**변경 파일**: `include/ds/Inventory.h`, `src/Inventory.cpp`, `include/Game.h`, `src/Game.cpp`.

**진행 구조**
- George (C404, 1st): 전제 없음 → 처치 시 `KeyPiece` + `HyunwooPass` 드롭.
- Hyunwoo Lee (IoT, 2nd): `HyunwooPass` 필요 → 처치 시 `KeyPiece` + `SeungHyeonPass` 드롭.
- SeungHyeon Yoon (A205, 3rd): `SeungHyeonPass` 필요 → 처치 시 `KeyPiece` 드롭.
- `KeyPiece` 3개 → `combine` 명령으로 `Key` 합성(조각 3개 소비).
- EunJeong Kim (Bus Station, final): `Key` 필요.
- 순서는 Pass 아이템 사슬로 자연히 강제됨(Hyunwoo는 George 처치 후에만 도전 가능 …).

**변경 내용**
1. `Inventory::countItem(name)` 추가(연결 리스트 순회로 동일 이름 개수 세기) — 열쇠조각 카운트용.
2. `Game::missionPrereqItem(MissionType)` : 몬스터별 전제 아이템 반환(George는 빈 문자열).
   `runEncounter`에서 미션 시작 전에 전제 아이템 보유 검사 → 없으면 차단 메시지.
3. `Game::dropMissionRewards(MissionType)` : 승리 시 미션별로 KeyPiece + 다음 Pass 지급.
   `runEncounter`의 승리 처리에서 호출.
4. `Game::combineKeyPieces()` (`combine` 명령) : KeyPiece 3개 확인 → 3개 제거 후 Key 추가.
   3개 미만이면 보유 개수 안내, 이미 Key 있으면 안내.
5. `Game::showTimetable()` (`timetable` 명령) : Timetable 아이템 보유 시 처치 순서 출력(vocab과 동일 패턴).
6. Library에 `Timetable` 아이템 추가. `printHelp`에 `timetable`·`combine` 줄 추가.

**설계 결정**
- Timetable은 **정보용**(순서 안내)만, George의 전제로는 두지 않음. 순서 강제는 Pass 사슬로 충분
  (SeungHyeon 도전엔 Hyunwoo의 드롭이 필요 → 자동으로 George가 가장 먼저). 사용자의 "George가 첫 번째"와 일치.
- 열쇠조각은 단일 이름 `KeyPiece`로 누적 → `countItem`으로 개수 판정, `combine`에서 3개 소비.
  인벤토리(단일 연결 리스트) 자료구조의 추가 활용 사례가 됨.
- 전제 아이템(Pass/Key)은 소비하지 않고 보유만 검사(재도전 불가하므로 1회성).

**검증**
- `make clean && make` 성공 (경고 없음).
- timetable: 아이템 보유 시 순서 출력 정상.
- 순서 강제: HyunwooPass 없이 Hyunwoo 도전 → "won't face you yet" 차단 확인.
- combine: 조각 0개 → "0/3" 안내 확인.
- 처치 드롭: George 승리 시 KeyPiece+HyunwooPass 인벤토리 반영 확인.
- **전체 사슬 end-to-end**(랜덤 퀴즈 반복 시도로 전체 클리어 1회 확보):
  George→Hyunwoo→SeungHyeon 순차 처치 + 각 드롭 + `combine`로 Key 합성 + EunJeong Key 게이팅 통과 +
  "completed the final mission"까지 확인.

## 2026-05-31 — Key로 Bus Station 문 잠금/해제

**요청**: Key가 있어야 Bus Station에 진입(문 열림). 그 전엔 못 감. 진입 후 EunJeong Kim event 진행.

**변경 파일**: `src/Game.cpp`.

**변경 내용**
1. `moveToRoom()` 맨 앞에 게이트 추가: 목적지가 "Bus Station"이고 인벤토리에 `Key`가 없으면
   "The gate to the Bus Station is locked. You need the Key to open it." 출력 후 이동 취소.
   - `moveToRoom`이 이름 이동·번호 이동·return의 **단일 관문**이라, 모든 경로의 진입이 한 번에 게이팅됨.
2. `moveInteractive()` 목록에서 Key 미보유 시 Bus Station 항목에 " (locked - needs Key)" 표시.

**설계 결정**
- 진입 게이트와 기존 EunJeong event 전제(`missionPrereqItem(FinalRC)=Key`)가 둘 다 Key를 요구 →
  진입 시점에 이미 Key 보유가 보장되므로 event 전제는 사실상 안전망(중복이나 무해). 그대로 유지.
- 게이트는 `moveToRoom` 한 곳에만 둠(그래프/Room 코드는 게임 아이템을 몰라야 하므로 거기엔 넣지 않음).

**검증**
- `make clean && make` 성공 (경고 없음).
- Key 없이 Cafe STAY→Bus Station: 차단 메시지 + Cafe STAY에 잔류 확인. 인터랙티브 목록 "(locked - needs Key)" 확인.
- **Key 보유 시 진입 성공 → 최종 클리어**: 같은-초 시드 고정을 역이용한 적응형 테스트
  (1단계 문제 탐지 → 정답 산출 → 재실행)로 결정적 재현 — combine→Key→Bus Station 진입→
  "completed the final mission" 확인.

**테스트 메모**
- `srand(time())`은 초 단위 시드라, 루프에서 빠르게 연속 실행하면 같은 초 프로세스가 동일 문제를 받음.
  고정 답 brute-force가 막혔던 이유. 문제를 먼저 탐지해 정답을 맞추는 적응형 방식으로 해결.

## 2026-05-31 — EunJeong Kim 최종 이벤트를 KENTECH 퀴즈로 변경

**요청**: EunJeong Kim의 event를 "RC 키워드 입력"에서 **KENTECH 학교 관련 퀴즈**로 변경.

**변경 파일**: `include/Monster.h`, `include/Game.h`, `src/Game.cpp`.

**변경 내용**
1. 최종 미션을 RC 키워드 입력 → **KENTECH 객관식 퀴즈**로 교체. 다른 퀴즈와 동일하게
   `ChoiceQuestion` 풀 + `askChoice`(랜덤 출제) 사용. 문제 4종:
   - KENTECH 약자(Korea Institute of Energy Technology)
   - 특화 분야(Energy)
   - 위치(Naju)
   - 핵심 과제(Energy and climate)
2. 내부 명칭 정리(내용과 일치하도록): `MissionType::FinalRC`→`FinalQuiz`, `runFinalRC`→`runKentechQuiz`.
   관련 5개 참조(monster 생성, switch, gameCleared 체크, missionPrereqItem, 주석) 일괄 수정.
3. `libraryHint()`의 Bus Station 안내를 "campus radio call sign" → "a quiz about KENTECH itself"로 갱신.
4. RC 비교에만 쓰이던 `toUpper()` 헬퍼가 미사용이 되어 제거(경고 해소).

**주의 (사실 확인 필요)**
- KENTECH 관련 사실(약자/분야/위치/미션)은 일반 상식 기준으로 작성. 팀에서 정확성 확인 후
  필요시 `runKentechQuiz`의 풀을 수정 권장(학교 공식 정보 기준).

**검증**
- `make clean && make` 성공 (경고 없음, toUpper 제거 후 확인).
- 잔존 `FinalRC`/`runFinalRC`/"call sign" 참조 없음 확인.
- 적응형 테스트로 Bus Station 도달 → 최종 퀴즈 출력 + 정답 시 "+100 score"·클리어 확인.

## 2026-05-31 — 잡몬스터(망령) 랜덤 인카운터 + Heart(HP) + Cafe STAY 상점 추가

**요청**: (1) 무작위로 튀어나오는 잡몬스터 2종(켄텍 대학생/대학원생의 망령)을 추가. 등장 시
눈앞의 단어/짧은 문장을 시간(10초) 내에 빠르게 타이핑, 실패 시 HP 감소. (2) HP는 `Heart`로
기본 3, 1씩 감소, 0이면 게임 오버. (3) Cafe STAY에서 점수로 회복 아이템을 사서 회복.

**사용자 결정 (AskUserQuestion)**: 제한시간=10초(엔터 제출 시점 판정), HP 0=게임 오버,
상점=`buy`(구매→인벤토리) + `use`(소비→회복) 2단계.

**변경 파일**: `include/Player.h`, `src/Player.cpp`, `include/Game.h`, `src/Game.cpp`.
(Monster.h/Room/Makefile/main.cpp 변경 없음, 신규 파일 없음.)

**변경 내용**
1. **Heart(HP)** (`Player`): `hp/maxHp`(기본 3) 필드 + `getHp/getMaxHp/loseHeart/heal/isAlive/spendScore`.
   `printStatus`에 `Hearts: <3 <3 <3 (3/3)` 표시 추가(ASCII 안전).
2. **망령 인카운터** (`Game`, 파일 스코프 풀):
   - `WRAITH_WORDS`(단어, 대학생용) / `WRAITH_SENTENCES`(짧은 문장, 대학원생용).
   - `typeChallenge(target)`: target 출력 → `time()`로 제출까지 경과 초 측정 →
     `trim(line)==target && elapsed<=10`이면 성공. 오타→"Mistyped", 초과→"Too slow".
   - `runWraithEncounter()`: 50/50로 대학생(단어)/대학원생(문장) 선택 → 성공 "banished",
     실패 `loseHeart()` + 상태 출력.
   - `maybeRandomEncounter()`: 매 명령 후 `AMBUSH_PERCENT`(=25%) 확률로 등장.
3. **Cafe STAY 상점** (`Game`): 파일 스코프 `SHOP_GOODS`(Coffee 10점/+1, Financier 20점/+2).
   `showShop()`(메뉴, Cafe STAY에서만), `buyItem()`(점수 차감→인벤토리),
   `useItem()`(인벤토리 소비→`heal`, 만땅이면 거부). `buildKentechWorld`의 무료 Coffee/Financier
   2줄 제거(상점 상품으로 전환). 명령 `shop`/`buy`/`use` + `printHelp` 3줄 추가.
4. **메인 루프** (`run`): `processCommand` 직후 `maybeRandomEncounter()` 호출,
   `!isAlive()`면 "*** GAME OVER ***" 출력 후 종료(점수는 기존대로 저장/랭킹).

**설계 결정**
- 제한시간은 **엔터 제출 시점**에 경과초로 판정(입력 도중 강제 차단 X). 콘솔 C++에서 이식성
  우선(헤드리스 채점 터미널에서도 동작). 초 단위 해상도라 약간 관대하나 "10초 안에" 요구 충족.
- 망령은 교수 진행(`MissionType`/Monster) 체계와 무관 → Monster 클래스 미변경, Game 안에서만 처리.
- `use`를 별도 명령으로 둬 인벤토리(단일 연결 리스트) 자료구조 활용을 한 단계 더 노출.

**검증** (`AMBUSH_PERCENT`/`TYPE_LIMIT_SEC`/시작 HP를 임시 고정해 결정적으로 확인 후 원복)
- `make clean && make` 성공 (경고 없음).
- 상점: 메뉴 출력, `buy Coffee`(-10), 인벤토리 반영, 점수 부족/미보유/만땅 거부 모두 확인.
- 회복: 시작 HP=1 고정 → `use Coffee`로 1/3→2/3, 아이템 소비 확인.
- 망령: 두 종류(단어/문장) 등장 확인. 정답+시간내→"banished"(같은-초 시드로 정답 캡처),
  오타→-1 Heart, 시간초과(limit -1로 강제)→"Too slow"+-1 Heart 확인.
- 게임 오버: 3회 실패 → 0/3 → "GAME OVER" 출력 후 종료 확인.
- 원복: `AMBUSH_PERCENT=25`, `TYPE_LIMIT_SEC=10`, 시작 HP=3 복구 확인.

**튜닝 포인트 (필요 시 조정)**
- 등장 확률 `AMBUSH_PERCENT`(25), 제한시간 `TYPE_LIMIT_SEC`(10),
  상점 가격/회복량 `SHOP_GOODS`, 단어/문장 풀.

## 2026-05-31 — 상점 가격 통일(7) + 클리어 시 HP 점수 환산 + score 획득 경로 축소

**요청**: (1) Coffee/Financier 가격을 모두 score 7로. (2) 게임 클리어 시 남은 HP를 1당 7점으로
환산해 점수에 가산. (3) 앞으로 score는 **몬스터 처치**와 **아이템 획득** 시에만 오르게.

**변경 파일**: `src/Game.cpp` 만 수정.

**변경 내용**
1. `HEART_SCORE = 7` 상수 도입(“Heart 1개 = 7점”의 단일 출처). `SHOP_GOODS`의 Coffee/Financier
   가격을 모두 `HEART_SCORE`(=7)로 통일(회복량은 Coffee +1 / Financier +2 유지).
2. **클리어 보너스**: `runEncounter`의 FinalQuiz 승리 처리에서 `gameCleared=true` 직후
   `player.getHp() * HEART_SCORE`를 점수에 가산하고 "Hearts bonus: N x 7 = +M" 출력.
3. **score 획득 경로 축소**: `addScore` 호출 4곳 중 2곳 제거 —
   - `moveToRoom`의 이동 +1점 제거(이동으로는 점수 안 오름).
   - `processOneEvent`의 `event.scoreDelta` 가산 제거(이벤트로는 점수 안 오름, 설명/상태 출력은 유지).
   - 유지: `takeItem`의 아이템 value(아이템 획득), `runEncounter`의 reward(몬스터 처치), 그리고 위 클리어 보너스.

**설계 결정**
- 상점가와 클리어 환산을 같은 `HEART_SCORE` 상수로 묶어 "Heart의 점수 가치"를 한 곳에서 관리.
  (둘 다 7이므로 가격 7=Heart 1개치, Financier는 같은 값에 +2 회복이라 가성비 우위 — 요청대로 가격은 동일.)
- `GameEvent::scoreDelta` 필드는 기존 구조라 그대로 두고 가산만 중단(미사용이 됐으나 사전 존재 코드라 미삭제).

**검증** (`AMBUSH_PERCENT`/시작위치/Key를 임시 고정해 결정적으로 확인 후 원복)
- `make clean && make` 성공 (경고 없음).
- 경제: 이동·event 후 score 0 유지, `take VocabularyList` 후 +15 확인.
- 상점: Cafe STAY에서 Coffee/Financier 모두 "7 score" 표기 확인.
- 클리어 보너스: 시작을 Bus Station+Key로 임시 고정, 같은-초 시드로 최종 퀴즈 정답 →
  reward 100 + HP 3×7=21 = **121점** 가산 + "Hearts bonus: 3 x 7 = +21" 출력 확인.
- 원복: 시작 위치/Key 임시 코드 제거, `AMBUSH_PERCENT=25` 복구, `addScore`가 아이템·몬스터·클리어보너스
  3곳만 남음 확인.

## 2026-06-01 — 후속 밸런스/설정 조정 모음

이전 엔트리(상점·HP·score) 이후 사용자 요청으로 진행한 작은 조정들을 묶어 기록.

**변경 파일**: `src/Player.cpp`, `src/Game.cpp`.

**변경 내용**
1. **시작 score 10** (`Player.cpp`): 생성자 `score(0)` → `score(10)`.
   효과: 시작하자마자 Cafe STAY에서 Coffee(7) 1회 구매 가능.
2. **Financier 가격 2배 = 14** (`Game.cpp` `SHOP_GOODS`): `HEART_SCORE`(7) → `HEART_SCORE * 2`.
   사유: 회복 +2라 "Heart 1개=7점" 기준으로 가격도 2배. Coffee는 7 유지.
3. **타이핑 제한 10초 → 15초** (`Game.cpp` `TYPE_LIMIT_SEC`).
4. **교수 event 패배 시 HP 감소** (`Game.cpp` `runEncounter` 패배 분기 1곳):
   `player.loseHeart()` + "-1 Heart" 추가, HP 0이면 "run out of Hearts" 안내 →
   기존 메인 루프 `isAlive()` 검사가 GAME OVER 처리. 네 교수(George/Hyunwoo/SeungHyeon/EunJeong)가
   같은 패배 경로를 공유하므로 한 곳 수정으로 전부 적용. (망령과 동일한 HP 패널티 체계로 통일.)
5. **잡몹 등장 확률 25% → 18%** (`Game.cpp` `AMBUSH_PERCENT`): "조금 많이 나온다"는 피드백 반영해 소폭 하향.

**검증**
- 각 변경 후 `make` 성공 (경고 없음).
- 시작 score: `status` → `Score: 10` 확인.
- 상점가: Cafe STAY에서 `Coffee - 7`, `Financier - 14` 확인.
- 타이핑: 망령 프롬프트 `Type this within 15 seconds:` 확인.
- 교수 패배 HP: George 3회 오답 → HP 3→2→1→0 → "run out of Hearts" → GAME OVER 확인
  (검증 위해 `AMBUSH_PERCENT` 임시 0 고정 후 25→18로 원복).
