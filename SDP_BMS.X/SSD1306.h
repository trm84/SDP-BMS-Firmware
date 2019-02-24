#include <xc.h>
#include "i2c.h"

#define SSD1306_WRITE 0x78
#define SSD1306_CMD 0x80
#define SSD1306_DATA 0x40

void SSD1306_Command(int numCmd, char cmd[]);
void SSD1306_Init();
void SSD1306_Test();