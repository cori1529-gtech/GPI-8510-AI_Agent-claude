# CLAUDE.md — GPI-8510 축산전용 인디케이터 펌웨어

> 이 문서는 Claude Code가 본 프로젝트에서 코드를 작성/수정할 때 반드시 따라야 할 규칙과
> 하드웨어/소프트웨어 컨텍스트를 정의한다. 모든 작업 전에 이 문서를 기준으로 판단할 것.
> 분석 근거: main.c(V4.0), hw_config.c/.h, usart.c, CAL.C, def.h, global.h, stm32_high.uvproj, 회로도(GPI-8510-ET-000-00 R0)

---

## 1. 프로젝트 개요

| 항목 | 내용 |
|------|------|
| 제품명 | GTECH-PMA-8510 V2.0 (GPI-8510) 축산전용 인디케이터 |
| 펌웨어 버전 | **V4.0** (2025.12.10) — def.h의 `VERSION` 매크로 |
| 용도 | 사일로(silo) 사료 무게 계측, 배출량 계산, 원격 전송 (LTE/CDMA, Zigbee, BLE) |
| 주요 기능 | 로드셀 2~3채널 무게 측정, LCD 표시, 배출량 계산/전송, SMS 원격제어, 원격 펌웨어 업데이트(IAP) |
| 서버 | 모니터링: `112.217.212.250:8391`, TMS: `112.217.212.250:8397` (global.h) |
| 고객 | 공공기관/지자체, 축산 농가 |

---

## 2. 개발 환경 (절대 변경 금지)

| 항목 | 내용 |
|------|------|
| MCU | **STM32F103ZET6** (144핀, Cortex-M3) |
| Flash | **512KB** (0x08000000 ~ 0x0807FFFF) |
| RAM | **64KB** (0x20000000 ~ 0x2000FFFF) |
| IDE | **Keil MDK uVision** (프로젝트 파일: stm32_high.uvproj) |
| 컴파일러 | ARM Compiler (ARMCC 5), C89/C90 기반 (uC99 비활성) |
| 최적화 | **-O2** (Optimization Level 2) — 변경 금지 |
| 라이브러리 | **STM32 Standard Peripheral Library (StdPeriph)** — HAL 아님! |
| 전처리 정의 | `USE_STDPERIPH_DRIVER, STM32F10X_HD` |
| 출력 파일명 | gpi-8310 |
| 워치독 | IWDG 사용, `IWDG_SetReload(0xFFFF)` ≈ 26.2초, 메인루프에서 `IWDG_ReloadCounter()` 호출 필수 |

### 2.1 클럭 트리 (hw_config.c `bsp_init_rcc()` 검증 완료)
```
HSE 8MHz → PLL ×9 → SYSCLK 72MHz
  HCLK  = SYSCLK      = 72MHz
  PCLK2 = HCLK        = 72MHz  (APB2: GPIO, USART1, TIM8, ADC)
  PCLK1 = HCLK / 2    = 36MHz  (APB1: USART2/3, UART4, TIM2/3, I2C1)
LSE 32.768kHz (RTC용, X2)
```
- 활성 클럭: GPIOA~G 전체, AFIO, TIM2(타임베이스)/TIM3/TIM8, USART1~UART4, DMA1/2, ADC1/ADC3, I2C1(EEPROM), DAC
- NVIC: `NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1)`

### 2.2 ★★★ IAP 부트로더 메모리 맵 (hw_config.c에서 확정 — 극도로 중요)
```c
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);   // bsp_init_interrupt()
```
| 영역 | 주소 | 크기 | 내용 |
|------|------|------|------|
| 부트로더 | 0x08000000 ~ 0x08002FFF | 12KB | IAP 부트로더 (별도 프로젝트) |
| **애플리케이션** | **0x08003000 ~** | ~500KB | 본 펌웨어 (벡터 테이블 오프셋 0x3000) |

- ⚠️ **`NVIC_SetVectorTable(..., 0x3000)` 줄을 절대 수정/삭제하지 말 것.** 이 오프셋이 깨지면 인터럽트가 부트로더 벡터로 점프하여 장비가 즉시 죽는다.
- 앱 빌드 시 Keil Target의 IROM 시작 주소도 0x08003000 기준이어야 함 (링커 설정과 벡터 오프셋은 한 쌍).
- 원격 업데이트(`#REMOTE_PROGRAM_UPLOADING@`)는 이 구조에 의존한다. Flash 쓰기 코드 작성 시 부트로더 영역(0x08000000~0x08002FFF) 침범 금지.

### 2.3 ⚠️ 매우 중요한 제약사항

1. **절대 STM32 HAL/LL 라이브러리 코드를 생성하지 말 것.** 이 프로젝트는 StdPeriph 라이브러리 기반이다.
   - 올바른 예: `GPIO_SetBits(GPIOA, GPIO_Pin_4);`
   - 잘못된 예: `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);` ← 금지
2. **파일 인코딩: CP949 (EUC-KR)** — 소스 파일의 한글 주석은 CP949로 저장되어 있다.
   - 파일 수정 시 UTF-8로 변환 저장하면 Keil에서 한글 주석이 전부 깨진다.
   - 새 파일 생성 시에도 한글 주석을 넣을 경우 CP949 유지 또는 영문 주석 사용.
