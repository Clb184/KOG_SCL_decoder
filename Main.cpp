#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#define MAXCHAR_KOG 10
#define SCL_BUFFER 50

bool sclmapUse = false;

unsigned int ins_namePos[0xff];
unsigned int ins_nameLength[0xff];
std::string ins_name[0xff];


struct texEntry
{
	int entryPoint[12];
	int num_tex;
};

struct SCLHeader
{
	//Cantidad de enemigos en niveles del SCL, no de los ataques de los jugadores.
	int Lvl1SCL;
	int Lvl2SCL;
	int Lvl3SCL;
	int Lvl4SCL;

	//Direcci?n del Inicializador de texturas.
	int TexInit;

	//Direcciones de los ataques de los jugadores.
	int Lv1Attack[MAXCHAR_KOG];
	int Lv2Attack[MAXCHAR_KOG];
	int BossAttack[MAXCHAR_KOG];
	int ComboAttack[MAXCHAR_KOG];

	//Direcciones de los retratos y sus scripts.
	int AnmLv1[MAXCHAR_KOG];
	int AnmLv2[MAXCHAR_KOG];
	int AnmBoss[MAXCHAR_KOG];
	int AnmWin[MAXCHAR_KOG];

	//Timelines
	int SCL_lv1[SCL_BUFFER];
	int SCL_lv2[SCL_BUFFER];
	int SCL_lv3[SCL_BUFFER];
	int SCL_lv4[SCL_BUFFER];

	//Direcciones de los retratos (?
	texEntry LTEntry[MAXCHAR_KOG];
};

char* buffer;
char* mapbuffer;
long lSize;
long mapSize;

SCLHeader header = {};

const char* kog_char(int i)
{
	switch (i)
	{
	case 0: return "VIVIT"; break;
	case 1: return "Milia"; break;
	case 2: return "Mei & Mai"; break;
	case 3: return "Gates"; break;
	case 4: return "Marie"; break;
	case 5: return "Erich"; break;
	case 6: return "Morgan"; break;
	case 7: return "Muse"; break;
	case 8: return "Yuuka"; break;
		//case 9: return ""; break;
	}
}

int lv1Atk_add[MAXCHAR_KOG], lv2Atk_add[MAXCHAR_KOG], BossAtk_add[MAXCHAR_KOG], ComboAtk_add[MAXCHAR_KOG];
int lv1Port_add[MAXCHAR_KOG], lv2Port_add[MAXCHAR_KOG], BossPort_add[MAXCHAR_KOG], WinPort_add[MAXCHAR_KOG];

inline int32_t convCharInt(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}


inline int16_t convCharInt(uint8_t c1, uint8_t c2) {
	return c2 << 8 | c1;
}

