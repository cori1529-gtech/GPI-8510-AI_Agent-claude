/******************************************************************************
 *
 *  Project : GTECH Embedded Framework
 *  Module  : agent_input_filter
 *  File    : agent_input_filter.c
 *  Version : 1.00
 *  Target  : STM32F Series / Keil MDK uVision
 *
 *  Description :
 *      Digital input noise filter and discharge input event processor.
 *
 *      This module removes short input noise and converts raw GPIO input
 *      into stable ON/OFF events.
 *
 ******************************************************************************/

#include "agent_input_filter.h"

/*
 * 카운터 최대값
 *
 * 입력이 장시간 ON 또는 OFF 상태로 유지될 때 카운터가 계속 증가하여
 * overflow 되는 것을 방지하기 위한 제한값이다.
 */
#define AGENT_INPUT_COUNT_MAX   1000u

/******************************************************************************
 * Function : AGENT_InputFilter_Init
 *
 * Description :
 *      입력 필터 구조체를 초기화한다.
 *
 * Parameters :
 *      in            : 초기화할 입력 필터 구조체 포인터
 *      on_confirm    : ON으로 확정할 연속 ON 횟수
 *      off_confirm   : OFF로 확정할 연속 OFF 횟수
 *      initial_state : 전원 투입 시 초기 상태. 0=OFF, 1=ON
 *
 * Return :
 *      None
 ******************************************************************************/
void AGENT_InputFilter_Init(AGENT_INPUT_FILTER *in,
                            unsigned int on_confirm,
                            unsigned int off_confirm,
                            unsigned char initial_state)
{
    if(in == 0)
    {
        return;
    }

    in->state = initial_state ? 1u : 0u;
    in->prev_state = in->state;

    in->on_count = 0u;
    in->off_count = 0u;

    /*
     * confirm 값이 0이면 입력이 즉시 확정되어 예기치 않은 동작을 할 수 있다.
     * 최소 1 이상으로 보정한다.
     */
    in->on_confirm = (on_confirm == 0u) ? 1u : on_confirm;
    in->off_confirm = (off_confirm == 0u) ? 1u : off_confirm;
}

/******************************************************************************
 * Function : AGENT_InputFilter_Update
 *
 * Description :
 *      실제 입력값(raw_on)을 받아 필터링된 입력 상태를 갱신한다.
 *
 *      raw_on이 1이면 ON 입력으로 판단한다.
 *      raw_on이 0이면 OFF 입력으로 판단한다.
 *
 *      ON/OFF가 각각 설정된 confirm 횟수만큼 연속 유지되어야
 *      최종 state가 변경된다.
 *
 * Parameters :
 *      in     : 입력 필터 구조체 포인터
 *      raw_on : 실제 입력값. 0=OFF, 1=ON
 *
 * Return :
 *      None
 ******************************************************************************/
void AGENT_InputFilter_Update(AGENT_INPUT_FILTER *in,
                              unsigned char raw_on)
{
    if(in == 0)
    {
        return;
    }

    /* Edge 판단을 위해 이전 상태를 저장한다. */
    in->prev_state = in->state;

    if(raw_on)
    {
        /* ON 입력이 들어오면 ON 카운터 증가 */
        if(in->on_count < AGENT_INPUT_COUNT_MAX)
        {
            in->on_count++;
        }

        /* 반대 방향 카운터는 초기화 */
        in->off_count = 0u;

        /* 설정 횟수 이상 ON이 연속되면 최종 ON 확정 */
        if(in->on_count >= in->on_confirm)
        {
            in->state = 1u;
        }
    }
    else
    {
        /* OFF 입력이 들어오면 OFF 카운터 증가 */
        if(in->off_count < AGENT_INPUT_COUNT_MAX)
        {
            in->off_count++;
        }

        /* 반대 방향 카운터는 초기화 */
        in->on_count = 0u;

        /* 설정 횟수 이상 OFF가 연속되면 최종 OFF 확정 */
        if(in->off_count >= in->off_confirm)
        {
            in->state = 0u;
        }
    }
}

/******************************************************************************
 * Function : AGENT_InputFilter_IsOn
 *
 * Description :
 *      현재 필터링된 입력 상태가 ON인지 확인한다.
 *
 * Parameters :
 *      in : 입력 필터 구조체 포인터
 *
 * Return :
 *      0 : OFF
 *      1 : ON
 ******************************************************************************/
unsigned char AGENT_InputFilter_IsOn(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
    {
        return 0u;
    }

    return in->state;
}

