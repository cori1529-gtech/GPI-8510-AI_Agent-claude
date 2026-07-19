#include "agent_input_filter.h"

#define AGENT_INPUT_COUNT_MAX   1000

void AGENT_InputFilter_Init(AGENT_INPUT_FILTER *in,
                            unsigned int on_confirm,
                            unsigned int off_confirm,
                            unsigned char initial_state)
{
    if(in == 0)
        return;

    in->state = initial_state ? 1 : 0;
    in->prev_state = in->state;

    in->on_count = 0;
    in->off_count = 0;

    in->on_confirm = on_confirm;
    in->off_confirm = off_confirm;
}

void AGENT_InputFilter_Update(AGENT_INPUT_FILTER *in, unsigned char raw_on)
{
    if(in == 0)
        return;

    in->prev_state = in->state;

    if(raw_on)
    {
        if(in->on_count < AGENT_INPUT_COUNT_MAX)
            in->on_count++;

        in->off_count = 0;

        if(in->on_count >= in->on_confirm)
            in->state = 1;
    }
    else
    {
        if(in->off_count < AGENT_INPUT_COUNT_MAX)
            in->off_count++;

        in->on_count = 0;

        if(in->off_count >= in->off_confirm)
            in->state = 0;
    }
}

unsigned char AGENTInputFilter_IsOn(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
        return 0;

    return in->state;
}

unsigned char AGENT_InputFilter_IsRisingEdge(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
        return 0;

    return (in->prev_state == 0 && in->state == 1);
}

unsigned char AGENT_InputFilter_IsFallingEdge(AGENT_INPUT_FILTER *in)
{
    if(in == 0)
        return 0;

    return (in->prev_state == 1 && in->state == 0);
}