3. **줄바꿈: CRLF (Windows)** — LF로 바꾸지 말 것.
4. **C89 스타일 준수**: 변수 선언은 블록 최상단에서만. `for(int i=0; ...)` 같은 C99 문법 금지. (`//` 주석은 기존 코드에서 사용 중이므로 허용)
5. 새 소스 파일 추가 시, Keil 프로젝트(.uvproj)에 수동 등록이 필요함을 사용자에게 알릴 것.
6. 빌드는 Keil MDK에서만 가능하다. Claude Code 환경에서 컴파일 검증 불가 → 문법/API 정합성을 특히 신중히 확인할 것.
7. **def.h의 `VERSION` 매크로가 중복 정의되어 있음** (여러 개의 `#define VERSION`이 나열, 마지막 정의가 적용). 버전 올릴 때는 새 줄 추가가 아니라 이 패턴을 정리하도록 제안할 것.
8. **`Delay_ms()`/`Delay_us()`는 타이머가 아닌 busy-wait 루프다** (hw_config.c). 정밀하지 않고 CPU를 점유하므로:
   - 정밀 타이밍이 필요한 곳에 사용 금지 (통신 타이밍은 TIM 기반 카운터 사용)
   - 메인루프/태스크 안에서 긴 Delay 호출 금지 (워치독 + 통신 유실)
   - 최적화 레벨 변경 시 지연 시간이 달라진다 → -O2 고정 유지 이유 중 하나

---

## 3. 하드웨어 구성 (회로도 GPI-8510-ET-000-00 R0 기준)

### 3.1 전원부
- SMPS DC15V/1A 입력 → LM2576/LM2575 벅컨버터
- 전원 레일: +7V, +5V, +3.3V, +3.3V_CDMA, +5VA/-5VA (아날로그), 배터리 7.4V (BAT_CHECK로 감시)

### 3.2 계측부 (로드셀 / ADC)
| 채널 | ADC 칩 | 인터페이스 | 신호명 |
|------|--------|-----------|--------|
| LC1 | CS5555-SS (U1) | 비트뱅잉 시리얼 | LC1_SCLK, LC1_DOUT, LC1_PDWN |
| LC2 | CS5555-SS (U2) | 비트뱅잉 시리얼 | LC2_SCLK, LC2_DOUT, LC2_PDWN |
| LC3 | ADS1251U (IC15) | 비트뱅잉 시리얼 | LC3_CLK, LC3_SCLK, LC3_DOUT |

- 아날로그 전원 +5VA/-5VA 사용, OPA2277/TL074 증폭단 경유
- 사일로 1/2 이중 채널 구조 (변수 접미사 `_2`가 사일로 2번을 의미)

### 3.3 통신부 — UART 확정 맵 (usart.c 검증 완료)

| 포트 | 핀 (TX/RX) | 보드레이트 | 하드웨어 | 용도 | DE(485 방향) 핀 |
|------|-----------|-----------|---------|------|----------------|
| USART1 | PA9 / PA10 | **115200** | MAX3232 (IC10) | 정션보드 RS-232, **디버그 로그**(`USART1_puts`) | - |
| USART2 | PA2 / PA3 | **9600** | MAX489 (IC13) / BLE 공용 | PC 프로그램, RS-422/485, BLE(BOT-CLE310) 절체 | **PA4** (`USART2TX_enable()`) |
| USART3 | PB10 / PB11 | **115200** | MAX3232 (IC12) | **CDMA/LTE 모뎀** (AT 커맨드, SMS) | PB12 (485 TX EN, 초기화만) |
| UART4 | PC10 / PC11 | **115200** | 릴레이 절체 | **Zigbee / Bluetooth** 공용 | - |
| UART5 | (TX5/RX5) | 미초기화 | MAX489 (IC14) | RS-485 온도센서 (현재 `USARTx_Initial()`에 없음) | DE_RS485_5 |

- 초기화 진입점: `USARTx_Initial()` → `USARTx_setting(포트, 보드레이트, 패리티)` (usart.c)
- **RS-485 송신 절차**: `USART2TX_enable()` → PA4 HIGH + `guRS422485TxEnableTimer = 6` (약 6ms 후 타이머가 자동 disable). 송신 코드 작성 시 이 타이밍 로직을 깨지 말 것.
- 모뎀 전원 시퀀스: `Cdma_Power_On()` / `Cdma_Power_Off()` / `modem_power_reset()` 사용

### 3.4 UART 수신 아키텍처 (인터럽트 + 큐)

| 포트 | 수신 방식 | 버퍼 |
|------|----------|------|
| USART1 | IRQ → 노드 큐 시스템 | `gbl_qlist_usart1_rx` / `_free` (q_node_type, queue.c) |
| USART3 (모뎀) | IRQ → 링버퍼 2중 저장 | `RxCdmaQ[8000]` (링큐) + `RxCdmaBuffer[2000]` + SMS 전용 `RxBuffer2_sms[256]` |
| UART4 | IRQ → 링버퍼 | Zigbee/BT 수신 |

