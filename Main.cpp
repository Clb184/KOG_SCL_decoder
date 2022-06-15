#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#define MAXCHAR_KOG 10
#define SCL_BUFFER 50

struct int_ch
{
	char b1;
	char b2;
	char b3;
	char b4;
};

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

	//Dirección del Inicializador de texturas.
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

/*int convCharInt(char c1, char c2, char c3, char c4)
{
	char a[4];
	a[0] = c1;
	a[1] = c2;
	a[2] = c3;
	a[3] = c4;
	return *(int*)a;
}*/

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

void openFile(const char* fileName)
{
	SCLHeader header = {};
	FILE* pFile;
	long lSize;
	char* buffer;
	size_t result;

	pFile = fopen(fileName, "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

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
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.Lv2Attack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.BossAttack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.ComboAttack[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}



	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmLv1[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmLv2[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmBoss[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
	}
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		lv += 0x4;
		header.AnmWin[i] = convCharInt(buffer[lv + 0], buffer[lv + 1], buffer[lv + 2], buffer[lv + 3]);
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
	lv = header.TexInit;


	/*
	A partir de acá leeré las instrucciones
	
	*/
	int sub_id;
	unsigned int address, pop, push, sndId, chId, parId, param, type;
	unsigned char ar, ind, reg, id, spid, anmid, anmspd;
	unsigned int x1, x2, y1, y2;
	int dmg, w, var_content;
	unsigned char iter = 0, var, var2;
	short int x, y, v0, va, t, sint_cont;
	char vd;
	char strname[256];

	unsigned int stck[256];
	unsigned int stckPtr = 0;

	for (int i = 0; i < 256; i++)
	{
		stck[i] = 0xffffffff;
	}
	//std::string texFile;







// Parte de escritura en la pantalla

	std::cout << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 1: " << header.Lvl1SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 2: " << header.Lvl2SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 3: " << header.Lvl3SCL << std::endl;
	std::cout << "Cantidad de enemigos SCL Lvl. 4: " << header.Lvl4SCL << std::endl << std::endl;

	printf("Direccion de inicializacion de texturas: 0x%x\n\n", header.TexInit);

	std::string charaName;

	std::cout << "Direcciones de los Ataques de Nivel 1 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if(i != 9)
			printf("%s: 0x%x\n", charaName, header.Lv1Attack[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Ataques de Nivel 2 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.Lv2Attack[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Bossfights //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.BossAttack[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de los Combo Attacks //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.ComboAttack[i]);
		if (i == 9)
			std::cout << std::endl;
	}



	std::cout << "// Direcciones de retrato de ataque de Nivel 1 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmLv1[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de retrato de ataque de Nivel 2 //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmLv2[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direcciones de retrato de Jefe //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (i != 9)
			printf("%s: 0x%x\n", charaName, header.AnmBoss[i]);
		if (i == 9)
			std::cout << std::endl;
	}
	std::cout << "// Direccion de retratos de victoria //" << std::endl;
	for (int i = 0; i < MAXCHAR_KOG; i++)
	{
		switch (i)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
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
		switch (j)
		{
		case 0: charaName = "VIVIT"; break;
		case 1: charaName = "Milia"; break;
		case 2: charaName = "Mei & Mai"; break;
		case 3: charaName = "Gates"; break;
		case 4: charaName = "Marie"; break;
		case 5: charaName = "Erich"; break;
		case 6: charaName = "Morgan"; break;
		case 7: charaName = "Muse"; break;
		case 8: charaName = "Yuuka"; break;
		case 9: charaName = ""; break;
		}
		if (j < 9)
		std::cout << charaName << ": " << std::endl;
		for (int i = 0; i < header.LTEntry[j].num_tex; i++)
		{
			printf("Direccion Tex %d: 0x%x,\n", i + 1, header.LTEntry[j].entryPoint[i]);

			if (i >= header.LTEntry[j].num_tex - 1)
				std::cout << std::endl;
		}
		if(j < 9)
		std::cout << "Cantidad de direccion de texturas: " << header.LTEntry[j].num_tex << std::endl;

		std::cout << std::endl;
	}





	sub_id = 0;
	printf("Sub%d:\n", sub_id);
	sub_id++;
	int actPos = lv;
	while(lv < lSize)
	{
		switch (uChar(buffer[lv]))
		{
		case 0x30:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("wait{ %d, }\n\n", t);
			lv += 0x3; break;

		case 0x31:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			printf("enmCreate{\n	x: %d,\n	y: %d,\n	address: 0x%x,\n}\n\n", x, y, address);
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
				if(i < iter)
				strname[i] = buffer[lv + 2 + i];
				if (i >= iter)
					strname[i] = 0;
			}
			//texFile = strname;
			printf("loadTexture{\n    Reg: %d,\n    Index: %d,\n    Filename: \"%s\",\n}\n\n", reg, id, strname);
			//delete[iter] strname;
			lv += 0x2 + iter; iter = 0; break;
		}
		case 0x41:
		{
			spid = uChar(buffer[lv + 1]);
			x1 = convCharUInt(buffer[lv + 2], buffer[lv + 3]);
			y1 = convCharUInt(buffer[lv + 4], buffer[lv + 5]);
			x2 = convCharUInt(buffer[lv + 6], buffer[lv + 7]);
			y2 = convCharUInt(buffer[lv + 8], buffer[lv + 9]);
			printf("sprite%d: { xi: %d, yi: %d, xf: %d, yf: %d }\n\n", spid, x1, y1, x2, y2);
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
			printf("AnimeDef{\n    Index: %d,\n    spr_number: %d,\n    sprites: { ", ind, ar);
			for (int i = 0; i < ar; i++)
			{
				if(i < ar - 1)
				printf("sprite%d, ", sprites[i]);
				else
				printf("sprite%d }\n", sprites[i]);
			}
			printf("}\n\n");
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
				if (i < iter)
					strname[i] = buffer[lv + 2 + i];
				if (i >= iter)
					strname[i] = 0;
			}
			//texFile = strname;
			printf("loadTextureOld{\n    Reg: %d,\n    Index: %d,\n    Filename: \"%s\",\n}\n\n", reg, id, strname);

			lv += 0x2 + iter; iter = 0; break;
		}
		case 0x44:
		{
			while (buffer[lv + 1 + iter])
			{
				iter++;
			}
			for (int i = 0; i <= iter; i++)
			{
				if (i < iter)
					strname[i] = buffer[lv + 1 + i];
				if (i >= iter)
					strname[i] = 0;
			}
			//texFile = strname;
			printf("loadTextureEx{\n    Filename: \"%s\",\n}\n\n", strname);
			lv += 0x2 + iter; iter = 0; break;
		}

		case 0x45:
			printf("anmStop;\n\n");
			lv++; break;

		case 0x50:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("call{ address: 0x%x, }\n\n", address);
			lv += 0x5; break;

		case 0x51:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			address = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			printf("atkCreate{\n    dx: %d,\n    dy: %d,\n    address: 0x%x,\n}\n\n", x, y, address);
			lv += 0x9;  break;

		case 0x52: 

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("enmSet{ address: 0x%x, }\n\n", address);
			lv += 0x5; break;

		case 0x53:
			pop = buffer[lv + 1];
			printf("return{ pop: %d, }\n\n", pop);
			lv += 0x2; break;

		case 0x54:
			anmid = uChar(buffer[lv + 1]);
			anmspd = uChar(buffer[lv + 2]);
			printf("anmSet{\n    anmId: %d,\n    speed: %d,\n}\n\n", anmid, anmspd);
			lv += 0x3; break;

		case 0x55:

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("deathCall{ address: 0x%x, }\n\n", address);
			lv += 0x5; break;

		case 0x56:

			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("setParentlessObj{ address: 0x%x, }\n\n", address);
			lv += 0x5; break;

		case 0x57:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			t = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			printf("moveVel{\n    vel: %d,\n    time:%d,\n}\n\n", v0, t);
			lv += 0x5; break;

		case 0x58:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			va = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			t = convCharInt(buffer[lv + 5], buffer[lv + 6]);
			printf("moveVelAccel{\n    velInit: %d,\n    accel: %d,\n    time:%d,\n}\n\n", v0, va, t);
			lv += 0x7; break;

		case 0x59:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			vd = buffer[lv + 3];
			t = convCharInt(buffer[lv + 4], buffer[lv + 5]);
			printf("moveVelRot{\n    velInit: %d,\n    angVel: %d,\n    time:%d,\n}\n\n", v0, vd, t);
			lv += 0x6; break;

		case 0x5a:
			printf("waitMainObjRet;\n\n");
			lv++; break;

		case 0x5b:
			sndId = uChar(buffer[lv + 1]);
			printf("playSound{ %d, }\n\n", sndId);
			lv += 0x2;  break;

		case 0x5c:
			printf("die;\n\n");
			lv++; break;

		case 0x5d:
			dmg = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			printf("madnessGaugeDmg{ %d, }\n\n", dmg);
			lv += 0x3; break;
		case 0x5e:
			chId = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("childCreate{\n    id: %d,\n    address: 0x%x,\n}\n\n", address);
			lv += 0x6; break;

		case 0x5f:

			chId = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("changeChildTask{\n    id: %d,\n    address: 0x%x,\n}\n\n", address);
			lv += 0x6; break;
		case 0x60:

			parId = uChar(buffer[lv + 1]);
			printf("parentId{ id: %d, }\n\n", parId);
			lv += 0x2; break;

		case 0x61:
			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			t = convCharUInt(buffer[lv + 3], buffer[lv + 4]);
			printf("LMovSyncParent{\n    v0: %d,\n    time: %d,\n}\n\n", v0, t);
			lv += 0x5; break;

		case 0x62:
			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			va = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			t = convCharUInt(buffer[lv + 5], buffer[lv + 6]);
			printf("AcMovSyncParent{\n    velInit: %d,\n    accel: %d,\n    time: %d,\n}\n\n", v0, va, t);
			lv += 0x7; break;

		case 0x63:

			v0 = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			vd = buffer[lv + 3];
			t = convCharUInt(buffer[lv + 4], buffer[lv + 5]);
			printf("RotMovSyncParent{\n    velInit: %d,\n    angVel: %d,\n    time:%d,\n}\n\n", v0, vd, t);
			lv += 0x6; break;

		case 0x64:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("WaitSyncParent{ time: %d, }\n\n", t);
			lv += 0x3; break;

		case 0x65:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			param = convCharInt(buffer[lv + 5], buffer[lv + 6], buffer[lv + 7], buffer[lv + 8]);
			address = convCharInt(buffer[lv + 9], buffer[lv + 10], buffer[lv + 11], buffer[lv + 12]);
			printf("atkSet2{\n    dx: %d,\n    dy: %d,\n    param: 0x%x,\n    address: 0x%x,\n}\n\n", x, y, param, address);
			lv += 0xc; break;

		case 0x66:
			x = convCharInt(buffer[lv + 1], buffer[lv + 2]);
			y = convCharInt(buffer[lv + 3], buffer[lv + 4]);
			type = uChar(buffer[lv + 5]);
			printf("effCreate{\n    dx: %d,\n    dy: %d,\n    type: %d,\n}\n\n", x, y, type);
			lv += 0x6;  break;

		case 0x70:
			printf("tamaOn;\n\n");
			lv++; break;

		case 0x71:
			printf("laserOn;\n\n");
			lv++; break;

		case 0x72:
			printf("enmAng;\n\n");
			lv++; break;

		case 0x73:
			printf("aimAng;\n\n");
			lv++; break;

		case 0x74:
			printf("laserAccumulated;\n\n");
			lv++; break;

		case 0x75:
			w = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("laserOpen{ width: %d,}\n\n", w);
			lv += 0x5; break;


		case 0x76:
			printf("laserClose;\n\n");
			lv++; break;

		case 0x77:
			printf("hominglOn;\n\n");
			lv++; break;

		case 0x78:
			printf("lightningSphere;\n\n");
			lv++; break;

		case 0x79:
			printf("MorganRingOn;\n\n");
			lv++; break;

		case 0x7a:
			printf("crossOn;\n\n");
			lv++; break;

		case 0x7b:
			printf("flowerOn;\n\n");
			lv++; break;

		case 0x7c:
			printf("GFireOn;\n\n");
			lv++; break;

		case 0x7d:
			printf("ionRingOn;\n\n");
			lv++; break;

		case 0x91:
			printf("waitKeyMSG;\n\n");
			lv++; break;

		case 0x92:
			printf("newLineMSG;\n\n");
			lv++; break;

		case 0x93:
			t = convCharUInt(buffer[lv + 1], buffer[lv + 2]);
			printf("waitMSG{ %d, }\n\n", t);
			lv += 0x3; break;

		case 0xa0:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("anmTask{ address: 0x%d, }\n\n", address);
			lv += 0x5; break;

		case 0xa1:
			type = uChar(buffer[lv + 1]);
			printf("anmTexMode{ mode: %d, }\n\n", type);
			lv += 0x2; break;

		case 0xc0:
			var = uChar(buffer[lv - 1]);
			printf("PUSH var%d;\n\n", var);
			lv++; break;

		case 0xc1:
			var = uChar(buffer[lv - 1]);
			printf("POP var%d;\n\n", var);
			lv += 0x1; break;

		case 0xc2:
			var = uChar(buffer[lv + 1]);
			var_content = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("var%d = %d;\n\n", var, var_content);
			lv += 0x6; break;

		case 0xc3:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			printf("var%d = var%d;\n\n", var, var2);
			lv += 0x3; break;

		case 0xc4:
			var_content = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("PUSH %d;\n\n", var_content);
			lv += 0x5; break;

		case 0xc5:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("POPJmpTrue{\n    POP var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xc6:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("POPJmpFalse{\n    POP var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xc7:
			address = convCharInt(buffer[lv + 1], buffer[lv + 2], buffer[lv + 3], buffer[lv + 4]);
			printf("jmp{ address: 0x%x, }\n\n", address);
			lv += 0x5; break;

		case 0xcc:
			printf("endFunc;\n\n");
			if (lv < lSize - 1)
			{
				printf("Sub%d:\n", sub_id);
				sub_id++;
			}
			lv++; break;

		case 0xcd:
			var = uChar(buffer[lv + 1]);
			address = convCharUInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("JmpStackTop{\n    var: %d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xd0:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("PUSH var%d + var%d;\n\n", var, var2);
			printf("PUSH SUM;\n\n");
			lv++; break;

		case 0xd1:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("PUSH var%d - var%d;\n\n", var, var2);
			printf("PUSH SUB;\n\n");
			lv++; break;

		case 0xd2:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("PUSH var%d * var%d;\n\n", var, var2);
			printf("PUSH MUL;\n\n");
			lv++; break;

		case 0xd3:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("PUSH var%d / var%d;\n\n", var, var2);
			printf("PUSH DIV;\n\n");
			lv++; break;

		case 0xd4:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("PUSH var%d \% var%d;\n\n", var, var2);
			printf("PUSH MOD;\n\n");
			lv++; break;

		case 0xd5:
			var = uChar(buffer[lv + 1]);
			//printf("PUSH -1 * var%d;\n\n", var);
			printf("PUSH NEG;\n\n");
			lv++; break;

		case 0xd6:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			//printf("sin(var%d, var%d);\n\n", var, var_content);
			printf("PUSH SIN;");
			lv++; break;

		case 0xd7:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 1]);
			//printf("cos(var%d, var%d);\n\n", var, var2);
			printf("PUSH COS;");
			lv++; break;

		case 0xd8:
			//var = uChar(buffer[lv + 1]);
			printf("PUSH rnd();\n\n", var);
			lv++; break;

		case 0xd9:
			var = uChar(buffer[lv + 1]);
			var2 = uChar(buffer[lv + 2]);
			printf("PUSH atan;\n\n", var, var2);
			lv++; break;

		case 0xda:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpEq{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xdb:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpNotEq{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xdc:
			var = uChar(buffer[lv + 1]);
			address = convCharUInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpGreat{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xdd:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpLess{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xde:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpGrEq{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xdf:
			var = uChar(buffer[lv + 1]);
			address = convCharInt(buffer[lv + 2], buffer[lv + 3], buffer[lv + 4], buffer[lv + 5]);
			printf("jmpLsEq{\n    push: var%d,\n    address: 0x%x,\n}\n\n", var, address);
			lv += 0x6; break;

		case 0xe0:
			var = uChar(buffer[lv + 1]);
			sint_cont = convCharInt(buffer[lv + 2], buffer[lv + 3]);
			//printf("PUSH MAX(var%d, %d);\n\n", var, sint_cont);
			printf("PUSH MAX();\n\n");
			lv++; break;

		case 0xe1:
			printf("PUSH MIN();\n\n");
			lv++; break;

		default: lv++; break;
		}
#ifdef CLB_DEBUG
		printf("buffer pos: 0x%x\n\n", lv);
#endif // CLB_DEBUG

	}
	//delete[] buffer;
}

#ifdef CLB_DEBUG
int main()
{
	int number_1 = 3;
	std::string filename;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	const char* fName = filename.c_str();
	openFile(fName);
	//system("pause");
}
#endif

#ifndef CLB_DEBUG

int main(int argc, char* argv[])
{
	if (argv[1] == NULL)
	{
		printf("Uso: KOG_SCL.exe input\n");
		return 1;
	}
	else
	{
		const char* fName = argv[1];
		openFile(fName);
	}
}
#endif // !
