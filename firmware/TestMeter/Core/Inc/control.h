#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

void Control_Init(void);
void Control_Loop(void);

void TAC_CounterHandler(void);
void TAC_IrqHandler(void);
void TAC_UpdateHandler(void);

#endif