- USART3 핸들러는 `#` 또는 `$` 문자로 SMS 패킷 시작을 감지하고, `@`/CR/LF로 종료 판정 → `Usart3_sms_received_flag = 1` 세팅. 메인루프의 `Agent_CdmaTask()`가 이 플래그를 소비한다.
- IRQ 핸들러와 공유되는 변수는 반드시 `volatile` (vu8/vu16/vu32) 유지.
- 버퍼 크기 상수 (def.h): `RX_CDMA_BUFFER_SIZE=2000`, `RXCARDBUFFER2SIZE=2000`, `MAX_RXBUFFER2_SMS=256`, `RXCDMAQ_SIZE=8000` (global.h)

### 3.5 메모리 & RTC
| 장치 | 칩 | 인터페이스 | 용도 |
|------|-----|-----------|------|
| EEPROM | AT24C16 (IC9) | **하드웨어 I2C1: PB6(SCL)/PB7(SDA), AF_OD** (hw_config.c 검증) | 설정값(factor) 저장 — `read_factors()`/`write_factors()` |
| SPI Flash | W25Q128 (IC8) | SPI1 (PA5/PA6/PA7, SPI_NSS) | 한글 폰트, IAP 펌웨어 저장 |
| RTC | HT1381 (IC7) | 3-wire (CLOCK_SC, CLOCK_IO, CLOCK_RE) | 날짜/시각 (32.768kHz X1) |

- ⚠️ **EEPROM 접근 전 `FSMC_DISABLE` 필수** (매크로는 def.h: `RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE)`). FSMC 활성 상태에서는 EEPROM R/W 실패 — main.c 주석 참조.

### 3.6 표시 & 입출력
| 장치 | 인터페이스 | 신호 |
|------|-----------|------|
| LCD (OLED, 128x64급) | 8비트 패러렐 | OLED_DB0~DB7, OLED_RD, WR, DC, RESET, CS |
| 멤브레인 키 | GPIO | KEY5 ~ KEY9 |
| 부저 | GPIO + TR (C3198) | BUZZER |
| 릴레이 출력 4CH | GPIO | OUTPUT1 ~ OUTPUT4 |
| 절연 입력 4CH | GPIO | INPUT1 ~ INPUT4 |
| 옵션 스위치 | GPIO | OPTION_D0 ~ D6, BOOT0 |

- 한글 폰트: 16x16 / 24x24, SPI Flash 저장, `lcd70_show_string_mode_24x24()`, `GLCD_string()` 등으로 출력
- 다국어: `gnlLanguage` (1 = 한글, 그 외 = 영문) — 화면 문자열 작성 시 양쪽 분기 필요

### 3.7 GPIO 초기화 분산 주의
- `bsp_init_gpio()` (hw_config.c)는 **I2C1 핀(PB6/PB7)만** 설정한다.
- 나머지 GPIO는 `bsp_key_gpio_init()` (key.c 추정, 릴레이/LCD 포함), `USARTx_setting()` (UART 핀), 각 드라이버 파일에 분산되어 있음.
- → Phase 1 BSP 통합 시 이 분산된 초기화를 모두 찾아 `board_gpi8510.h` 기준으로 재구성할 것.

---

## 4. 소프트웨어 구조

### 4.1 파일 구성 (2026-07-20 갱신 — 4계층 구조 적용 완료)

