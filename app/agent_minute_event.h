#ifndef __AGENT_MINUTE_EVENT_H
#define __AGENT_MINUTE_EVENT_H

/* Phase 2 pilot extraction - see prj/CLAUDE.md section 6.
   Not yet included from main.c: main.c already has its own local
   prototype (kept as-is to avoid touching more than necessary for
   this pilot). Available for future callers. */
void Agent_MinuteEvent_Task(void);

#endif /* __AGENT_MINUTE_EVENT_H */
