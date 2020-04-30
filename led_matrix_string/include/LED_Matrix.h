#define LED_MATRIX_ACTIVE_STATE_HIGH 1
#define LED_MATRIX_ACTIVE_STATE_LOW  0

void LED_Matrix_Init(u8 idx, const void* pinColor, const void* portColor, const u8* pinComm, const u8* portComm, u8 ledPinsCount, u8 colCount, u8 rowCount, u8 activeState);
void LED_Matrix_DrawPatt(u8 idx, const u8* patt);