```
app/      비즈니스 로직 (Agent_* 태스크 — main.c에서 추출됨)
├── agent_baichul_send.c          배출정보 서버 전송 (Agent_BaichulSend_Task)
├── agent_cdma_task.c              모뎀 수신 처리 (Agent_CdmaTask)
├── agent_date_change.c            날짜 변경 이벤트 (Agent_DateChange_Task)
├── agent_discharge_calc.c         배출량 계산 (Agent_DischargeCalc_Task)
├── agent_discharge_input.c        배출 시작/정지 입력 감지 (Agent_DischargeInput_Task)
├── agent_discharge_stop.c         배출 정지 처리
├── agent_discharge_stop_delay.c   정지 후 지연 처리 (Agent_DischargeStopDelay_Task)
├── agent_display.c                무게/상태 LCD 표시 함수 5개
├── agent_make_baichul_packet.c    배출 패킷 생성
├── agent_minute_event.c/.h        1분 주기 이벤트 (Agent_MinuteEvent_Task)
└── agent_weight_display.c         무게 LCD 표시 (Agent_WeightDisplay_Task)

service/  미들웨어 (통신 프로토콜 / 큐 / 파싱 / 무게 필터)
├── agent_input_filter.c   입력 필터링
├── agent_weight.c         무게 계산
├── cdma_queue.c           CDMA 송신 큐 (ZIGBEE_puts / CdmaSendQueue_puts,gets / modem_power_reset)
├── global.c               전역 상태/큐 정의
├── parsing.c              수신 패킷 파싱
├── queue.c                노드 큐 시스템 (q_add_tail, q_remove_tail, q_get_count)
├── rs485.c                RS-485 통신
├── sms.c                  SMS 처리
├── tcpip_send.c           서버 전송(모뎀 TCP/IP) — ★ Send_to_server() 콤마연산자 버그 수정 완료
└── zigbee.c               Zigbee 통신

driver/   하드웨어 드라이버
├── adc.c              CS5555/ADS1251 로드셀 ADC
├── buzzer.c           부저
├── i2c_ee.c           EEPROM (I2C1 하드웨어)
├── key.c              키 입력 (⚠️ 한글 주석 113곳 인코딩 손상 — 복구 불가, 동작은 정상)
├── lcd.c / lcd70.c    LCD 드라이버 (lcd70 = 한글 폰트 출력)
├── led.c              LED (main.c의 Silo_1/2_Led_Operation, Error_1/2_Led_Operation 흡수)
├── sflash.c           W25Q128 SPI Flash
├── timer.c            ★ 실제 HT1381 RTC 드라이버(Get1381/Read1381/Set1381 등) + TIM 타임베이스
└── usart.c            USART1~4 초기화 / IRQ / 송수신

bsp/      보드 정의
└── board_gpi8510.h    핀맵/클럭/벡터 오프셋 — 신규 보드 대응 시 이 파일만 수정하면 됨

src/      아직 이동하지 않은 파일 (레거시 구조 유지, 대부분 CAL/IAP 관련이라 보류 중)
├── main.c              main.c 슬림화 진행 중 — 세션 시작 시 약 8,000줄 → 현재 약 5,080줄.
│                        남은 것: while(1) 수퍼루프, mode_check/cal_mode/set_mode 디스패처,
│                        zero_func/zero_func2, Silo_1_Getweight, goto_iap 등
│                        ★ 전부 CAL(캘리브레이션)·IAP 관련이라 최후순위로 보류 중.
├── CAL.C / CAL.H       ★ 캘리브레이션 모드 (2,400줄) — 현장 데이터 보호 최우선, 리팩토링 보류
├── set.c / set.h       설정 모드 — set.h는 아무 곳에서도 include되지 않는 죽은 헤더(10.2절 참고)
├── var.c / var.h       전역 변수 정의
├── rtc.c               죽은 코드(진짜 RTC 드라이버는 driver/timer.c로 이미 이동됨) — 삭제 여부 미결정
├── spi.c               죽은 코드(호출부 없음) — 삭제 여부 미결정
├── hw_config.c/.h      클럭(bsp_init_rcc)/I2C핀/인터럽트(벡터 0x3000!) 초기화, 비트밴딩 매크로
├── stm32f10x_it.c/.h   인터럽트 핸들러
├── font.c / icon.c     한글 폰트/아이콘 데이터
├── def.h / global.h / type.h / stypes.h   공용 매크로/타입 (아직 미분류)
└── (그 외 driver/service로 옮긴 .c에 대응하는 .h 헤더들 및 StdPeriph 관련 파일들)

lib/
└── STM32_USB-FS-Device_Lib_V3.4.0/   (CMSIS + StdPeriph 라이브러리)
```

- **남은 정리(급하지 않음)**: `driver/`·`service/`로 옮긴 `.c` 파일들의 짝인 `.h` 헤더는 아직 `src/`에 남아있다. Include Path에 `src`도 포함되어 있어 빌드에는 문제없지만, 언젠가 헤더도 같은 폴더로 옮기면 구조가 더 깔끔해진다.

### 4.2 아키텍처: 수퍼루프 + Agent 태스크
`main()` → 초기화 → `while(1)` 무한루프에서 아래 태스크를 폴링 방식으로 순환 호출:

```
while(1) {
    IWDG_ReloadCounter();            // 워치독 리로드 (누락 시 26초 후 리셋!)
    Agent_CdmaTask();                // 모뎀 수신 데이터 처리
    Agent_DischargeInput_Task();     // 배출 시작/정지 입력 감지
    Agent_DischargeStopDelay_Task(); // 정지 후 지연 처리
    Agent_BaichulSend_Task();        // 배출정보 서버 전송
    Agent_DateChange_Task();         // 날짜 변경 이벤트
    Agent_WeightDisplay_Task();      // 무게 LCD 표시
    Agent_DischargeCalc_Task();      // 배출량 계산
    Agent_KeyProcess_Task();         // 키 입력 처리
    Agent_MinuteEvent_Task();        // 1분 주기 이벤트
}
```

- RTOS 없음. 블로킹 지연(`Delay_ms`) 남용 시 워치독 리셋 및 통신 유실 위험 → 새 코드에서 긴 블로킹 금지
- 사일로 1/2 이중화: 동일 로직이 `..._2`, `2` 접미사로 중복 존재
- CDMA 송신은 상태머신 방식: `iSendStage`, `CDMA_STAGE_*`, `STEP_SEND_INIT(1010)`, `STEP_SEND_CMD(1020)` (global.h). 송신 큐: `CdmaSendQueue[10][256]` + head/tail 인덱스.

