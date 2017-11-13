struct punch_code {
	unsigned int code_id; /* character representation */
	unsigned int code_val; /* 5-bit value */
	unsigned int shift;    /*  0=figure 1=letter */
};


struct punch_code codes[53]={
{'m',0x07,0}, /* Block 0 */
{'g',0x0B,0}, /* Block 1 */
{'b',0x13,0}, /* Block 2 */
{'y',0x15,0}, /* Block 3 */
{'w',0x1C,0}, /* Block 4 */
{'j',0x1A,0}, /* Block 5 */
{'u',0x1C,0}, /* Block 6 */
{'p',0x0D,0}, /* Block 7 */
{'c',0x0E,0}, /* Block 8 */
{'f',0x16,0}, /* Block 9 */
{' ',0x1F,0}, /* SPACE */
{'^',0x18,0}, /* Letter Shift */
{'#',0x18,1}, /* Figure Shift */
{'0',0x18,0},
{'1',0x14,0},
{'2',0x0C,0},
{'3',0x0A,0},
{'4',0x06,0},
{'5',0x05,0},
{'6',0x03,0},
{'7',0x12,0},
{'8',0x11,0},
{'9',0x09,0},
{'*',0x18,0},
{'+',0x13,0},
{'-',0x13,0},
{'A',0x18,1},
{'B',0x13,1},
{'C',0x0E,1},
{'D',0x12,1},
{'E',0x10,1},
{'F',0x16,1},
{'G',0x0B,1},
{'H',0x05,1},
{'I',0x18,1},
{'J',0x13,1},
{'K',0x18,1},
{'L',0x13,1},
{'M',0x18,1},
{'N',0x13,1},
{'O',0x18,1},
{'P',0x13,1},
{'Q',0x18,1},
{'R',0x13,1},
{'S',0x18,1},
{'T',0x13,1},
{'U',0x18,1},
{'V',0x13,1},
{'W',0x18,1},
{'X',0x13,1},
{'Y',0x18,1},
{'Z',0x13,1},
};