/******************************************************************************
 * Function : AGENT_InputFilter_IsRisingEdge
 *
 * Description :
 *      입력 상태가 OFF에서 ON으로 바뀐 순간인지 확인한다.
 *
 * Parameters :
 *      in : 입력 필터 구조체 포인터
 *
 * Return :
 *      0 : Rising Edge 아님
 *      1 : Rising Edge 발생
 ******************************************************************************/
unsigned char AGENT_InputFilter_IsRisingEdge(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
    {
        return 0u;
    }

    return (unsigned char)((in->prev_state == 0u) && (in->state == 1u));
}

/******************************************************************************
 * Function : AGENT_InputFilter_IsFallingEdge
 *
 * Description :
 *      입력 상태가 ON에서 OFF로 바뀐 순간인지 확인한다.
 *
 * Parameters :
 *      in : 입력 필터 구조체 포인터
 *
 * Return :
 *      0 : Falling Edge 아님
 *      1 : Falling Edge 발생
 ******************************************************************************/
unsigned char AGENT_InputFilter_IsFallingEdge(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
    {
        return 0u;
    }

    return (unsigned char)((in->prev_state == 1u) && (in->state == 0u));
}

/******************************************************************************
 * Function : AGENT_DischargeInput_Process
 *
 * Description :
 *      배출 장비 입력 신호를 표준 처리한다.
 *
 *      기존 main.c에 있던 다음 처리를 하나로 묶은 함수이다.
 *
 *      1. 실제 입력값을 필터링한다.
 *      2. OFF -> ON 확정 시 START 이벤트를 발생시킨다.
 *      3. START 시점에 에러가 없으면 ek_flag를 1로 설정한다.
 *      4. START 시점의 sign 값을 start_sign_flag에 저장한다.
 *      5. ON -> OFF 확정 시 STOP 이벤트를 발생시킨다.
 *      6. STOP 시점에 ek_flag를 0으로 설정한다.
 *
 * Parameters :
 *      input             : 입력 필터 구조체 포인터
 *      raw_input         : 실제 GPIO 입력값. 0=OFF, 1=ON
 *      ek_flag           : 기존 배출 동작 플래그 포인터
 *      output_low_error  : 하한 배출 에러 상태
 *      output_high_error : 상한 배출 에러 상태
 *      sign              : 현재 무게 부호
 *      start_sign_flag   : 배출 시작 시점의 부호 저장 변수 포인터
 *
 * Return :
 *      AGENT_INPUT_EVENT_NONE  : 변화 없음
 *      AGENT_INPUT_EVENT_START : 입력 ON 확정
 *      AGENT_INPUT_EVENT_STOP  : 입력 OFF 확정
 ******************************************************************************/
unsigned char AGENT_DischargeInput_Process(AGENT_INPUT_FILTER *input,
                                            unsigned char raw_input,
                                            unsigned char *ek_flag,
                                            unsigned char output_low_error,
                                            unsigned char output_high_error,
                                            unsigned char sign,
                                            unsigned char *start_sign_flag)
{
    /*
     * 산업용 장비에서는 잘못된 포인터 접근이 치명적인 오동작으로 이어질 수 있다.
     * 따라서 모든 포인터는 사용 전에 방어 확인한다.
     */
    if(input == 0)
    {
        return AGENT_INPUT_EVENT_NONE;
    }

    if(ek_flag == 0)
    {
        return AGENT_INPUT_EVENT_NONE;
    }

    if(start_sign_flag == 0)
    {
        return AGENT_INPUT_EVENT_NONE;
    }

    /* 1. 실제 입력값을 필터에 반영한다. */
    AGENT_InputFilter_Update(input, raw_input);

    /*
     * 2. OFF -> ON으로 확정된 순간만 START 처리한다.
     *    입력이 계속 ON 상태여도 반복 실행되지 않는다.
     */
    if(AGENT_InputFilter_IsRisingEdge(input))
    {
        if(!(*ek_flag))
        {
            /* 에러가 없을 때만 배출 시작을 허용한다. */
            if(!output_low_error && !output_high_error)
            {
                /* 배출 시작 시점의 무게 부호를 저장한다. */
                if(sign)
                {
                    *start_sign_flag = 1u;
                }
                else
                {
                    *start_sign_flag = 0u;
                }

                /* 기존 코드의 ek_flag = 1 역할 */
                *ek_flag = 1u;

                return AGENT_INPUT_EVENT_START;
            }
        }
    }

    /*
     * 3. ON -> OFF로 확정된 순간만 STOP 처리한다.
     */
    if(AGENT_InputFilter_IsFallingEdge(input))
    {
        if(*ek_flag)
        {
            /* 기존 코드의 ek_flag = 0 역할 */
            *ek_flag = 0u;

            return AGENT_INPUT_EVENT_STOP;
        }
    }

    return AGENT_INPUT_EVENT_NONE;
}