### 4.3 시간 체계 (global.h — 매우 중요)
```c
#define DEF_1_SEC  100              // ★ 시스템 틱 = 10ms, 100틱 = 1초
#define DEF_1_MIN  (DEF_1_SEC * 60)
```
| 상수 | 값 | 의미 |
|------|-----|------|
| `SEND_INTERVAL_MONITORING` | 60분 | 상태정보 전송 주기 |
| `SEND_SET_INTERVAL_MONITORING` | 360분 | 설정정보 전송 주기 (6시간) |
| `GET_TIME_INTERFAL_TIME` | 1분 | RTC 시각 재독출 주기 |
| `WORKING_SEND_INTERVAL` | 10분 | 배출 중 중량 전송 주기 |

- 타이머 변수: `timer_ms`, `monitoring_timer_ms`, `bachul_send_timer_ms` 등 (모두 volatile, 10ms 틱 기준)
- **새 주기 로직 작성 시 반드시 `DEF_1_SEC`/`DEF_1_MIN` 기반으로 작성** (ms 직접 계산 금지)

### 4.4 캘리브레이션 시스템 (CAL.C — 현장 데이터 보호 최우선 영역)

**⚠️ 이 영역의 변수/수식을 수정하면 현장에 설치된 장비의 무게 정확도가 깨진다. 수정 전 반드시 사용자 확인.**

핵심 변수 (사일로 2번은 `_2` 접미사):
| 변수 | 의미 |
|------|------|
| `v_capacity` | 최대 계량 용량 (예: 250kg) |
| `v_division` | 눈금 수 (분해능 분할 수, 기본 5) |
| `v_e_resolution` | 외부 분해능 = v_capacity / v_division |
| `v_ei_multiply_factor` | 내부/외부 분해능 배수 (= 10 고정) |
| `v_i_resolution` | 내부 분해능 = v_e_resolution × 10 |
| `v_zero` | 영점 AD값 (무부하 상태) |
| `v_full` | 분동(테스트 웨이트) 적재 시 AD값 |
| `v_span` | = v_full − v_zero |
| `v_setting_weight` | 캘리브레이션에 사용한 분동 무게 |
| `gnfFactor` (float) | 최종 환산 계수 — **가장 중요한 값** |
| `v_decimal_point` | 소수점 위치 |
| `lc_number`, `lc_capacity` | 로드셀 개수(최대 6ea), 로드셀 정격 |
| `dZeroValue[8]`, `dFullValue[8]` | 게인 1~64 단계별 캘리브레이션 데이터 |

캘리브레이션 절차 (`cal_mode()` 흐름):
```
Get_division() → Get_Capacity_New() → 내부 분해능 계산
→ cal_zero_span_ad_set(CAL_ZERO_AD_SET)   // 영점 AD 취득
→ Set_Weight()                             // 분동 무게 입력
→ cal_zero_span_ad_set(CAL_SPAN_AD_SET)   // 스팬 AD 취득
→ calc_factor()                            // gnfFactor 산출 + AD 저/고/포화 검사
→ write_factors()                          // EEPROM 저장
```
- `calc_factor()`는 `bAdcLow / bAdcHigh / bAdcSaturation` 검사로 잘못된 캘리브레이션을 거부한다. 이 검사 로직 제거 금지.
- 원격 스팬 조정: `@#RFAC=...@` 명령 → `little_span_adjust()` 경로.
- factor의 EEPROM 저장/복원: `write_factors()` / `read_factors()` (정의는 SET.C 추정 — 미확인).

### 4.5 전송 주기 정책 (비즈니스 로직)
- 상태정보 + 사료 재고량: 1시간 1회
- 배출 시작 시: 상태정보 즉시 전송
- 배출 중: 10분마다 중량 전송 (`WORKING_SEND_INTERVAL`)
- 배출 종료 시: 배출량/재고량/배출시간 전송
- 카운터/아날로그/통신값: 1분 간격, 알람: 즉시

### 4.6 원격 명령 프로토콜 (예시)
```
@#RFAC=12341@                              스팬 팩터 설정
@#RWEIGHT=12345@                           현재무게
@#RZERO=12345@                             영점
@#ZIGBEE=000195000000F4EA,$RFAC=12345@     Zigbee 경유 명령
#RES_MODEMLONE...@                         모뎀 리셋
#REG_TELEPHONE01034431529@                 전화번호 등록
#REMOTE_PROGRAM_UPLOADING@                 원격 펌웨어 업데이트(IAP)
#IP=123.123.123.123:12345@                 업데이트 서버 IP 저장
```
- 패킷 시작 문자: `#` (지테크 서버 경유 SMS), `$` (LTE 자체 SMS) / 종료: `@` (USART3 IRQ 핸들러 파싱 로직)

---

## 5. 코드 컨벤션

