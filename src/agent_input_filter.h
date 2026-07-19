#ifndef __AGENT_INPUT_FILTER_H__
#define __AGENT_INPUT_FILTER_H__

/******************************************************************************
 *
 *  Project : GTECH Embedded Framework
 *  Module  : agent_input_filter
 *  File    : agent_input_filter.h
 *  Version : 1.00
 *  Target  : STM32F Series / Keil MDK uVision
 *
 *  Description :
 *      Digital input noise filter and discharge input event processor.
 *
 *      This module standardizes ON/OFF debounce processing for industrial
 *      equipment inputs such as FIRST_INPUT, SECOND_INPUT, door sensors,
 *      limit switches, proximity sensors, and relay feedback signals.
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 입력 이벤트 결과값
 */
#define AGENT_INPUT_EVENT_NONE      0u  /* 변화 없음 */
#define AGENT_INPUT_EVENT_START     1u  /* OFF -> ON 확정 */
#define AGENT_INPUT_EVENT_STOP      2u  /* ON -> OFF 확정 */

/*
 * 입력 필터 구조체
 *
 * state        : 현재 필터링 완료된 입력 상태
 * prev_state   : 이전 입력 상태. Rising/Falling Edge 판단용
 * on_count     : 연속 ON 감지 카운터
 * off_count    : 연속 OFF 감지 카운터
 * on_confirm   : ON으로 인정하기 위한 연속 ON 횟수
 * off_confirm  : OFF로 인정하기 위한 연속 OFF 횟수
 */
typedef struct
{
    unsigned char state;
    unsigned char prev_state;

    unsigned int on_count;
    unsigned int off_count;

    unsigned int on_confirm;
    unsigned int off_confirm;

} AGENT_INPUT_FILTER;

/*
 * 입력 필터 초기화 함수
 */
void AGENT_InputFilter_Init(AGENT_INPUT_FILTER *in,
                            unsigned int on_confirm,
                            unsigned int off_confirm,
                            unsigned char initial_state);

/*
 * 입력 필터 업데이트 함수
 */
void AGENT_InputFilter_Update(AGENT_INPUT_FILTER *in,
                              unsigned char raw_on);

/*
 * 현재 필터링된 입력 상태 확인
 */
unsigned char AGENT_InputFilter_IsOn(AGENT_INPUT_FILTER *in);

/*
 * OFF -> ON 변화 감지
 */
unsigned char AGENT_InputFilter_IsRisingEdge(AGENT_INPUT_FILTER *in);

/*
 * ON -> OFF 변화 감지
 */
unsigned char AGENT_InputFilter_IsFallingEdge(AGENT_INPUT_FILTER *in);

/*
 * 배출 입력 표준 처리 함수
 *
 * 입력 필터 처리 + Rising/Falling Edge + ek_flag 처리 + sign 저장을
 * 하나의 함수로 표준화한다.
 */
unsigned char AGENT_DischargeInput_Process(AGENT_INPUT_FILTER *input,
                                            unsigned char raw_input,
                                            unsigned char *ek_flag,
                                            unsigned char output_low_error,
                                            unsigned char output_high_error,
                                            unsigned char sign,
                                            unsigned char *start_sign_flag);

#ifdef __cplusplus
}
#endif

#endif /* __AGENT_INPUT_FILTER_H__ */
