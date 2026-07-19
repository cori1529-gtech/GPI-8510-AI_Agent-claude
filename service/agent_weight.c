#include "agent_weight.h"

/******************************************************************************
 * Internal Function : AGENT_Weight_AbsLong
 *
 * Description:
 *   long 타입 절대값 계산 함수.
 *   Keil/임베디드 환경에서 표준 라이브러리 의존성을 줄이기 위해 내부 함수 사용.
 ******************************************************************************/
static long AGENT_Weight_AbsLong(long value)
{
    if(value < 0)
        return -value;

    return value;
}

/******************************************************************************
 * Function : AGENT_Weight_Init
 ******************************************************************************/
void AGENT_Weight_Init(AGENT_WEIGHT *w,
                       long stable_tol,
                       unsigned int stable_confirm,
                       long jump_limit)
{
    if(w == 0)
        return;

    w->current_weight = 0;
    w->previous_weight = 0;
    w->delta_weight = 0;
    w->abs_delta_weight = 0;

    w->sign = 0;
    w->previous_sign = 0;

    w->loadcell_error = 0;
    w->direction = AGENT_WEIGHT_DIR_NONE;

    w->is_first_update = 1;
    w->stable = 0;
    w->stable_count = 0;
    w->unstable_count = 0;

    w->stable_tolerance = stable_tol;
    w->stable_confirm = stable_confirm;
    w->jump_limit = jump_limit;

    w->update_count = 0;
    w->event = AGENT_WEIGHT_EVENT_NONE;
}

/******************************************************************************
 * Function : AGENT_Weight_Update
 ******************************************************************************/
unsigned int AGENT_Weight_Update(AGENT_WEIGHT *w,
                                 long current_weight,
                                 unsigned char sign,
                                 unsigned char loadcell_error)
{
    if(w == 0)
        return AGENT_WEIGHT_EVENT_NONE;

    w->event = AGENT_WEIGHT_EVENT_NONE;
    w->update_count++;

    /* 최초 1회 업데이트는 이전값 비교 기준을 만들기 위한 단계 */
    if(w->is_first_update)
    {
        w->current_weight = current_weight;
        w->previous_weight = current_weight;
        w->delta_weight = 0;
        w->abs_delta_weight = 0;

        w->sign = sign ? 1 : 0;
        w->previous_sign = w->sign;
        w->loadcell_error = loadcell_error ? 1 : 0;
        w->direction = AGENT_WEIGHT_DIR_NONE;

        w->stable_count = 0;
        w->unstable_count = 0;
        w->stable = 0;

        w->is_first_update = 0;
        w->event |= AGENT_WEIGHT_EVENT_FIRST_UPDATE;

        if(w->loadcell_error)
            w->event |= AGENT_WEIGHT_EVENT_LOADCELL_ERR;

        return w->event;
    }

    /* 직전값 보관 */
    w->previous_weight = w->current_weight;
    w->previous_sign = w->sign;

    /* 현재값 반영 */
    w->current_weight = current_weight;
    w->sign = sign ? 1 : 0;
    w->loadcell_error = loadcell_error ? 1 : 0;

    /* 변화량 계산 */
    w->delta_weight = w->current_weight - w->previous_weight;
    w->abs_delta_weight = AGENT_Weight_AbsLong(w->delta_weight);

    /* 방향 판단 */
    if(w->delta_weight < 0)
    {
        w->direction = AGENT_WEIGHT_DIR_DECREASE;
        w->event |= AGENT_WEIGHT_EVENT_DECREASE;
    }
    else if(w->delta_weight > 0)
    {
        w->direction = AGENT_WEIGHT_DIR_INCREASE;
        w->event |= AGENT_WEIGHT_EVENT_INCREASE;
    }
    else
    {
        w->direction = AGENT_WEIGHT_DIR_NONE;
    }

    /* 부호 변화 감시 */
    if(w->sign != w->previous_sign)
    {
        w->event |= AGENT_WEIGHT_EVENT_SIGN_CHANGED;
    }

    /* 로드셀 에러 감시 */
    if(w->loadcell_error)
    {
        w->event |= AGENT_WEIGHT_EVENT_LOADCELL_ERR;
    }

    /* 급격한 변화 감시: 배출량 계산에서는 이런 값은 노이즈 후보로 봐야 함 */
    if((w->jump_limit > 0) && (w->abs_delta_weight > w->jump_limit))
    {
        w->event |= AGENT_WEIGHT_EVENT_JUMP_DETECTED;
    }

    /* 안정/불안정 판단 */
    if(w->abs_delta_weight <= w->stable_tolerance)
    {
        if(w->stable_count < 60000u)
            w->stable_count++;

        w->unstable_count = 0;

        if(w->stable_count >= w->stable_confirm)
        {
            w->stable = 1;
            w->event |= AGENT_WEIGHT_EVENT_STABLE;
        }
    }
    else
    {
        if(w->unstable_count < 60000u)
            w->unstable_count++;

        w->stable_count = 0;
        w->stable = 0;
        w->event |= AGENT_WEIGHT_EVENT_UNSTABLE;
    }

    return w->event;
}

long AGENT_Weight_GetCurrent(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->current_weight;
}

long AGENT_Weight_GetPrevious(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->previous_weight;
}

long AGENT_Weight_GetDelta(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->delta_weight;
}

long AGENT_Weight_GetAbsDelta(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->abs_delta_weight;
}

unsigned char AGENT_Weight_GetDirection(AGENT_WEIGHT *w)
{
    if(w == 0)
        return AGENT_WEIGHT_DIR_NONE;

    return w->direction;
}

unsigned char AGENT_Weight_IsStable(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->stable;
}

unsigned char AGENT_Weight_IsLoadcellError(AGENT_WEIGHT *w)
{
    if(w == 0)
        return 0;

    return w->loadcell_error;
}

unsigned int AGENT_Weight_GetEvent(AGENT_WEIGHT *w)
{
    if(w == 0)
        return AGENT_WEIGHT_EVENT_NONE;

    return w->event;
}