| 항목 | 규칙 | 예시 |
|------|------|------|
| 함수명 | `동사_명사()` 소문자+언더스코어 또는 `Agent_기능_Task()` | `read_factors()`, `Agent_WeightDisplay_Task()` |
| 전역변수 | `g` 접두 + 헝가리안 혼용 | `gnfFactor`(float), `gnlMyAddress`(long), `gbHourEvent`(bool성), `giCdmaReturn`(int), `gszCdmaReturn`(string) |
| volatile | IRQ 공유 변수는 `vu8/vu16/vu32` 또는 `volatile int` | `vu32 timer_ms` |
| 타입 | StdPeriph 타입 사용 | `u8, u16, u32, s32, uc32` |
| 상수 | `#define` 대문자 | `#define MONITORING_SERVER_PORT 8391` |
| 주석 | 한글(CP949) + 영어 혼용, 버전 이력은 main.c 상단과 def.h | |
| 사일로 2번 | `_2` 또는 `2` 접미사 | `zero_func2()`, `v_capacity_2` |
| GPIO 비트 제어 | 비트밴딩 매크로 사용 가능 (hw_config.h) | `PBout(5) = 1;` |
| 디버그 출력 | `sprintf(str,...)` + `USART1_puts(str)` | printf 리타겟 없음 |
| 화면 문자열 | `gnlLanguage` 분기 (한글/영문) | `if(gnlLanguage==1) GLCD_string(0,0,"무게조정"); else ...` |

- 기존 코드 스타일을 존중할 것. 리팩토링 시에도 함수/변수 이름 변경은 사용자 승인 후 진행.

---

## 6. 리팩토링 로드맵 (목표: 신규 PCB 펌웨어 개발 3개월 → 3일)

### Phase 1 — BSP 분리 (현재 진행)
- 모든 핀 정의/하드웨어 설정을 `board_gpi8510.h` 단일 헤더로 추출
- 목표: 새 PCB 대응 시 이 파일 하나만 수정하면 되는 구조
- 우선 대상: UART 핀맵(3.3절 표), I2C1(PB6/PB7), OLED_*, KEY*, OUTPUT/INPUT, LC1~3 신호, DE 핀
- 주의: GPIO 초기화가 hw_config.c / key.c / usart.c / 각 드라이버에 분산 (3.7절)

### Phase 2 — 계층 분리
```
app/      비즈니스 로직 (배출량 계산, 전송 정책, 화면 구성, 캘리브레이션 UI)
service/  미들웨어 (통신 프로토콜, 큐, 파싱, 무게 필터, CDMA 상태머신, factor 관리)
driver/   하드웨어 드라이버 (ADC, LCD, UART, EEPROM, Flash, RTC)
bsp/      보드 정의 (핀맵, 클럭, 벡터 오프셋)
```
- 기존 Agent_* 태스크들이 app/ 계층의 뼈대가 됨

### Phase 3 — 표준화
- CLAUDE.md(본 문서) 기반 표준 프롬프트 세트 구축
- 신규 보드 개발 시: 회로도 → BSP 헤더 생성 → 드라이버 재사용 → 앱 로직만 신규 작성

### 리팩토링 작업 규칙
1. **한 번에 한 모듈씩.** 대규모 일괄 변경 금지 — Keil에서 빌드 확인 후 다음 단계 진행.
2. 함수 이동 시 동작 변경 금지 (순수 이동 → 빌드 확인 → 정리 순서).
3. 전역변수는 즉시 제거하지 말고 목록화 → 모듈별 소유권 지정 → 점진적 캡슐화.
4. 인터럽트 핸들러(usart.c, stm32f10x_it.c)와 공유되는 변수는 `volatile` 유지 확인.
5. 수정 후 반드시: "Keil MDK에서 빌드하고 Program Size(Code/RO/RW/ZI)를 이전과 비교하세요"라고 안내.
   - 참고 기준: Code≈46KB, RO≈48KB, RW≈1.3KB, ZI≈23KB (Flash 512KB / RAM 64KB 중 여유 충분)
6. 주석 처리로 남아있는 구버전 코드 블록(중복 `USART2_IRQHandler`, hw_config.c의 미사용 HSI 분기 등)은 이동 대상에서 제외하고, 삭제는 사용자 승인 후.
7. **캘리브레이션(CAL.C) 영역과 IAP 벡터 오프셋(0x3000)은 최후순위 리팩토링 대상** — 잘못되면 현장 장비 무게 정확도/부팅이 깨진다.

---

## 7. Claude Code가 절대 하지 말아야 할 것

1. ❌ HAL/LL/CubeMX 스타일 코드 생성 (StdPeriph만 사용)
2. ❌ 파일을 UTF-8로 재저장 (CP949 유지)
3. ❌ C99/C11 문법 사용 (블록 중간 변수 선언 등)
4. ❌ 메인루프에 긴 블로킹 코드 삽입 (워치독 26초 리셋)
5. ❌ `IWDG_ReloadCounter()` 호출 경로 제거
6. ❌ **`NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000)` 수정/삭제** (IAP 부트로더 구조 파괴)
7. ❌ Flash 쓰기 코드에서 부트로더 영역(0x08000000~0x08002FFF) 접근
8. ❌ FSMC 활성 상태에서 EEPROM 접근 코드 작성 (`FSMC_DISABLE` 선행 필수)
9. ❌ 캘리브레이션 변수(`gnfFactor`, `v_zero`, `v_span` 등)와 `calc_factor()` 수식을 사용자 확인 없이 수정
10. ❌ 사용자 확인 없이 전역변수/함수 이름 일괄 변경
11. ❌ printf 계열을 디버그에 사용 (기존 방식: `sprintf` + `USART1_puts`)
12. ❌ IRQ 공유 변수의 `volatile` 제거
13. ❌ RS-485 DE 타이밍(`guRS422485TxEnableTimer = 6`) 로직 우회
14. ❌ 시간 주기를 ms 하드코딩 (반드시 `DEF_1_SEC`/`DEF_1_MIN` 사용)
15. ❌ `Delay_ms()`를 정밀 타이밍 용도로 사용 (busy-wait, 부정확)
16. ❌ 서버 IP/포트 하드코딩 위치 추가 (global.h의 기존 정의 사용)

