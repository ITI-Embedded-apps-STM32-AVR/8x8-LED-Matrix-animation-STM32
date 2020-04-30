#include "STD_TYPES.h"
#include "BIT_MAN.h"
#include "LED_Marix_Config.h"
#include "LED_Matrix.h"
#include "DIO_interface.h"
#include "Delay_interface.h"

typedef struct
{
	u8* pinColor;
	u8* portColor;

	u8* pinComm;
	u8* portComm;

	u8 ledPinsCount;
	u8 colCount;
	u8 rowCount;

	u8 ledActiveState;
} LED_Matrix_t;

LED_Matrix_t mtx[LED_MATRIX_COUNT];

void LED_Matrix_Init(u8 idx, const void* pinColor, const void* portColor, const u8* pinComm, const u8* portComm, u8 ledPinsCount, u8 colCount, u8 rowCount, u8 activeState)
{
	mtx[idx].pinColor       = (u8*)pinColor;
	mtx[idx].portColor      = (u8*)portColor;
	mtx[idx].pinComm        = (u8*)pinComm;
	mtx[idx].portComm       = (u8*)portComm;
	mtx[idx].ledPinsCount   = ledPinsCount;
	mtx[idx].colCount       = colCount;
	mtx[idx].rowCount       = rowCount;
	mtx[idx].ledActiveState = activeState;

	for (u8 r = 0; r < mtx[idx].rowCount; r++)
	{
		DIO_setPinMode(mtx[idx].portComm[r], mtx[idx].pinComm[r], DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
		/* if LED operates on HIGH, then Comm must be set to LOW and vice versa */
		DIO_setPinVal(mtx[idx].portComm[r], mtx[idx].pinComm[r], mtx[idx].ledActiveState);
	}

	for (u8 c = 0; c < mtx[idx].colCount; c++)
	{
		for (u8 p = 0; p < mtx[idx].ledPinsCount; p++)
		{
			u8 pinStep = mtx[idx].ledPinsCount * c;

			DIO_setPinMode(mtx[idx].portColor[pinStep + p], mtx[idx].pinColor[pinStep + p], DIO_PIN_MODE_OUT_PUSH_PULL_2MHZ);
			DIO_setPinVal(mtx[idx].portColor[pinStep + p], mtx[idx].pinColor[pinStep + p], !mtx[idx].ledActiveState);
		}
	}
}

void LED_Matrix_DrawPatt(u8 idx, const u8* patt)
{
	for (u8 r = 0; r < mtx[idx].rowCount; r++) /* foreach row */
	{
		for (u8 c = 0; c < mtx[idx].colCount; c++) /* foreach column in the row */
		{
			/* treat pattern array as an X x y matrix, where X (step size) = column count, Y (offset in the step) = r */
			u8 pattSelect = patt[mtx[idx].colCount * r + c];
			/* treat led pin & port as an X x y matrix, where X (step size) = pin count of each led, Y (offset in the step) = r */
			u8 pinStep = mtx[idx].ledPinsCount * c;

			/* turn off each pin of the current LED */
			for (u8 p = 0; p < mtx[idx].ledPinsCount; p++)
			{
				DIO_setPinVal(mtx[idx].portColor[pinStep + p], mtx[idx].pinColor[pinStep + p], !mtx[idx].ledActiveState);
			}

			/* if pattern = turn on all pins */
			if (pattSelect == (mtx[idx].ledPinsCount + 1))
			{
				for (u8 p = 0; p < mtx[idx].ledPinsCount; p++)
				{
					DIO_setPinVal(mtx[idx].portColor[pinStep + p], mtx[idx].pinColor[pinStep + p], mtx[idx].ledActiveState);
				}
			}
			/* if pattern is anything inside the range */
			else if ((pattSelect != 0) && (pattSelect <= mtx[idx].ledPinsCount))
			{
				DIO_setPinVal(mtx[idx].portColor[pinStep + pattSelect - 1], mtx[idx].pinColor[pinStep + pattSelect - 1], mtx[idx].ledActiveState);
			}
		}

		/* activate the Comm pin */
		/* if LED operates on HIGH, then Comm must be set to LOW and vice versa */
		DIO_setPinVal(mtx[idx].portComm[r], mtx[idx].pinComm[r], !mtx[idx].ledActiveState);

		/* wait for some time so that the image is clear */
		delay_ms(1);

		/* deactivate the Comm pin */
		DIO_setPinVal(mtx[idx].portComm[r], mtx[idx].pinComm[r], mtx[idx].ledActiveState);
	}
}