inline uint32_t convCharUInt(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

inline uint16_t convCharUInt(uint8_t c1, uint8_t c2) {
	return c2 << 8 | c1;
}

inline uint8_t uChar(uint8_t ch)
{
	return ch;
}
/*int convCharInt(char c1, char c2, char c3, char c4)
{
	char a[4];
	a[0] = c1;
	a[1] = c2;
	a[2] = c3;
	a[3] = c4;
	return *(int*)a;
}*/


void headerInfo()
{

	// Parte de escritura en la pantalla

	std::cout << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 1: " << header.Lvl1SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 2: " << header.Lvl2SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 3: " << header.Lvl3SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 4: " << header.Lvl4SCL << std::endl << std::endl;

	printf("Direccion de inicializacion de texturas: 0x%x\n\n", header.TexInit);

	const char* charaName = "";

	std::cout << "Direcciones de los Ataques de Nivel 1 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i < 9)
			printf("%s: 0x%x\n", charaName, header.Lv1Attack[i]);
		else if (i >= 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Ataques de Nivel 2 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i < 9)
			printf("%s: 0x%x\n", charaName, header.Lv2Attack[i]);
		else if (i >= 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Bossfights //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i < 9)
			printf("%s: 0x%x\n", charaName, header.BossAttack[i]);
		else if (i >= 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Combo Attacks //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i < 9)
			printf("%s: 0x%x\n", charaName, header.ComboAttack[i]);
		else if (i >= 9)
			std::cout << std::endl;
	}



	std::cout << "// Direcciones de retrato de ataque de Nivel 1 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmLv1[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de retrato de ataque de Nivel 2 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmLv2[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de retrato de Jefe //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmBoss[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direccion de retratos de victoria //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		charaName = kog_char(i);
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmWin[i]);
		if (i == 9)
			std::cout << std::endl;
	}



	std::cout << "// S C L   L v l   1 //" << std::endl;
	for (int i = 0; i < header.Lvl1SCL; i++)
	{
		printf("Oleada No. %d: 0x%x\n", i + 1, header.SCL_lv1[i]);
		if (i >= header.Lvl1SCL - 1)
			std::cout << std::endl;
	}
	std::cout << "// S C L   L v l   2 //" << std::endl;
	for (int i = 0; i < header.Lvl2SCL; i++)
	{
		printf("Oleada No. %d: 0x%x\n", i + 1, header.SCL_lv2[i]);
		if (i >= header.Lvl2SCL - 1)
			std::cout << std::endl;
	}
	std::cout << "// S C L   L v l   3 //" << std::endl;
	for (int i = 0; i < header.Lvl3SCL; i++)
	{
		printf("Oleada No. %d: 0x%x\n", i + 1, header.SCL_lv3[i]);
		if (i >= header.Lvl3SCL - 1)
			std::cout << std::endl;
	}
	std::cout << "// S C L   L v l   4 //" << std::endl;
	for (int i = 0; i < header.Lvl4SCL; i++)
	{
		printf("Oleada No. %d: 0x%x\n", i + 1, header.SCL_lv4[i]);
		if (i >= header.Lvl4SCL - 1)
			std::cout << std::endl;
	}

	std::cout << "// Texture entries //" << std::endl;
	for (int j = 0; j < MAXCHAR_KOG; j++)
	{
		charaName = kog_char(j);
		if (j < 9)
			std::cout << charaName << ": " << std::endl;
		for (int i = 0; i < header.LTEntry[j].num_tex; i++)
		{
			printf("Direccion Tex %d: 0x%x,\n", i + 1, header.LTEntry[j].entryPoint[i]);

			if (i >= header.LTEntry[j].num_tex - 1)
				std::cout << std::endl;
		}
		if (j < 9)
			std::cout << "Cantidad de direccion de texturas: " << header.LTEntry[j].num_tex << std::endl;

		std::cout << std::endl;
	}


}