---

## 8. 자주 쓰는 작업 프롬프트 템플릿

```
[분석]   "main.c의 XXXX~YYYY 라인 함수를 분석하고 어떤 하드웨어를 건드리는지 설명해줘"
[분리]   "Agent_WeightDisplay_Task()를 app/weight_display.c로 이동해줘.
          동작 변경 없이 순수 이동만 하고, 필요한 extern 선언을 정리해줘"
[BSP]    "회로도 신호명 기준으로 OLED_* 핀들을 board_gpi8510.h에 #define으로 정리해줘"
[드라이버] "CS5555 비트뱅잉 읽기 코드를 driver/adc_cs5555.c로 추출해줘.
          LC1/LC2 채널을 파라미터로 받는 구조로"
[통신]   "USART3 수신 파싱(#...@ 패킷)을 service/modem_parser.c로 분리해줘.
          IRQ 핸들러는 링버퍼 저장만 남기고"
[검증]   "이 수정이 StdPeriph API와 C89 문법, CLAUDE.md 7번 금지사항을 준수하는지 검토해줘"
```

---

## 9. 미확인 정보 (사용자가 추후 보완할 것)

- [x] ~~각 USART 보드레이트~~ → 확인 완료 (3.3절)
- [x] ~~def.h, global.h 내용~~ → 반영 완료
- [x] ~~IAP 부트로더 시작 주소~~ → **확정: 앱 벡터 오프셋 0x3000, 부트로더 12KB** (2.2절)
- [x] ~~EEPROM 인터페이스~~ → **확정: 하드웨어 I2C1, PB6/PB7** (3.5절)
- [x] ~~캘리브레이션 변수/절차~~ → 반영 완료 (4.4절)
- [ ] EEPROM 메모리 맵 상세 (`read_factors()`/`write_factors()` 정의 — SET.C 필요)
- [x] ~~LCD 정확한 모델명/해상도~~ → **확정: 128x64, OLED 아님 — KS0108/HD61202 계열 듀얼칩 GLCD (E/R-W/DI/CS1/CS2 프로토콜). 정확한 모델명은 미확인.** (`board_gpi8510.h` 7절, `lcd.c`의 `lcd_command()`/`lcd_data_write()` 분석으로 확인)
- [ ] Zigbee 모듈 모델명
- [ ] UART5 사용 여부 (하드웨어는 존재하나 `USARTx_Initial()`에서 미초기화)
- [ ] type.h 내용 (q_node_type 정의)
- [ ] IAP 부트로더 프로젝트 (별도 관리 시 위치와 빌드 방법)

---

## 10. 신규 보드 개발 표준 절차

> 원본 계획 문서: `GPI-8510_신규PCB_7일_개발절차서.pdf` (프로젝트 루트, 2026-07-15 작성).
> 아래 프롬프트 세트는 그 문서의 5장 내용에, 실제 Phase 2(app/ 계층 추출) 작업에서 검증된
> 노하우를 더한 것이다. 새 보드가 나올 때마다 이 순서로 진행한다.

### 10.1 표준 프롬프트 세트 (5단계)

| 단계 | 프롬프트 |
|------|----------|
| ① 핀맵 | "회로도 신호명 기준으로 `board_신규.h`의 핀 `#define`을 새 보드에 맞게 수정해줘. 기존 대비 바뀐 핀만 표로 보여줘." |
| ② 분석 | "main.c의 XXXX~YYYY 라인 함수를 분석하고 어떤 하드웨어를 건드리는지 설명해줘." |
| ③ 드라이버 | "CS5555 비트뱅잉 읽기 코드를 driver/adc_cs5555.c로 추출해줘. 채널을 파라미터로 받는 구조로." |
| ④ 앱 | "신규 보드의 추가 요구사항(예: 센서 1개 추가)만 app/ 계층에 신규 작성해줘. 기존 태스크 패턴을 따라줘." |
| ⑤ 검증 | "이 수정이 StdPeriph API와 C89 문법을 준수하고 워치독 경로를 유지하는지 검토해줘." |

### 10.2 app/ 계층 함수 추출 시 실전 노하우 (Phase 2에서 검증됨)

**핵심 위험 — CP949 파일을 Edit 도구로 직접 건드리면 안 된다.**
main.c 등 한글 주석이 섞인 파일을 Claude Code의 일반 Edit/Write 도구로 수정하면, 도구가
파일 전체를 UTF-8로 재인코딩하면서 한글 주석이 깨질 수 있다(실제로 이 세션에서 key.c
한 곳이 이렇게 손상되어 복구하지 못한 전례가 있음). 대신 다음 방식을 쓴다:

