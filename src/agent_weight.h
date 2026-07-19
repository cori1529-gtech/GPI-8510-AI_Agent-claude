#ifndef __AGENT_WEIGHT_H__
#define __AGENT_WEIGHT_H__

/******************************************************************************
 * Project : GTECH Embedded Framework
 * Module  : agent_weight
 * Version : 1.00-monitor
 * Target  : STM32F Series / Keil MDK uVision / C
 *
 * Description:
 *   기존 프로젝트의 무게 계산 로직을 즉시 대체하지 않고,
 *   gxlWeight, gxlWeight_2 같은 기존 최종 무게값을 감시/기록/진단하기 위한
 *   1차 표준 모듈이다.
 *
 * Design Policy:
 *   1. 기존 Silo_1_Getweight(), Silo_2_Getweight() 계산 로직은 건드리지 않는다.
 *   2. 기존 코드에서 계산된 무게값만 입력받아 상태를 추적한다.
 *   3. 급격한 변화, 부호 변화, 로드셀 에러, 증가/감소 방향을 판단한다.
 *   4. 추후 agent_discharge 모듈에서 배출량 계산 표준화의 기반으로 사용한다.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* 무게 변화 방향 */
#define AGENT_WEIGHT_DIR_NONE       0u
#define AGENT_WEIGHT_DIR_DECREASE   1u
#define AGENT_WEIGHT_DIR_INCREASE   2u

/* 이벤트 플래그 */
#define AGENT_WEIGHT_EVENT_NONE           0x0000u
#define AGENT_WEIGHT_EVENT_FIRST_UPDATE   0x0001u
#define AGENT_WEIGHT_EVENT_DECREASE       0x0002u
#define AGENT_WEIGHT_EVENT_INCREASE       0x0004u
#define AGENT_WEIGHT_EVENT_SIGN_CHANGED   0x0008u
#define AGENT_WEIGHT_EVENT_LOADCELL_ERR   0x0010u
#define AGENT_WEIGHT_EVENT_JUMP_DETECTED  0x0020u
#define AGENT_WEIGHT_EVENT_STABLE         0x0040u
#define AGENT_WEIGHT_EVENT_UNSTABLE       0x0080u

/*
 * AGENT_WEIGHT
 *
 * 기존 프로젝트에서 생성된 최종 무게값을 감시하기 위한 상태 구조체.
 * Silo 1개당 1개의 구조체를 사용한다.
 */
typedef struct
{
    long current_weight;       /* 현재 무게값 */
    long previous_weight;      /* 직전 무게값 */
    long delta_weight;         /* 현재 - 직전 변화량 */
    long abs_delta_weight;     /* 변화량 절대값 */

    unsigned char sign;        /* 현재 부호: 기존 sign 또는 sign_2 */
    unsigned char previous_sign; /* 직전 부호 */

    unsigned char loadcell_error; /* 현재 로드셀 에러 상태 */
    unsigned char direction;      /* 증가/감소/변화없음 */

    unsigned char is_first_update; /* 최초 업데이트 여부 */
    unsigned char stable;          /* 안정 상태 판단 결과 */
    unsigned int stable_count;     /* 안정 지속 카운터 */
    unsigned int unstable_count;   /* 불안정 지속 카운터 */

    long stable_tolerance;         /* 이 값 이하 변화면 안정 후보 */
    unsigned int stable_confirm;   /* 안정 인정 반복 횟수 */
    long jump_limit;               /* 이 값 초과 변화면 급변/노이즈 후보 */

    unsigned long update_count;    /* 업데이트 누적 횟수 */
    unsigned int event;            /* 최신 이벤트 플래그 */

} AGENT_WEIGHT;

/******************************************************************************
 * Function : AGENT_Weight_Init
 *
 * Description:
 *   무게 감시 구조체를 초기화한다.
 *
 * Parameters:
 *   w              : AGENT_WEIGHT 구조체 주소
 *   stable_tol     : 안정 판단 허용 변화량. 예: 1~3 kg
 *   stable_confirm : 안정 인정 반복 횟수. 예: 3~5회
 *   jump_limit     : 급격한 변화 판단 기준. 예: 100 kg
 ******************************************************************************/
void AGENT_Weight_Init(AGENT_WEIGHT *w,
                       long stable_tol,
                       unsigned int stable_confirm,
                       long jump_limit);

/******************************************************************************
 * Function : AGENT_Weight_Update
 *
 * Description:
 *   기존 코드에서 계산된 현재 무게값을 agent_weight에 반영한다.
 *   이 함수는 무게를 새로 계산하지 않는다.
 *
 * Parameters:
 *   w              : AGENT_WEIGHT 구조체 주소
 *   current_weight : 기존 코드에서 계산된 최종 무게값
 *   sign           : 기존 코드의 sign 또는 sign_2
 *   loadcell_error : 기존 코드의 로드셀 에러 상태
 *
 * Return:
 *   이벤트 플래그 반환
 ******************************************************************************/
unsigned int AGENT_Weight_Update(AGENT_WEIGHT *w,
                                 long current_weight,
                                 unsigned char sign,
                                 unsigned char loadcell_error);

long AGENT_Weight_GetCurrent(AGENT_WEIGHT *w);
long AGENT_Weight_GetPrevious(AGENT_WEIGHT *w);
long AGENT_Weight_GetDelta(AGENT_WEIGHT *w);
long AGENT_Weight_GetAbsDelta(AGENT_WEIGHT *w);
unsigned char AGENT_Weight_GetDirection(AGENT_WEIGHT *w);
unsigned char AGENT_Weight_IsStable(AGENT_WEIGHT *w);
unsigned char AGENT_Weight_IsLoadcellError(AGENT_WEIGHT *w);
unsigned int AGENT_Weight_GetEvent(AGENT_WEIGHT *w);

#ifdef __cplusplus
}
#endif

#endif /* __AGENT_WEIGHT_H__ */