void getHeaderInfo()
{

	int lv = 0x0;
	header.Lvl1SCL = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	lv += 0x4;
	header.Lvl2SCL = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	lv += 0x4;
	header.Lvl3SCL = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	lv += 0x4;
	header.Lvl4SCL = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);

	lv += 0x4;
	header.TexInit = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);



	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.Lv1Attack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		lv1Atk_add[i] = header.Lv1Attack[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.Lv2Attack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		lv2Atk_add[i] = header.Lv2Attack[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.BossAttack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		BossAtk_add[i] = header.BossAttack[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.ComboAttack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		ComboAtk_add[i] = header.ComboAttack[i];
	}



	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmLv1[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		lv1Port_add[i] = header.AnmLv1[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmLv2[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		lv2Port_add[i] = header.AnmLv2[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmBoss[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		BossPort_add[i] = header.AnmBoss[i];
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmWin[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		WinPort_add[i] = header.AnmWin[i];
	}



	for (int i = 0; i < SCL_BUFFER; i++)
	{
		lv += 0x4;
		header.SCL_lv1[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < SCL_BUFFER; i++)
	{
		lv += 0x4;
		header.SCL_lv2[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < SCL_BUFFER; i++)
	{
		lv += 0x4;
		header.SCL_lv3[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < SCL_BUFFER; i++)
	{
		lv += 0x4;
		header.SCL_lv4[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}




	for (int j = 0; j < MAXCHAR_KOG; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			lv += 0x4;
			header.LTEntry[j].entryPoint[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
		}
		lv += 0x4;
		header.LTEntry[j].num_tex = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
}


void Decode()
{
	getHeaderInfo();

	int lv = header.TexInit;
	//headerInfo();

	/*
	A partir de ac? leer? las instrucciones

	*/
	unsigned int sub_cnt = 0, sub_id = 0, sub_add[4096], get_subid = 0, lab_cnt = 0, lab_id = 0, lab_add[4096], get_labid = 0;
	unsigned int address, pop, push, sndId, chId, parId, param, type;
	unsigned char ar, ind, reg, id, spid, anmid, anmspd;
	unsigned int x1, x2, y1, y2;
	int dmg, w, var_content;
	unsigned char iter = 0, var, var2;
	short int x, y, v0, va, t, sint_cont;
	char vd;
	char strname[256];

	//unsigned int stck[256];
	//unsigned int stckPtr = 0;

	//for (int i = 0; i < 256; i++)
	//{
	//	stck[i] = 0xffffffff;
	//}
	//std::string texFile;

	for (int j = 0; j < (sizeof(sub_add) / sizeof(int)); j++)
	{
		sub_add[j] = 0x00000000;
	}
	for (int j = 0; j < (sizeof(lab_add) / sizeof(int)); j++)
	{
		lab_add[j] = 0x00000000;
	}

	sub_add[sub_cnt] = lv;
	sub_cnt++;
	int getpos = lv;
	lab_cnt = 0;
	lab_add[0] = 0;

	while (lv < lSize)
	{
		iter = 0;
		switch (uChar(buffer[lv]))
		{
		case 0x30:
			lv += 0x3; break;

		case 0x31:
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			for (int i = 0 ; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x9; break;

		case 0x40:
		{
			while (buffer[lv + 2 + iter])
			{
				iter++;
			}
			lv += 0x3 + iter; iter = 0; break;
		}
		case 0x41:
		{
			lv += 0xA; break;
		}
		case 0x42:
		{
			ar = uChar(buffer[lv + 2]);
			lv += 0x3 + (ar); break;
		}

		case 0x43:
		{
			while (buffer[lv + 2 + iter])
			{
				iter++;
			}
			lv += 0x3 + iter; iter = 0; break;
		}
		case 0x44:
		{
			while (buffer[lv + 1 + iter])
			{
				iter++;
			}
			lv += 0x2 + iter; iter = 0; break;
		}

		case 0x50:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x5; get_subid = 0; break;

		case 0x51:
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x9; get_subid = 0; break;

		case 0x52:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0x53:
			lv += 0x2; break;

		case 0x54:
			lv += 0x3; break;

		case 0x55:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0x56:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0x57:
			lv += 0x5; break;

		case 0x58:
			lv += 0x7; break;

		case 0x59:
			lv += 0x6; break;

		case 0x5b:
			lv += 0x2;  break;

		case 0x5d:
			lv += 0x3; break;

		case 0x5e:
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x6; break;

		case 0x5f:
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x6; break;

		case 0x61:
			lv += 0x5; break;

		case 0x62:
			lv += 0x7; break;

		case 0x63:
			lv += 0x6; break;

		case 0x64:
			lv += 0x3; break;

		case 0x65:
			address = convCharInt(buffer[lv + 9], buffer[lv + 10], buffer[lv + 11], buffer[lv + 12]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0xd; break;

		case 0x66:
			lv += 0x6;  break;

		case 0x75:
			lv += 0x5; break;

		case 0x93:
			lv += 0x3; break;

		case 0xa0:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (address == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = address;
					sub_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xa1:
			lv += 0x2; break;


		case 0xc2:
			lv += 0x6; break;

		case 0xc3:
			lv += 0x3; break;

		case 0xc4:
			lv += 0x5; break;

		case 0xc5:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xc6:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xc7:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xca:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xcb:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		case 0xcc:
			for (int i = 0; i < sub_cnt + 1; i++)
			{
				if (lv + 1 == sub_add[i])
				{
					break;
				}
				else if (sub_add[i] != 0x00000000)
				{
				}
				else
				{
					sub_add[i] = lv + 1;
					sub_cnt++;
					break;
				}
			}
			lv++; break;

		case 0xce:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			for (int i = 0; i < lab_cnt + 1; i++)
			{
				if (address == lab_add[i])
				{
					break;
				}
				else if (lab_add[i] != 0x00000000)
				{
				}
				else
				{
					lab_add[i] = address;
					lab_cnt++;
					break;
				}
			}
			lv += 0x5; break;

		default:
			if (lv < lSize - 1)
				lv++;
			break;
		}
	}

	std::vector<int> sb_ord(sub_add, sub_add + (sizeof(sub_add) / sizeof(int)));
	std::vector<int> lb_ord(lab_add, lab_add + (sizeof(lab_add) / sizeof(int)));

	std::sort(sb_ord.begin(), sb_ord.begin() + sub_cnt);
	std::sort(lb_ord.begin(), lb_ord.begin() + lab_cnt);

	for (int it = 0; it < sub_cnt; it++)
	{
		sub_add[it] = sb_ord[it];
	}
	for (int it = 0; it < lab_cnt; it++)
	{
		lab_add[it] = lb_ord[it];
	}

	lv = getpos;
	char instname[256];
	char * getName;
	int actPos = lv;
	int get_comboid = 0;
	get_subid = 0;
	get_labid = 0;
	while (lv < lSize)
	{
		get_subid = 0;
		get_labid = 0;
		for (int sb = 0; sb <= sub_cnt; sb++)
		{
			if (lv == sub_add[sb])
			{
				printf("Sub Sub%d:\n", sb);
				break;
			}
		}
		for (int lb = 0; lb <= lab_cnt; lb++)
		{
			if (lv == lab_add[lb])
			{
				printf("Lab_%d:\n", lb);
				break;
			}
		}
		for (int cb = 0; cb < MAXCHAR_KOG - 1; cb++)
		{
			if (lv == ComboAtk_add[cb])
			{
				printf("[Combo%d]\n", cb);
				break;
			}
		}

		unsigned char pos = buffer[lv];
		bool isIns = (pos == 0x30 || pos == 0x31 || (pos >= 0x40 && pos <= 0x45) ||
			(pos >= 0x50 && pos <= 0x66) || (pos >= 0x70 && pos <= 0x7d) ||
			(pos >= 0x90 && pos <= 0x93) || pos == 0xa0 || pos == 0xa1 ||
			(pos >= 0xc0 && pos <= 0xc7) || (pos >= 0xca && pos <= 0xce) ||
			(pos >= 0xd0 && pos <= 0xe1));

		if (isIns)
		{
			int getLength = ins_nameLength[pos];


			if (ins_nameLength[pos])
			{
				switch (pos)
				{
				case 0xcc:
				case 0x53:
					break;
				default:
					std::cout << "    ";
					break;
				}
				//if (pos == 0xcc)
				

				std::cout << ins_name[pos];
			}
			else
			{
				if (pos != 0xcc)
				std::cout << "    ";
				printf("ins_%d", uChar(buffer[lv]));
			}
		}


		switch (uChar(buffer[lv]))
		{
		case 0x30:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("(%d);\n", t);
			lv += 0x3; break;

		case 0x31:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
			printf("(%d, %d, Sub%d);\n", x, y, get_subid);
			lv += 0x9; break;

		case 0x40:
		{
			reg = uChar(buffer[lv - 1]);
			id = uChar(buffer[lv + 1]);
			while (buffer[lv + 2 + iter])
			{
				iter++;
			}
			for (int i = 0; i <= iter; i++)
			{
					strname[i] = buffer[lv + 2 + i];
					strname[i + 1] = 0;
			}
			//texFile = strname;
			printf("(%d, %d, \"%s\");\n", reg, id, strname);
			//delete[iter] strname;
			lv += 0x3 + iter; iter = 0; break;
		}
		case 0x41:
		{
			spid = uChar(buffer[lv + 1]);
			x1 = convCharUInt(buffer[lv + 2], buffer[lv + 3]);
			y1 = convCharUInt(buffer[lv + 4], buffer[lv + 5]);
			x2 = convCharUInt(buffer[lv + 6], buffer[lv + 7]);
			y2 = convCharUInt(buffer[lv + 8], buffer[lv + 9]);
			printf("(%d, %d, %d, %d);\n", spid, x1, y1, x2 - x1, y2 - y1);
			lv += 0xA; break;
		}
		case 0x42:
		{
			ind = uChar(buffer[lv + 1]);
			ar = uChar(buffer[lv + 2]);
			unsigned char sprites[128];
			for (int i = 0; i < ar; i++)
			{
				sprites[i] = uChar(buffer[lv + 3 + i]);
			}
			printf("(%d, %d, ", ind, ar);
			for (int i = 0; i < ar; i++)
			{
				if (i < ar - 1)
					printf("%d, ", sprites[i]);
				else
					printf("%d);", sprites[i]);
			}
			printf("\n");
			lv += 0x3 + (ar); break;
		}

		case 0x43:
		{
			reg = uChar(buffer[lv - 1]);
			id = uChar(buffer[lv + 1]);
			while (buffer[lv + 2 + iter])
			{
				iter++;
			}
			for (int i = 0; i <= iter; i++)
			{
					strname[i] = buffer[lv + 2 + i];
					strname[i + 1] = 0;
			}
			//texFile = strname;
			printf("(%d, %d, \"%s\");\n", reg, id, strname);

			lv += 0x3 + iter; iter = 0; break;
		}
		case 0x44:
		{
			while (buffer[lv + 1 + iter])
			{
				iter++;
			}
			for (int i = 0; i <= iter; i++)
			{
					strname[i] = buffer[lv + 1 + i];
					strname[i + 1] = 0;
			}
			//texFile = strname;
			printf("(\"%s\");\n", strname);
			lv += 0x2 + iter; iter = 0; break;
		}

		case 0x45:
			printf("();\n");
			lv++; break;

		case 0x50:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(Sub%d);\n", get_subid);
			lv += 0x5; get_subid = 0; break;

		case 0x51:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}

			printf("( %d, %d, Sub%d);\n", x, y, get_subid);
			lv += 0x9; get_subid = 0; break;

		case 0x52:

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(Sub%d);\n", get_subid);
			lv += 0x5; break;

		case 0x53:
			pop = buffer[lv + 1];
			printf("(var%d);\n\n", pop);
			lv += 0x2; break;

		case 0x54:
			anmid = uChar(buffer[lv + 1]);
			anmspd = uChar(buffer[lv + 2]);
			printf("(%d, %d);\n", anmid, anmspd);
			lv += 0x3; break;

		case 0x55:

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(Sub%d);\n", get_subid);
			lv += 0x5; break;

		case 0x56:

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(Sub%d);\n", get_subid);
			lv += 0x5; break;

		case 0x57:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			t = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			printf("(%d, %d);\n", v0, t);
			lv += 0x5; break;

		case 0x58:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			va = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			t = convCharInt(buffer[lv + 5], buffer[lv + 6]);
			printf("(%d, %d, %d);\n", v0, va, t);
			lv += 0x7; break;

		case 0x59:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			vd = buffer[lv + 3];
			t = convCharInt(buffer[lv + 4], buffer[lv + 5]);
			printf("(%d, %d, %d);\n", v0, vd, t);
			lv += 0x6; break;

		case 0x5a:
			printf("();\n");
			lv++; break;

		case 0x5b:
			sndId = uChar(buffer[lv + 1]);
			printf("(%d);\n", sndId);
			lv += 0x2;  break;

		case 0x5c:
			printf("();\n\n");
			lv++; break;

		case 0x5d:
			dmg = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			printf("(%d);\n", dmg);
			lv += 0x3; break;

		case 0x5e:
			chId = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(%d, Sub%d);\n", chId, get_subid);
			lv += 0x6; break;

		case 0x5f:

			chId = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(%d, Sub%d);\n", chId, get_subid);
			lv += 0x6; break;

		case 0x60:

			parId = uChar(buffer[lv - 1]);
			printf("(%d);\n", parId);
			lv++; break;

		case 0x61:
			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			t = convCharUInt(buffer[lv + 3], buffer[lv + 4]);
			printf("(%d, %d);\n", v0, t);
			lv += 0x5; break;

		case 0x62:
			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			va = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			t = convCharUInt(buffer[lv + 5], buffer[lv + 6]);
			printf("(%d, %d, %d);\n", v0, va, t);
			lv += 0x7; break;

		case 0x63:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			vd = buffer[lv + 3];
			t = convCharUInt(buffer[lv + 4], buffer[lv + 5]);
			printf("(%d, %d, %d);\n", v0, vd, t);
			lv += 0x6; break;

		case 0x64:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("(%d);\n", t);
			lv += 0x3; break;

		case 0x65:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			param = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			address = convCharInt(buffer[lv + 9], buffer[lv + 10], buffer[lv + 11], buffer[lv + 12]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(%d, %d, %d, Sub%d);\n", x, y, param, get_subid);
			lv += 0xd; break;

		case 0x66:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			type = uChar(buffer[lv + 5]);
			printf("(%d, %d, %d);\n", x, y, type);
			lv += 0x6;  break;

		case 0x70:
			printf("();\n");
			lv++; break;

		case 0x71:
			printf("();\n");
			lv++; break;

		case 0x72:
			printf("();\n");
			lv++; break;

		case 0x73:
			printf("();\n");
			lv++; break;

		case 0x74:
			printf("();\n");
			lv++; break;

		case 0x75:
			w = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("(%d);\n", w);
			lv += 0x5; break;

		case 0x76:
			printf("();\n");
			lv++; break;

		case 0x77:
			printf("();\n");
			lv++; break;

		case 0x78:
			printf("();\n");
			lv++; break;

		case 0x79:
			printf("();\n");
			lv++; break;

		case 0x7a:
			printf("();\n");
			lv++; break;

		case 0x7b:
			printf("();\n");
			lv++; break;

		case 0x7c:
			printf("();\n");
			lv++; break;

		case 0x7d:
			printf("();\n");
			lv++; break;

		case 0x90:
			printf("();\n");
			lv++; break;

		case 0x91:
			printf("();\n");
			lv++; break;

		case 0x92:
			printf("();\n");
			lv++; break;

		case 0x93:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("(%d);\n", t);
			lv += 0x3; break;

		case 0xa0:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != sub_add[get_subid])
			{
				get_subid++;
			}
				printf("(Sub%d);\n", get_subid);
			lv += 0x5; break;

		case 0xa1:
			type = uChar(buffer[lv + 1]);
			printf("(%d);\n", type);
			lv += 0x2; break;

		case 0xc0:
			var = uChar(buffer[lv - 1]);
			printf("(var%d);\n", var);
			lv++; break;

		case 0xc1:
			var = uChar(buffer[lv - 1]);
			printf("(var%d);\n", var);
			lv++; break;

		case 0xc2:
			var = uChar(buffer[lv + 1]);
			var_content = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("(var%d, %d);\n", var, var_content);
			lv += 0x6; break;

		case 0xc3:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			printf("var%d = var%d;\n", var, var2);
			lv += 0x3; break;

		case 0xc4:
			var_content = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("(%d);\n", var_content);
			lv += 0x5; break;

		case 0xc5:
			//var = uChar(buffer[lv - 1]);
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
				printf("(Lab\_%d)\n", get_labid);
			lv += 0x5; break;

		case 0xc6:
			//var = uChar(buffer[lv - 1]);
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
				printf("(Lab\_%d)\n", get_labid);
			lv += 0x5; break;

		case 0xc7:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
				printf("(Lab\_%d);\n", get_labid);
			lv += 0x5; break;

		case 0xca:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
			printf("(Lab\_%d);\n", get_labid);
			lv+= 0x5; break;

		case 0xcb:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
				printf("(Lab\_%d);\n", get_labid);
			lv+= 0x5; break;

		case 0xcc:
			printf("();\n\n");
			lv++; break;

		case 0xcd:
			printf("();\n");
			lv++; break;

		case 0xce:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			while (address != lab_add[get_labid])
			{
				get_labid++;
			}
				printf("(Lab\_%d);\n", get_labid);
			lv += 0x5; break;

		case 0xd0:
			printf("();\n");
			lv++; break;

		case 0xd1:
			printf("();\n");
			lv++; break;

		case 0xd2:
			printf("();\n");
			lv++; break;

		case 0xd3:
			printf("();\n");
			lv++; break;

		case 0xd4:
			printf("();\n");
			lv++; break;

		case 0xd5:
			printf("();\n");
			lv++; break;

		case 0xd6:
			printf("();\n");
			lv++; break;

		case 0xd7:
			printf("();\n");
			lv++; break;

		case 0xd8:
			printf("();\n");
			lv++; break;

		case 0xd9:
			printf("();\n");
			lv++; break;

		case 0xda:
			printf("();\n");
			lv++; break;

		case 0xdb:
			printf("();\n");
			lv++; break;

		case 0xdc:
			printf("();\n");
			lv++; break;

		case 0xdd:
			printf("();\n");
			lv++; break;

		case 0xde:
			printf("();\n");
			lv++; break;

		case 0xdf:
			printf("();\n");
			lv++; break;

		case 0xe0:
			printf("();\n");
			lv++; break;

		case 0xe1:
			printf("();\n");
			lv++; break;

		default:
			if (lv < lSize - 1)
			{
				lv++;	
			}
			break;
		}
#ifdef CLB_DEBUG
		//printf("buffer pos: 0x%x\n\n", lv);
		if (!isIns)
		{
			//printf("Espacio vac?o\n");
		}
#endif // CLB_DEBUG
	}

	get_subid = 0;
	get_labid = 0;
	printf("\nSCLlv1{\n");
	for (int i = 0; i < header.Lvl1SCL; i++)
	{
		while (header.SCL_lv1[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("SCLlv2{\n");
	for (int i = 0; i < header.Lvl2SCL; i++)
	{
		while (header.SCL_lv2[i] != sub_add[get_subid])
		{
			get_subid++;
		}

			printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("SCLlv3{\n");
	for (int i = 0; i < header.Lvl3SCL; i++)
	{
		while (header.SCL_lv3[i] != sub_add[get_subid])
		{
			get_subid++;
		}
			printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("SCLlv4{\n");
	for (int i = 0; i < header.Lvl4SCL; i++)
	{
		while (header.SCL_lv4[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;

	printf("AttackLv1{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.Lv1Attack[i] != sub_add[get_subid])
		{
			get_subid++;
		}
			printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("AttackLv2{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.Lv2Attack[i] != sub_add[get_subid])
		{
			get_subid++;
		}
			printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("BossAttack{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.BossAttack[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;

	printf("Lv1Portrait{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.AnmLv1[i] != sub_add[get_subid])
		{
			get_subid++;
		}
			printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("Lv2Portrait{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.AnmLv2[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("BossPortrait{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.AnmBoss[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	printf("WinPortrait{\n");
	for (int i = 0; i < MAXCHAR_KOG - 1; i++)
	{
		while (header.AnmWin[i] != sub_add[get_subid])
		{
			get_subid++;
		}
		printf("    Sub%d();\n", get_subid);
	}
	printf("}\n\n");
	get_subid = 0;
	get_labid = 0;
	for (int j = 0; j < MAXCHAR_KOG - 1; j++)
	{
		printf("TexPortrait%d{\n", j);
		for (int i = 0; i < header.LTEntry[j].num_tex; i++)
		{
			while (header.LTEntry[j].entryPoint[i] != sub_add[get_subid])
			{
				get_subid++;
			}
			printf("    Sub%d();\n", get_subid);
			get_subid = 0;
			get_labid = 0;
		}
		printf("}\n\n");
	}
}

void openFile(const char* fileName)
{


	//This code has been taken from https://cplusplus.com/reference/cstdio/fread/
	FILE* pFile;
	size_t result;

	pFile = fopen(fileName, "rb");
	if (pFile == NULL) { fputs("Couldnt open the file", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }
	//Until here

}


void openSCLmap(const char* fileName)
{


	//std::string ins_name[82];

	//This code has been taken from https://cplusplus.com/reference/cstdio/fread/
	FILE* pFile;
	size_t result;

	pFile = fopen(fileName, "rb");
	if (pFile == NULL) { fputs("Couldn't open the sclmap", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	mapSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	mapbuffer = (char*)malloc(sizeof(char) * mapSize);
	if (mapbuffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(mapbuffer, 1, mapSize, pFile);
	if (result != mapSize) { fputs("Reading error", stderr); exit(3); }
	//Until here

	int i = 0;
	while (mapbuffer[i] != '!')
	{
		i++;
		if (i > mapSize)
		{
			printf("No se encuentra la etiqueta \"!kog_ins\"\n");
			exit(4);
		}
	}
	char chkmap[7];
	chkmap[0] = 'k';
	chkmap[1] = 'o';
	chkmap[2] = 'g';
	chkmap[3] = '_';
	chkmap[4] = 'i';
	chkmap[5] = 'n';
	chkmap[6] = 's';
	i++;
	int j = 0;
	while (j < 7 && chkmap[j] == mapbuffer[i + j])
	{
		j++;
	}
	if (j < 7)
	{
		printf("Etiqueta desconocida.\n");
		exit(5);
	}
	i += j;


#ifdef CLB_DEBUG
	printf("Archivo abierto correctamente.\n");
#endif



	int ins_num = 0;
	int getins_n[4];
	getins_n[3] = 0;
	int k = 0;
	char insname[256];
	int l = 0;
	while (mapbuffer[i] != ';')
	{
		while (mapbuffer[i] >= '0' && mapbuffer[i] <= '9')
		{
			getins_n[k] = mapbuffer[i] - 0x30;
			if (getins_n[3] != 0)
			{
				printf("Numero de instruccion invalido.\n");
				exit(6);
			}
			k++;
			i++;
		}
		if (mapbuffer[i - 1] >= '0' && mapbuffer[i - 1] <= '9')
		{
			if (k == 3)
			{
				ins_num = (getins_n[0] * 100) + (getins_n[1] * 10) + getins_n[2];
			}
			else if (k == 2)
			{
				ins_num = (getins_n[0] * 10) + getins_n[1];
			}
			else if (k == 0)
			{
			}
			else
			{
				printf("Numero de instruccion invalido.\n");
				exit(6);
			}

			while (ins_num >= 0x30 && !(mapbuffer[i] >= 'A' && mapbuffer[i] <= 'z') && mapbuffer[i] != ';')
			{
				i++;
			}
			k = 0;
			int size = 0;
			int init_pos = i;
			while ((mapbuffer[i] >= 'A' && mapbuffer[i] <= 'z') || (mapbuffer[i] >= '0' && mapbuffer[i] <= '9'))
			{
				insname[size] = mapbuffer[i];
				insname[size + 1] = 0;
				l++;
				i++;
				size++;
			}
			l = 0;
			if (ins_num == 0x30 || ins_num == 0x31 || (ins_num >= 0x40 && ins_num <= 0x45) ||
				(ins_num >= 0x50 && ins_num <= 0x66) || (ins_num >= 0x70 && ins_num <= 0x7d) ||
				(ins_num >= 0x90 && ins_num <= 0x93) || ins_num == 0xa0 || ins_num == 0xa1 ||
				(ins_num >= 0xc0 && ins_num <= 0xc7) || (ins_num >= 0xca && ins_num <= 0xce) ||
				(ins_num >= 0xd0 && ins_num <= 0xe1))
			{
				ins_namePos[ins_num] = init_pos;
				ins_nameLength[ins_num] = size;
				ins_name[ins_num] = insname;
			}

		}


		i++;
	}
}

#ifdef CLB_DEBUG
int main()
{
	int number_1 = 3;
	std::string filename;
	std::string mapname;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	std::cout << "Nombre del mapa: "; std::cin >> mapname;
	const char* fName = filename.c_str();
	const char* mName = mapname.c_str();
	openSCLmap(mName);
	openFile(fName);
	Decode();

	free(buffer);
	free(mapbuffer);
	//system("pause");
}
#endif

#ifndef CLB_DEBUG

int main(int argc, char* argv[])
{
	if (argv[1] == NULL)
	{
		printf("Uso: KOG_SCL.exe input [sclmap]\n");
		return 1;
	}
	else
	{
		const char* fName = argv[1];
		openFile(fName);
		if (argv[2] != NULL)
		{
			const char* fName = argv[2];
			openSCLmap(fName);
		}
		Decode();
	}
	free(buffer);
	free(mapbuffer);
	//printf("Parametros:\n - -d: Para decodificar el archivo.\n - -m: Para leer un sclmap (Se usa junto a -d).\n");
	//return -1;
}
#endif // !
