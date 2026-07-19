#ifndef __INPUT_FILTER_H__
#define __INPUT_FILTER_H__

typedef struct
{
    unsigned char state;
    unsigned char prev_state;

    unsigned int on_count;
    unsigned int off_count;

    unsigned int on_confirm;
    unsigned int off_confirm;

} AGENT_INPUT_FILTER;

void AGENT_InputFilter_Init(AGENT_INPUT_FILTER *in,
                            unsigned int on_confirm,
                            unsigned int off_confirm,
                            unsigned char initial_state);

void AGENT_InputFilter_Update(AGENT_INPUT_FILTER *in, unsigned char raw_on);

unsigned char AGENT_InputFilter_IsOn(AGENT_INPUT_FILTER *in);
unsigned char AGENT_InputFilter_IsRisingEdge(AGENT_INPUT_FILTER *in);
unsigned char AGENT_InputFilter_IsFallingEdge(AGENT_INPUT_FILTER *in);

#endif