1. **Python 스크립트 파일을 먼저 만들고 실행한다** (`python3 -c "..."` 인라인 실행 금지 —
   따옴표가 셸 인용부호와 충돌해서 깨진 전례가 있음).
2. 파일을 `open(path, 'rb').read().decode('latin1')`로 읽는다 — latin1은 모든 바이트값을
   1:1로 왕복 변환하므로 CP949 내용을 건드리지 않고 안전하게 텍스트로 다룰 수 있다.
3. 옮길 함수 본문은 **정확한 문자열 매칭 후 개수(count)가 1인지 검증**하고서 잘라낸다.
   `main.c의 서로 다른 함수 경계를 착각해 다른 함수(예: 584줄짜리 Agent_KeyProcess_Task)까지
   통째로 삭제된 사고`가 있었다 — 함수 끝 경계는 반드시 실제 텍스트("return XXX;\r\n}")로
   특정하고, "다음 주석 블록 앞의 마지막 `}`" 같은 간접적인 방법은 쓰지 않는다.
4. 새 파일에 넣는 **주석에는 절대 `/* ... */`를 리터럴로 쓰지 않는다** — 중첩되어 주석이
   조기 종료되고 이후 코드가 전부 깨진다("block comment"처럼 풀어서 쓸 것).
5. `text.encode('latin1')`로 다시 쓴 뒤, 반드시 **① `cp949`로 디코딩 가능한지 ② CRLF만
   있는지(bare LF 없는지)**를 검증하고 나서 다음 단계로 넘어간다.
6. 새로 만드는 `app/*.c`/`*.h` 파일 자체는 **영문 주석으로 새로 작성**하고(헤더 자체 방침과
   동일), 원본에서 그대로 가져오는 함수 본문의 한글 주석만 위 방식으로 보존한다.

**의존성(전역변수/함수) 추적 방법:**
- 옮기려는 함수가 쓰는 모든 식별자를 `grep -rn "\b식별자\b" src/*.h`로 먼저 훑어서 이미
  `extern` 선언된 헤더가 있는지 확인한다.
- 헤더에 없으면 `grep`으로 실제 정의(main.c 등)를 찾아 정확한 타입으로 새 파일에
  `extern` 선언을 추가한다. **타입을 반드시 원본과 일치시킬 것** — 예: `s32`는 ARMCC5의
  `stdint.h` 체계에서 `int`이지 `long`이 아니다 (같은 32비트라도 C에서는 재선언 충돌 발생).
- **같은 변수가 여러 헤더에 서로 다른 타입으로 선언된 경우가 있었다** — `set.h`와
  `var.h`가 `gnlDailyTotal`/`gnlPreDailyTotal`을 다른 부호로 선언해 동시 include 시
  컴파일 에러가 났던 문제. **2026-07 정리 완료**: 조사 결과 실제 정의는 var.c 한 곳뿐
  이었고(set.c 쪽은 블록 주석 안의 죽은 텍스트), set.h는 아무도 include하지 않는 죽은
  헤더였다. set.h의 선언을 var.h와 일치하도록 수정했고, var.c의 `gnlDailyTotal2`/
  `gnlPreDailyTotal2` 정의도 var.h 선언(signed)과 일치시켰다. 교훈: grep으로 "이중
  정의"처럼 보여도 블록 주석/죽은 헤더일 수 있으니 반드시 원문을 열어 확인할 것.
- 매크로(`#define`)는 여러 파일에 중복 정의해도 문제없다(변수와 다름) — 필요하면 새 파일에
  그대로 복사해도 된다.

**이 코드베이스 특유의 죽은 코드 패턴:**
- `#ifdef dpddpp`, `#ifdef dpdpd`, `#ifdef dpfdpfdp` 처럼 **프로젝트 어디에도 정의되지
  않은 임의의 매크로**로 감싸서 옛날 버전 함수를 비활성화해둔 곳이 여러 군데 있다
  (`Agent_DischargeInput_Task`, `Agent_BaichulSend_Task`, `tcpip_send_form_zigbee_data`
  등). 같은 이름의 함수가 두 번 나오면, `grep -rn "define <매크로명>"`으로 실제 정의
  여부를 먼저 확인해서 어느 쪽이 진짜 동작하는 코드인지 가려낼 것 — 첫 번째로 나오는
  게 항상 진짜라는 보장은 없다.
- `Agent_KeyProcess_Task()`는 선언·정의만 있고 **어디서도 호출되지 않는 죽은 함수**다.
  실제 키 입력 처리는 main.c 안의 인라인 `switch(KeyCode)` 블록(여러 곳)과
  `goto start_func`/`goto stop_func` 조합으로 구현되어 있다. `app/`으로 옮길 함수를
  고를 때 "선언은 있는데 호출부가 진짜 있는지" 먼저 `grep`으로 확인할 것.

**빌드/검증 순서:** 함수 하나 이동 → 파일 저장 → CP949/CRLF 무결성 자체 검증(스크립트) →
사용자에게 Keil 빌드 요청 → 성공하면 다음 함수로. 절대 여러 함수를 한 번에 옮기지 않는다.
