#include "AESCipher.h"
#include "WzArchive.h"
#include <string.h>
#include <stdlib.h>


#define BYTEn(x, n)   (*((char*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1) 
#define HIBYTE(x)   (*((char*)&(x)+1))

const char AESCipher::bDefaultAESKeyValue[16] = 
{
	(char)0xC6, (char)0x50, (char)0x53, (char)0xF2, (char)0xA8, (char)0x42, (char)0x9D,
	(char)0x7F, (char)0x77, (char)0x09, (char)0x1D, (char)0x26, (char)0x42, 
	(char)0x53, (char)0x88, (char)0x7C
};

AESCipher::AESCipher()
{
	/*UserKey[0] = 0x13;
	UserKey[1] = 0x52;
	UserKey[2] = 0x2A;
	UserKey[3] = 0x5B;
	UserKey[4] = 0x08;
	UserKey[5] = 0x02;
	UserKey[6] = 0x10;
	UserKey[7] = 0x60;
	UserKey[8] = 0x06;
	UserKey[9] = 0x02;
	UserKey[10] = 0x43;
	UserKey[11] = 0x0F;
	UserKey[12] = 0xB4;
	UserKey[13] = 0x4B;
	UserKey[14] = 0x35;
	UserKey[15] = 0x05;
	UserKey[16] = 0x1B;
	UserKey[17] = 0x0A;
	UserKey[18] = 0x5F;
	UserKey[19] = 0x09;
	UserKey[20] = 0x0F;
	UserKey[21] = 0x50;
	UserKey[22] = 0x0C;
	UserKey[23] = 0x1B;
	UserKey[24] = 0x33;
	UserKey[25] = 0x55;
	UserKey[26] = 0x01;
	UserKey[27] = 0x09;
	UserKey[28] = 0x52;
	UserKey[29] = 0xDE;
	UserKey[30] = 0xC7;
	UserKey[31] = 0x1E;*/
}


AESCipher::~AESCipher()
{
}

void AESCipher::Decrypt(char * pDest, char * pSrc, int nLen, unsigned int * pdwKey)
{
	AES_ALG_INFO AlgInfo; // [sp+4h] [bp-138h]@1
	unsigned int DstLen = 0;
	AES_EncKeySchedule(UserKey, &AlgInfo);
	AES_DecInit(&AlgInfo, pdwKey);
	if (nLen)
		OFB_DecUpdate(&AlgInfo, pSrc, nLen, pDest, &DstLen);
	OFB_DecFinal(&AlgInfo, &pDest[DstLen], &DstLen);
}

void AESCipher::Encrypt(char * pDest, char * pSrc, int nLen, unsigned int * pdwKey)
{
	AES_ALG_INFO AlgInfo; // [sp+4h] [bp-138h]@1
	unsigned int DstLen = 0;
	AES_EncKeySchedule(UserKey, &AlgInfo);
	AES_EncInit(&AlgInfo, pdwKey);
	if (nLen)
		OFB_EncUpdate(&AlgInfo, pSrc, nLen, pDest, &DstLen);
	OFB_EncFinal(&AlgInfo, &pDest[DstLen], &DstLen);
}

std::string AESCipher::DecodeString(WzArchive * pArchive)
{
	static char in[0x10000];
	static char16_t ws[0x8000];
	static char ns[0x10000];
	int nLen = 0;

	pArchive->Read((char*)&nLen, 1);
	char cLen = ((char*)&nLen)[0];
	if (cLen > 0)
	{
		if (cLen == 127)
			pArchive->Read((char*)&nLen, 4);
		nLen *= 2;

		Decrypt(ns, pArchive->GetStream()->GetStreamPtr(), nLen, &BasicKey);
	}
	else
	{
		if (cLen == -128)
			pArchive->Read((char*)&nLen, 4);
		else
			nLen = cLen * -1;

		Decrypt(ns, pArchive->GetStream()->GetStreamPtr(), nLen, &BasicKey);
	}

	pArchive->GetStream()->SetPosition(pArchive->GetStream()->GetPosition() + nLen);
	ns[nLen] = 0;
	return std::string(ns, nLen);
}

void AESCipher::GenTables()
{
	unsigned int v0; // ecx@1
	int v1; // eax@1
	int v2; // eax@3
	unsigned int v3; // ecx@3
	unsigned int v4; // ebx@5
	unsigned __int8 v5; // al@7
	int v6; // edx@9
	int v7; // eax@9
	int v8; // ecx@9
	int v9; // edx@9
	int v10; // eax@9
	int v11; // ecx@9
	unsigned int v12; // ebx@10
	unsigned __int32 v13; // edi@11
	unsigned int v14; // esi@11
	int v15; // ecx@12
	int v16; // eax@15
	int v17; // edi@17
	unsigned __int32 v18; // eax@17
	unsigned __int32 v19; // edi@17
	int v20; // ebp@17
	int v21; // ecx@23
	int v22; // eax@26
	int v23; // edi@28
	int v24; // [sp+10h] [bp-4h]@18

	log_tab[7] = 0;
	v0 = 0;
	v1 = 1;
	do
	{
		pow_tab[v0] = v1;
		log_tab[v1] = v0;
		v1 = (unsigned __int8)(2 * v1) ^ ((v1 & 0x80) != 0 ? 0x1B : 0) ^ (unsigned __int8)v1;
		++v0;
	} while (v0 < 0x100);
	log_tab[1] = 0;
	v2 = 1;
	v3 = (unsigned int)rco_tab;
	do
	{
		*(int *)v3 = v2;
		v3 += 4;
		v2 = ((v2 & 0x80) != 0 ? 0x1B : 0) ^ (unsigned __int8)(2 * v2);
	} while (v3 < (unsigned int)isb_tab);
	v4 = 0;
	do
	{
		if (v4)
			v5 = pow_tab[-(unsigned __int8)log_tab[v4] + 255];
		else
			v5 = 0;
		v6 = v5;
		v7 = (unsigned __int8)(2 * v5) | (v5 >> 7);
		v8 = (unsigned __int8)(2 * v7) | (unsigned __int8)((unsigned int)v7 >> 7);
		v9 = v8 ^ v7 ^ v6;
		v10 = (unsigned __int8)(2 * v8) | (unsigned __int8)((unsigned int)v8 >> 7);
		v11 = v10 ^ v9 ^ ((unsigned __int8)(2 * v10) | (unsigned __int8)((unsigned int)v10 >> 7)) ^ 0x63;
		sbx_tab[v4] = v11;
		isb_tab[v11] = v4++;
	} while (v4 < 0x100);
	v12 = 0;
	do
	{
		v13 = (unsigned __int8)sbx_tab[v12];
		v14 = v12;
		fl_tab[0][v12] = v13;
		fl_tab[1][v14] = _lrotl(v13, 8);
		fl_tab[2][v14] = _lrotl(v13, 16);
		fl_tab[3][v14] = _lrotl(v13, 24);
		if (v13)
			v15 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[2] + (unsigned __int8)log_tab[v13]) % 255];
		else
			v15 = 0;
		if (v13)
			v16 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[3] + (unsigned __int8)log_tab[v13]) % 255];
		else
			v16 = 0;
		v17 = v15 | ((v13 | ((v13 | (v16 << 8)) << 8)) << 8);
		ft_tab[0][v14] = v17;
		ft_tab[1][v14] = _lrotl(v17, 8);
		ft_tab[2][v14] = _lrotl(v17, 16);
		v18 = _lrotl(v17, 24);
		v19 = (unsigned __int8)isb_tab[v12];
		ft_tab[3][v14] = v18;
		il_tab[0][v14] = v19;
		il_tab[1][v14] = _lrotl(v19, 8);
		il_tab[2][v14] = _lrotl(v19, 16);
		v20 = 0;
		il_tab[3][v14] = _lrotl(v19, 24);
		if (v19)
			v24 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[14] + (unsigned __int8)log_tab[v19]) % 255];
		else
			v24 = 0;
		if (v19)
			v20 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[9] + (unsigned __int8)log_tab[v19]) % 255];
		if (v19)
			v21 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[13] + (unsigned __int8)log_tab[v19]) % 255];
		else
			v21 = 0;
		if (v19)
			v22 = (unsigned __int8)pow_tab[((unsigned __int8)log_tab[11] + (unsigned __int8)log_tab[v19]) % 255];
		else
			v22 = 0;
		v23 = v24 | ((v20 | ((v21 | (v22 << 8)) << 8)) << 8);
		it_tab[0][v14] = v23;
		it_tab[1][v14] = _lrotl(v23, 8);
		it_tab[2][v14] = _lrotl(v23, 16);
		++v12;
		it_tab[3][v14] = _lrotl(v23, 24);
	} while (v12 < 0x100);
	m_bTableGen = true;
}

void AESCipher::RIJNDAEL_KeySchedule(unsigned int * UserKey, unsigned int * e_key)
{
	unsigned int *v2; // esi@3
	unsigned int v3; // eax@3
	int v4; // edi@3
	unsigned __int32 v5; // ebx@4
	unsigned int v6; // ebx@4
	unsigned __int32 v7; // eax@4
	unsigned int v8; // ebx@4
	unsigned int v9; // ebx@4
	int v10; // ebx@4
	int v11; // ebx@4
	int v12; // ebx@4
	unsigned int v13; // edi@4
	int v14; // edi@4
	int v15; // edi@4
	unsigned int *UserKeya; // [sp+8h] [bp+8h]@3

	if (!m_bTableGen)
		GenTables();
	*e_key = *UserKey;
	e_key[1] = UserKey[4];
	v2 = e_key + 1;
	e_key[2] = UserKey[8];
	e_key[3] = UserKey[12];
	e_key[4] = UserKey[16];
	e_key[5] = UserKey[20];
	e_key[6] = UserKey[24];
	v3 = UserKey[28];
	e_key[7] = v3;
	v4 = v3;
	UserKeya = rco_tab;
	do
	{
		v5 = _lrotr(v4, 8) >> 24;
		v6 = fl_tab[2][(unsigned __int8)(_lrotr(v4, 8) >> 16)] ^ fl_tab[3][v5];
		v7 = _lrotr(v4, 8);
		v8 = fl_tab[1][BYTE1(v7)] ^ v6;
		v9 = *(v2 - 1) ^ *UserKeya ^ fl_tab[0][(unsigned __int8)_lrotr(v4, 8)] ^ v8;
		++UserKeya;
		v2[7] = v9;
		v10 = *v2 ^ v9;
		v2[8] = v10;
		v11 = v2[1] ^ v10;
		v2[9] = v11;
		v12 = v2[2] ^ v11;
		v2[10] = v12;
		v13 = v2[3] ^ fl_tab[0][(unsigned __int8)v12] ^ fl_tab[1][BYTE1(v12)] ^ fl_tab[2][(unsigned __int8)((unsigned int)v12 >> 16)] ^ fl_tab[3][(unsigned int)v12 >> 24];
		v2[11] = v13;
		v14 = v2[4] ^ v13;
		v2[12] = v14;
		v15 = v2[5] ^ v14;
		v2[13] = v15;
		v4 = v2[6] ^ v15;
		v2[14] = v4;
		v2 += 8;
	} while (UserKeya < &rco_tab[7]);
}

void AESCipher::AES_EncInit(AES_ALG_INFO * AlgInfo, unsigned int *pdwKey)
{
	AlgInfo->BufLen = 0;
	if (pdwKey)
	{
		for (int i = 0; i < 4; ++i)
			((int*)AlgInfo->ChainVar)[i] = *pdwKey;
	}
	else
		memcpy(&AlgInfo, bDefaultAESKeyValue, 16);
}

void AESCipher::AES_EncKeySchedule(unsigned int * UserKey, AES_ALG_INFO * AlgInfo)
{
	AlgInfo->RoundKey[0] = 8;
	RIJNDAEL_KeySchedule(UserKey, &AlgInfo->RoundKey[1]);
}

void AESCipher::AES_Encrypt(void * CipherKey, char * Data)
{
	int v2; // edx@1
	int v3; // esi@1
	int b0_4; // ST20_4@1
	int v5; // edi@1
	unsigned int b1_4; // ST10_4@1
	unsigned int v7; // ebx@1
	unsigned int v8; // ecx@1
	unsigned int v9; // esi@1
	unsigned int b0; // ST1C_4@1
	unsigned int v11; // edx@1
	unsigned int v12; // ST20_4@1
	unsigned int v13; // esi@1
	unsigned int b1; // ST0C_4@1
	unsigned int v15; // edi@1
	unsigned int v16; // ST10_4@1
	unsigned int v17; // ebx@1
	unsigned int v18; // esi@1
	unsigned int v19; // ST1C_4@1
	unsigned int v20; // edi@1
	unsigned int v21; // ST20_4@1
	unsigned int v22; // ecx@1
	unsigned int v23; // esi@1
	unsigned int v24; // ST0C_4@1
	unsigned int v25; // edi@1
	unsigned int v26; // edx@1
	unsigned int v27; // esi@1
	unsigned int v28; // ST1C_4@1
	unsigned int v29; // ST20_4@1
	unsigned int v30; // ebx@1
	unsigned int v31; // esi@1
	unsigned int v32; // ST0C_4@1
	unsigned int v33; // ST10_4@1
	unsigned int v34; // ecx@1
	unsigned int v35; // esi@1
	unsigned int v36; // ST1C_4@1
	unsigned int v37; // ST20_4@1
	unsigned int v38; // edx@1
	unsigned int v39; // esi@1
	unsigned int v40; // ST0C_4@1
	unsigned int v41; // edi@1
	unsigned int v42; // ebx@1
	unsigned int v43; // esi@1
	unsigned int v44; // ST1C_4@1
	unsigned int v45; // ST20_4@1
	unsigned int v46; // ecx@1
	unsigned int v47; // esi@1
	unsigned int v48; // ST0C_4@1
	unsigned int v49; // edx@1
	unsigned int v50; // ST10_4@1
	unsigned int v51; // esi@1
	unsigned int v52; // ST1C_4@1
	unsigned int v53; // ST20_4@1
	unsigned int v54; // ebx@1
	unsigned int v55; // esi@1
	unsigned int v56; // ST0C_4@1
	unsigned int v57; // ST10_4@1
	unsigned int v58; // ecx@1
	unsigned int v59; // ebx@1
	unsigned int v60; // ST20_4@1
	unsigned int v61; // esi@1
	unsigned int v62; // edi@1
	unsigned int v63; // ecx@1

	v2 = *((int *)CipherKey + 1) ^ *(int *)Data;
	v3 = *((int *)CipherKey + 3) ^ *((int *)Data + 2);
	b0_4 = *((int *)CipherKey + 2) ^ *((int *)Data + 1);
	v5 = *((int *)CipherKey + 4) ^ *((int *)Data + 3);
	b1_4 = *((int *)CipherKey + 6) ^ ft_tab[0][(unsigned __int8)b0_4] ^ ft_tab[3][(unsigned int)v2 >> 24] ^ ft_tab[1][BYTE1(v3)] ^ ft_tab[2][(unsigned __int8)((unsigned int)v5 >> 16)];
	v7 = *((int *)CipherKey + 5) ^ ft_tab[0][(unsigned __int8)v2] ^ ft_tab[1][BYTE1(b0_4)] ^ ft_tab[2][(unsigned __int8)((unsigned int)v3 >> 16)] ^ ft_tab[3][(unsigned int)v5 >> 24];
	v8 = *((int *)CipherKey + 7) ^ ft_tab[0][(unsigned __int8)v3] ^ ft_tab[2][(unsigned __int8)((unsigned int)v2 >> 16)] ^ ft_tab[3][(unsigned int)b0_4 >> 24] ^ ft_tab[1][BYTE1(v5)];
	v9 = *((int *)CipherKey + 8) ^ ft_tab[0][(unsigned __int8)v5] ^ ft_tab[1][BYTE1(v2)] ^ ft_tab[2][(unsigned __int8)((unsigned int)b0_4 >> 16)] ^ ft_tab[3][(unsigned int)v3 >> 24];
	b0 = *((int *)CipherKey + 9) ^ ft_tab[0][(unsigned __int8)v7] ^ ft_tab[1][BYTE1(b1_4)] ^ ft_tab[2][(unsigned __int8)(v8 >> 16)] ^ ft_tab[3][v9 >> 24];
	v11 = *((int *)CipherKey + 11) ^ ft_tab[0][(unsigned __int8)v8] ^ ft_tab[2][(unsigned __int8)(v7 >> 16)] ^ ft_tab[3][b1_4 >> 24] ^ ft_tab[1][BYTE1(v9)];
	v12 = *((int *)CipherKey + 10) ^ ft_tab[0][(unsigned __int8)b1_4] ^ ft_tab[3][v7 >> 24] ^ ft_tab[1][BYTE1(v8)] ^ ft_tab[2][(unsigned __int8)(v9 >> 16)];
	v13 = *((int *)CipherKey + 12) ^ ft_tab[0][(unsigned __int8)v9] ^ ft_tab[1][BYTE1(v7)] ^ ft_tab[2][(unsigned __int8)(b1_4 >> 16)] ^ ft_tab[3][v8 >> 24];
	b1 = *((int *)CipherKey + 13) ^ ft_tab[0][(unsigned __int8)b0] ^ ft_tab[1][BYTE1(v12)] ^ ft_tab[2][(unsigned __int8)(v11 >> 16)] ^ ft_tab[3][v13 >> 24];
	v15 = *((int *)CipherKey + 14) ^ ft_tab[0][(unsigned __int8)v12] ^ ft_tab[3][b0 >> 24] ^ ft_tab[1][BYTE1(v11)] ^ ft_tab[2][(unsigned __int8)(v13 >> 16)];
	v16 = v15;
	v17 = *((int *)CipherKey + 15) ^ ft_tab[0][(unsigned __int8)v11] ^ ft_tab[2][(unsigned __int8)(b0 >> 16)] ^ ft_tab[3][v12 >> 24] ^ ft_tab[1][BYTE1(v13)];
	v18 = *((int *)CipherKey + 16) ^ ft_tab[0][(unsigned __int8)v13] ^ ft_tab[1][BYTE1(b0)] ^ ft_tab[2][(unsigned __int8)(v12 >> 16)] ^ ft_tab[3][v11 >> 24];
	v19 = *((int *)CipherKey + 17) ^ ft_tab[0][(unsigned __int8)b1] ^ ft_tab[1][BYTE1(v16)] ^ ft_tab[2][(unsigned __int8)(v17 >> 16)] ^ ft_tab[3][v18 >> 24];
	v20 = *((int *)CipherKey + 18) ^ ft_tab[0][(unsigned __int8)v15] ^ ft_tab[3][b1 >> 24] ^ ft_tab[1][BYTE1(v17)] ^ ft_tab[2][(unsigned __int8)(v18 >> 16)];
	v21 = v20;
	v22 = *((int *)CipherKey + 19) ^ ft_tab[0][(unsigned __int8)v17] ^ ft_tab[2][(unsigned __int8)(b1 >> 16)] ^ ft_tab[3][v16 >> 24] ^ ft_tab[1][BYTE1(v18)];
	v23 = *((int *)CipherKey + 20) ^ ft_tab[0][(unsigned __int8)v18] ^ ft_tab[1][BYTE1(b1)] ^ ft_tab[2][(unsigned __int8)(v16 >> 16)] ^ ft_tab[3][v17 >> 24];
	v24 = *((int *)CipherKey + 21) ^ ft_tab[0][(unsigned __int8)v19] ^ ft_tab[1][BYTE1(v21)] ^ ft_tab[2][(unsigned __int8)(v22 >> 16)] ^ ft_tab[3][v23 >> 24];
	v25 = *((int *)CipherKey + 22) ^ ft_tab[0][(unsigned __int8)v20] ^ ft_tab[3][v19 >> 24] ^ ft_tab[1][BYTE1(v22)] ^ ft_tab[2][(unsigned __int8)(v23 >> 16)];
	v26 = *((int *)CipherKey + 23) ^ ft_tab[0][(unsigned __int8)v22] ^ ft_tab[2][(unsigned __int8)(v19 >> 16)] ^ ft_tab[3][v21 >> 24] ^ ft_tab[1][BYTE1(v23)];
	v27 = *((int *)CipherKey + 24) ^ ft_tab[0][(unsigned __int8)v23] ^ ft_tab[1][BYTE1(v19)] ^ ft_tab[2][(unsigned __int8)(v21 >> 16)] ^ ft_tab[3][v22 >> 24];
	v28 = *((int *)CipherKey + 25) ^ ft_tab[0][(unsigned __int8)v24] ^ ft_tab[1][BYTE1(v25)] ^ ft_tab[2][(unsigned __int8)(v26 >> 16)] ^ ft_tab[3][v27 >> 24];
	v29 = *((int *)CipherKey + 26) ^ ft_tab[0][(unsigned __int8)v25] ^ ft_tab[3][v24 >> 24] ^ ft_tab[1][BYTE1(v26)] ^ ft_tab[2][(unsigned __int8)(v27 >> 16)];
	v30 = *((int *)CipherKey + 27) ^ ft_tab[0][(unsigned __int8)v26] ^ ft_tab[2][(unsigned __int8)(v24 >> 16)] ^ ft_tab[3][v25 >> 24] ^ ft_tab[1][BYTE1(v27)];
	v31 = *((int *)CipherKey + 28) ^ ft_tab[0][(unsigned __int8)v27] ^ ft_tab[1][BYTE1(v24)] ^ ft_tab[2][(unsigned __int8)(v25 >> 16)] ^ ft_tab[3][v26 >> 24];
	v32 = *((int *)CipherKey + 29) ^ ft_tab[0][(unsigned __int8)v28] ^ ft_tab[1][BYTE1(v29)] ^ ft_tab[2][(unsigned __int8)(v30 >> 16)] ^ ft_tab[3][v31 >> 24];
	v33 = *((int *)CipherKey + 30) ^ ft_tab[0][(unsigned __int8)v29] ^ ft_tab[3][v28 >> 24] ^ ft_tab[1][BYTE1(v30)] ^ ft_tab[2][(unsigned __int8)(v31 >> 16)];
	v34 = *((int *)CipherKey + 31) ^ ft_tab[0][(unsigned __int8)v30] ^ ft_tab[2][(unsigned __int8)(v28 >> 16)] ^ ft_tab[3][v29 >> 24] ^ ft_tab[1][BYTE1(v31)];
	v35 = *((int *)CipherKey + 32) ^ ft_tab[0][(unsigned __int8)v31] ^ ft_tab[1][BYTE1(v28)] ^ ft_tab[2][(unsigned __int8)(v29 >> 16)] ^ ft_tab[3][v30 >> 24];
	v36 = *((int *)CipherKey + 33) ^ ft_tab[0][(unsigned __int8)v32] ^ ft_tab[1][BYTE1(v33)] ^ ft_tab[2][(unsigned __int8)(v34 >> 16)] ^ ft_tab[3][v35 >> 24];
	v37 = *((int *)CipherKey + 34) ^ ft_tab[0][(unsigned __int8)v33] ^ ft_tab[3][v32 >> 24] ^ ft_tab[1][BYTE1(v34)] ^ ft_tab[2][(unsigned __int8)(v35 >> 16)];
	v38 = *((int *)CipherKey + 35) ^ ft_tab[0][(unsigned __int8)v34] ^ ft_tab[2][(unsigned __int8)(v32 >> 16)] ^ ft_tab[3][v33 >> 24] ^ ft_tab[1][BYTE1(v35)];
	v39 = *((int *)CipherKey + 36) ^ ft_tab[0][(unsigned __int8)v35] ^ ft_tab[1][BYTE1(v32)] ^ ft_tab[2][(unsigned __int8)(v33 >> 16)] ^ ft_tab[3][v34 >> 24];
	v40 = *((int *)CipherKey + 37) ^ ft_tab[0][(unsigned __int8)v36] ^ ft_tab[1][BYTE1(v37)] ^ ft_tab[2][(unsigned __int8)(v38 >> 16)] ^ ft_tab[3][v39 >> 24];
	v41 = *((int *)CipherKey + 38) ^ ft_tab[0][(unsigned __int8)v37] ^ ft_tab[3][v36 >> 24] ^ ft_tab[1][BYTE1(v38)] ^ ft_tab[2][(unsigned __int8)(v39 >> 16)];
	v42 = *((int *)CipherKey + 39) ^ ft_tab[0][(unsigned __int8)v38] ^ ft_tab[2][(unsigned __int8)(v36 >> 16)] ^ ft_tab[3][v37 >> 24] ^ ft_tab[1][BYTE1(v39)];
	v43 = *((int *)CipherKey + 40) ^ ft_tab[0][(unsigned __int8)v39] ^ ft_tab[1][BYTE1(v36)] ^ ft_tab[2][(unsigned __int8)(v37 >> 16)] ^ ft_tab[3][v38 >> 24];
	v44 = *((int *)CipherKey + 41) ^ ft_tab[0][(unsigned __int8)v40] ^ ft_tab[1][BYTE1(v41)] ^ ft_tab[2][(unsigned __int8)(v42 >> 16)] ^ ft_tab[3][v43 >> 24];
	v45 = *((int *)CipherKey + 42) ^ ft_tab[0][(unsigned __int8)v41] ^ ft_tab[3][v40 >> 24] ^ ft_tab[1][BYTE1(v42)] ^ ft_tab[2][(unsigned __int8)(v43 >> 16)];
	v46 = *((int *)CipherKey + 43) ^ ft_tab[0][(unsigned __int8)v42] ^ ft_tab[2][(unsigned __int8)(v40 >> 16)] ^ ft_tab[3][v41 >> 24] ^ ft_tab[1][BYTE1(v43)];
	v47 = *((int *)CipherKey + 44) ^ ft_tab[0][(unsigned __int8)v43] ^ ft_tab[1][BYTE1(v40)] ^ ft_tab[2][(unsigned __int8)(v41 >> 16)] ^ ft_tab[3][v42 >> 24];
	v48 = *((int *)CipherKey + 45) ^ ft_tab[0][(unsigned __int8)v44] ^ ft_tab[1][BYTE1(v45)] ^ ft_tab[2][(unsigned __int8)(v46 >> 16)] ^ ft_tab[3][v47 >> 24];
	v49 = *((int *)CipherKey + 47) ^ ft_tab[0][(unsigned __int8)v46] ^ ft_tab[2][(unsigned __int8)(v44 >> 16)] ^ ft_tab[3][v45 >> 24] ^ ft_tab[1][BYTE1(v47)];
	v50 = *((int *)CipherKey + 46) ^ ft_tab[0][(unsigned __int8)v45] ^ ft_tab[3][v44 >> 24] ^ ft_tab[1][BYTE1(v46)] ^ ft_tab[2][(unsigned __int8)(v47 >> 16)];
	v51 = *((int *)CipherKey + 48) ^ ft_tab[0][(unsigned __int8)v47] ^ ft_tab[1][BYTE1(v44)] ^ ft_tab[2][(unsigned __int8)(v45 >> 16)] ^ ft_tab[3][v46 >> 24];
	v52 = *((int *)CipherKey + 49) ^ ft_tab[0][(unsigned __int8)v48] ^ ft_tab[1][BYTE1(v50)] ^ ft_tab[2][(unsigned __int8)(v49 >> 16)] ^ ft_tab[3][v51 >> 24];
	v53 = *((int *)CipherKey + 50) ^ ft_tab[0][(unsigned __int8)v50] ^ ft_tab[3][v48 >> 24] ^ ft_tab[1][BYTE1(v49)] ^ ft_tab[2][(unsigned __int8)(v51 >> 16)];
	v54 = *((int *)CipherKey + 51) ^ ft_tab[0][(unsigned __int8)v49] ^ ft_tab[2][(unsigned __int8)(v48 >> 16)] ^ ft_tab[3][v50 >> 24] ^ ft_tab[1][BYTE1(v51)];
	v55 = *((int *)CipherKey + 52) ^ ft_tab[0][(unsigned __int8)v51] ^ ft_tab[1][BYTE1(v48)] ^ ft_tab[2][(unsigned __int8)(v50 >> 16)] ^ ft_tab[3][v49 >> 24];
	v56 = *((int *)CipherKey + 53) ^ ft_tab[0][(unsigned __int8)v52] ^ ft_tab[1][BYTE1(v53)] ^ ft_tab[2][(unsigned __int8)(v54 >> 16)] ^ ft_tab[3][v55 >> 24];
	v57 = *((int *)CipherKey + 54) ^ ft_tab[0][(unsigned __int8)v53] ^ ft_tab[3][v52 >> 24] ^ ft_tab[1][BYTE1(v54)] ^ ft_tab[2][(unsigned __int8)(v55 >> 16)];
	v58 = *((int *)CipherKey + 55) ^ ft_tab[0][(unsigned __int8)v54] ^ ft_tab[2][(unsigned __int8)(v52 >> 16)] ^ ft_tab[3][v53 >> 24] ^ ft_tab[1][BYTE1(v55)];
	v59 = *((int *)CipherKey + 56) ^ ft_tab[0][(unsigned __int8)v55] ^ ft_tab[1][BYTE1(v52)] ^ ft_tab[2][(unsigned __int8)(v53 >> 16)] ^ ft_tab[3][v54 >> 24];
	v60 = *((int *)CipherKey + 58) ^ fl_tab[0][(unsigned __int8)v57] ^ fl_tab[3][v56 >> 24] ^ fl_tab[1][BYTE1(v58)] ^ fl_tab[2][(unsigned __int8)(v59 >> 16)];
	v61 = *((int *)CipherKey + 57) ^ fl_tab[0][(unsigned __int8)v56] ^ fl_tab[1][BYTE1(v57)] ^ fl_tab[2][(unsigned __int8)(v58 >> 16)] ^ fl_tab[3][v59 >> 24];
	v62 = *((int *)CipherKey + 59) ^ fl_tab[0][(unsigned __int8)v58] ^ fl_tab[2][(unsigned __int8)(v56 >> 16)] ^ fl_tab[3][v57 >> 24] ^ fl_tab[1][BYTE1(v59)];
	v63 = *((int *)CipherKey + 60) ^ fl_tab[0][(unsigned __int8)v59] ^ fl_tab[1][BYTE1(v56)] ^ fl_tab[2][(unsigned __int8)(v57 >> 16)] ^ fl_tab[3][v58 >> 24];
	*((int *)Data + 2) = v62;
	*(int *)Data = v61;
	*((int *)Data + 1) = v60;
	*((int *)Data + 3) = v63;
}

void AESCipher::AES_DecInit(AES_ALG_INFO * AlgInfo, unsigned int * pdwKey)
{
	AlgInfo->BufLen = 0;
	if (pdwKey)
	{
		for (int i = 0; i < 4; ++i)
			((int*)AlgInfo->ChainVar)[i] = *pdwKey;
	}
	else
		memcpy(&AlgInfo, bDefaultAESKeyValue, 16);
}

void AESCipher::AES_DecKeySchedule(unsigned int * UserKey, AES_ALG_INFO * AlgInfo)
{
	unsigned int v2; // ebx@2
	unsigned int v3; // ecx@2
	unsigned int v4; // eax@2
	int v5; // ST30_4@2
	unsigned __int32 v6; // ebx@2
	unsigned __int32 v7; // ebx@2
	unsigned __int32 v8; // eax@2
	unsigned int v9; // ebx@2
	unsigned int v10; // ecx@2
	unsigned int v11; // eax@2
	int v12; // ST18_4@2
	unsigned __int32 v13; // ebx@2
	unsigned __int32 v14; // ebx@2
	unsigned __int32 v15; // eax@2
	unsigned int v16; // ebx@2
	unsigned int v17; // ecx@2
	unsigned int v18; // eax@2
	int v19; // ST00_4@2
	unsigned __int32 v20; // ebx@2
	unsigned __int32 v21; // ebx@2
	unsigned __int32 v22; // eax@2
	unsigned int v23; // ebx@2
	unsigned int v24; // ecx@2
	unsigned int v25; // eax@2
	int v26; // ST28_4@2
	unsigned __int32 v27; // ebx@2
	unsigned __int32 v28; // ebx@2
	unsigned __int32 v29; // eax@2
	int v30; // ebx@2
	int v31; // eax@2
	bool v32; // zf@2
	unsigned int t_key[64]; // [sp+Ch] [bp-110h]@1
	int v34; // [sp+10Ch] [bp-10h]@1
	unsigned int *v35; // [sp+110h] [bp-Ch]@1
	int v36; // [sp+118h] [bp-4h]@1
	unsigned int t; // [sp+124h] [bp+8h]@2
	int ta; // [sp+124h] [bp+8h]@2
	int tb; // [sp+124h] [bp+8h]@2
	unsigned int tc; // [sp+124h] [bp+8h]@2
	int td; // [sp+124h] [bp+8h]@2
	unsigned int te; // [sp+124h] [bp+8h]@2
	int tf; // [sp+124h] [bp+8h]@2

	AlgInfo->RoundKey[0] = 8;
	RIJNDAEL_KeySchedule(UserKey, t_key);
	AlgInfo->RoundKey[1] = t_key[56];
	AlgInfo->RoundKey[2] = t_key[57];
	AlgInfo->RoundKey[3] = t_key[58];
	AlgInfo->RoundKey[4] = t_key[59];
	v36 = (int)&AlgInfo->RoundKey[7];
	v35 = &t_key[53];
	v34 = 13;
	do
	{
		t = *(v35 - 1);
		v2 = 2 * (t & 0xFF7F7F7F) ^ 27 * ((t >> 7) & 0x1010101);
		v3 = 2 * (v2 & 0xFF7F7F7F) ^ 27 * ((v2 >> 7) & 0x1010101);
		v4 = 2 * ((2 * (v2 & 0xFF7F7F7F) ^ 27 * ((v2 >> 7) & 0x1010101)) & 0xFF7F7F7F) ^ 27
			* (((2 * (v2 & 0xFF7F7F7F) ^ 27 * ((v2 >> 7) & 0x1010101)) >> 7) & 0x1010101);
		ta = v4 ^ t;
		*(int *)(v36 - 8) = v2 ^ v3 ^ v4;
		v5 = v2 ^ ta;
		v6 = _lrotr(v3 ^ ta, 16);
		v7 = _lrotr(v5, 8) ^ v6;
		v8 = _lrotr(ta, 24);
		*(int *)(v36 - 8) ^= v8 ^ v7;
		v9 = 2 * (*v35 & 0xFF7F7F7F) ^ 27 * ((*v35 >> 7) & 0x1010101);
		v10 = 2 * (v9 & 0xFF7F7F7F) ^ 27 * ((v9 >> 7) & 0x1010101);
		v11 = 2 * (v10 & 0xFF7F7F7F) ^ 27 * ((v10 >> 7) & 0x1010101);
		tb = v11 ^ *v35;
		*(int *)(v36 - 4) = v9 ^ v10 ^ v11;
		v12 = v9 ^ tb;
		v13 = _lrotr(v10 ^ tb, 16);
		v14 = _lrotr(v12, 8) ^ v13;
		v15 = _lrotr(tb, 24);
		*(int *)(v36 - 4) ^= v15 ^ v14;
		tc = v35[1];
		v16 = 2 * (tc & 0xFF7F7F7F) ^ 27 * ((tc >> 7) & 0x1010101);
		v17 = 2 * (v16 & 0xFF7F7F7F) ^ 27 * ((v16 >> 7) & 0x1010101);
		v18 = 2 * ((2 * (v16 & 0xFF7F7F7F) ^ 27 * ((v16 >> 7) & 0x1010101)) & 0xFF7F7F7F) ^ 27
			* (((2 * (v16 & 0xFF7F7F7F) ^ 27 * ((v16 >> 7) & 0x1010101)) >> 7) & 0x1010101);
		td = v18 ^ tc;
		*(int *)v36 = v16 ^ v17 ^ v18;
		v19 = v16 ^ td;
		v20 = _lrotr(v17 ^ td, 16);
		v21 = _lrotr(v19, 8) ^ v20;
		v22 = _lrotr(td, 24);
		*(int *)v36 ^= v22 ^ v21;
		te = v35[2];
		v23 = 2 * (te & 0xFF7F7F7F) ^ 27 * ((te >> 7) & 0x1010101);
		v24 = 2 * (v23 & 0xFF7F7F7F) ^ 27 * ((v23 >> 7) & 0x1010101);
		v25 = 2 * ((2 * (v23 & 0xFF7F7F7F) ^ 27 * ((v23 >> 7) & 0x1010101)) & 0xFF7F7F7F) ^ 27
			* (((2 * (v23 & 0xFF7F7F7F) ^ 27 * ((v23 >> 7) & 0x1010101)) >> 7) & 0x1010101);
		tf = v25 ^ te;
		*(int *)(v36 + 4) = v23 ^ v24 ^ v25;
		v26 = v23 ^ tf;
		v27 = _lrotr(v24 ^ tf, 16);
		v28 = _lrotr(v26, 8) ^ v27;
		v29 = _lrotr(tf, 24);
		v35 -= 4;
		v30 = v29 ^ v28;
		v31 = v36;
		*(int *)(v31 + 4) ^= v30;
		v32 = v34-- == 1;
		v36 = v31 + 16;
	} while (!v32);
	AlgInfo->RoundKey[57] = t_key[0];
	AlgInfo->RoundKey[58] = t_key[1];
	AlgInfo->RoundKey[59] = t_key[2];
	AlgInfo->RoundKey[60] = t_key[3];
}

void AESCipher::AES_Decrypt(void * CipherKey, char * Data)
{
	int v2; // esi@1
	int v3; // edx@1
	int v4; // edi@1
	__int16 b0_8; // ST28_2@1
	int b0; // ST20_4@1
	unsigned int v7; // ecx@1
	unsigned int b1_4; // ST14_4@1
	unsigned int b1_8; // ST18_4@1
	unsigned int v10; // esi@1
	unsigned int b0_4; // ST24_4@1
	unsigned int v12; // ST28_4@1
	unsigned int v13; // edx@1
	unsigned int v14; // esi@1
	unsigned int v15; // ecx@1
	unsigned int v16; // ST14_4@1
	unsigned int v17; // ST18_4@1
	unsigned int v18; // esi@1
	unsigned int v19; // edx@1
	unsigned int v20; // ST24_4@1
	unsigned int v21; // ST28_4@1
	unsigned int v22; // esi@1
	unsigned int v23; // ecx@1
	unsigned int v24; // ST14_4@1
	unsigned int v25; // ST18_4@1
	unsigned int v26; // esi@1
	unsigned int v27; // ST24_4@1
	unsigned int v28; // edx@1
	unsigned int v29; // ST28_4@1
	unsigned int v30; // esi@1
	unsigned int v31; // ST14_4@1
	unsigned int v32; // ST18_4@1
	unsigned int v33; // ecx@1
	unsigned int v34; // esi@1
	unsigned int v35; // edx@1
	unsigned int v36; // ST24_4@1
	unsigned int v37; // ST28_4@1
	unsigned int v38; // esi@1
	unsigned int v39; // ecx@1
	unsigned int v40; // ST14_4@1
	unsigned int v41; // ST18_4@1
	unsigned int v42; // esi@1
	unsigned int v43; // edx@1
	unsigned int v44; // ST24_4@1
	unsigned int v45; // ST28_4@1
	unsigned int v46; // esi@1
	unsigned int v47; // ST14_4@1
	unsigned int v48; // ecx@1
	unsigned int v49; // ST18_4@1
	unsigned int b1_12; // ST1C_4@1
	unsigned int v51; // ST24_4@1
	unsigned int v52; // ST28_4@1
	unsigned int v53; // edx@1
	unsigned int b0_12; // ST2C_4@1
	unsigned int v55; // ecx@1
	unsigned int v56; // ST14_4@1
	unsigned int v57; // ST18_4@1
	unsigned int v58; // ebx@1
	unsigned int v59; // ST24_4@1
	unsigned int v60; // ST28_4@1
	unsigned int v61; // edx@1

	v2 = *((int *)CipherKey + 3) ^ *((int *)Data + 2);
	v3 = *((int *)CipherKey + 2) ^ *((int *)Data + 1);
	v4 = *((int *)CipherKey + 4) ^ *((int *)Data + 3);
	b0_8 = *((short *)CipherKey + 6) ^ *((short *)Data + 4);
	b0 = *((int *)CipherKey + 1) ^ *(int *)Data;
	v7 = *((int *)CipherKey + 5) ^ it_tab[0][(unsigned __int8)b0] ^ it_tab[3][(unsigned int)v3 >> 24] ^ it_tab[2][(unsigned __int8)((unsigned int)v2 >> 16)] ^ it_tab[1][BYTE1(v4)];
	b1_4 = *((int *)CipherKey + 6) ^ it_tab[0][(unsigned __int8)v3] ^ it_tab[1][BYTE1(b0)] ^ it_tab[3][(unsigned int)v2 >> 24] ^ it_tab[2][(unsigned __int8)((unsigned int)v4 >> 16)];
	b1_8 = *((int *)CipherKey + 7) ^ it_tab[0][(unsigned __int8)b0_8] ^ it_tab[2][(unsigned __int8)((unsigned int)b0 >> 16)] ^ it_tab[1][BYTE1(v3)] ^ it_tab[3][(unsigned int)v4 >> 24];
	v10 = *((int *)CipherKey + 8) ^ it_tab[0][(unsigned __int8)v4] ^ it_tab[3][(unsigned int)b0 >> 24] ^ it_tab[2][(unsigned __int8)((unsigned int)v3 >> 16)] ^ it_tab[1][HIBYTE(b0_8)];
	b0_4 = *((int *)CipherKey + 10) ^ it_tab[0][(unsigned __int8)b1_4] ^ it_tab[1][BYTE1(v7)] ^ it_tab[3][b1_8 >> 24] ^ it_tab[2][(unsigned __int8)(v10 >> 16)];
	v12 = *((int *)CipherKey + 11) ^ it_tab[0][(unsigned __int8)b1_8] ^ it_tab[2][(unsigned __int8)(v7 >> 16)] ^ it_tab[1][BYTE1(b1_4)] ^ it_tab[3][v10 >> 24];
	v13 = *((int *)CipherKey + 9) ^ it_tab[0][(unsigned __int8)v7] ^ it_tab[3][b1_4 >> 24] ^ it_tab[2][(unsigned __int8)(b1_8 >> 16)] ^ it_tab[1][BYTE1(v10)];
	v14 = *((int *)CipherKey + 12) ^ it_tab[0][(unsigned __int8)v10] ^ it_tab[3][v7 >> 24] ^ it_tab[2][(unsigned __int8)(b1_4 >> 16)] ^ it_tab[1][BYTE1(b1_8)];
	v15 = *((int *)CipherKey + 13) ^ it_tab[0][(unsigned __int8)v13] ^ it_tab[3][b0_4 >> 24] ^ it_tab[2][(unsigned __int8)(v12 >> 16)] ^ it_tab[1][BYTE1(v14)];
	v16 = *((int *)CipherKey + 14) ^ it_tab[0][(unsigned __int8)b0_4] ^ it_tab[1][BYTE1(v13)] ^ it_tab[3][v12 >> 24] ^ it_tab[2][(unsigned __int8)(v14 >> 16)];
	v17 = *((int *)CipherKey + 15) ^ it_tab[0][(unsigned __int8)v12] ^ it_tab[2][(unsigned __int8)(v13 >> 16)] ^ it_tab[1][BYTE1(b0_4)] ^ it_tab[3][v14 >> 24];
	v18 = *((int *)CipherKey + 16) ^ it_tab[0][(unsigned __int8)v14] ^ it_tab[3][v13 >> 24] ^ it_tab[2][(unsigned __int8)(b0_4 >> 16)] ^ it_tab[1][BYTE1(v12)];
	v19 = *((int *)CipherKey + 17) ^ it_tab[0][(unsigned __int8)v15] ^ it_tab[3][v16 >> 24] ^ it_tab[2][(unsigned __int8)(v17 >> 16)] ^ it_tab[1][BYTE1(v18)];
	v20 = *((int *)CipherKey + 18) ^ it_tab[0][(unsigned __int8)v16] ^ it_tab[1][BYTE1(v15)] ^ it_tab[3][v17 >> 24] ^ it_tab[2][(unsigned __int8)(v18 >> 16)];
	v21 = *((int *)CipherKey + 19) ^ it_tab[0][(unsigned __int8)v17] ^ it_tab[2][(unsigned __int8)(v15 >> 16)] ^ it_tab[1][BYTE1(v16)] ^ it_tab[3][v18 >> 24];
	v22 = *((int *)CipherKey + 20) ^ it_tab[0][(unsigned __int8)v18] ^ it_tab[3][v15 >> 24] ^ it_tab[2][(unsigned __int8)(v16 >> 16)] ^ it_tab[1][BYTE1(v17)];
	v23 = *((int *)CipherKey + 21) ^ it_tab[0][(unsigned __int8)v19] ^ it_tab[3][v20 >> 24] ^ it_tab[2][(unsigned __int8)(v21 >> 16)] ^ it_tab[1][BYTE1(v22)];
	v24 = *((int *)CipherKey + 22) ^ it_tab[0][(unsigned __int8)v20] ^ it_tab[1][BYTE1(v19)] ^ it_tab[3][v21 >> 24] ^ it_tab[2][(unsigned __int8)(v22 >> 16)];
	v25 = *((int *)CipherKey + 23) ^ it_tab[0][(unsigned __int8)v21] ^ it_tab[2][(unsigned __int8)(v19 >> 16)] ^ it_tab[1][BYTE1(v20)] ^ it_tab[3][v22 >> 24];
	v26 = *((int *)CipherKey + 24) ^ it_tab[0][(unsigned __int8)v22] ^ it_tab[3][v19 >> 24] ^ it_tab[2][(unsigned __int8)(v20 >> 16)] ^ it_tab[1][BYTE1(v21)];
	v27 = *((int *)CipherKey + 26) ^ it_tab[0][(unsigned __int8)v24] ^ it_tab[1][BYTE1(v23)] ^ it_tab[3][v25 >> 24] ^ it_tab[2][(unsigned __int8)(v26 >> 16)];
	v28 = *((int *)CipherKey + 25) ^ it_tab[0][(unsigned __int8)v23] ^ it_tab[3][v24 >> 24] ^ it_tab[2][(unsigned __int8)(v25 >> 16)] ^ it_tab[1][BYTE1(v26)];
	v29 = *((int *)CipherKey + 27) ^ it_tab[0][(unsigned __int8)v25] ^ it_tab[2][(unsigned __int8)(v23 >> 16)] ^ it_tab[1][BYTE1(v24)] ^ it_tab[3][v26 >> 24];
	v30 = *((int *)CipherKey + 28) ^ it_tab[0][(unsigned __int8)v26] ^ it_tab[3][v23 >> 24] ^ it_tab[2][(unsigned __int8)(v24 >> 16)] ^ it_tab[1][BYTE1(v25)];
	v31 = *((int *)CipherKey + 30) ^ it_tab[0][(unsigned __int8)v27] ^ it_tab[1][BYTE1(v28)] ^ it_tab[3][v29 >> 24] ^ it_tab[2][(unsigned __int8)(v30 >> 16)];
	v32 = *((int *)CipherKey + 31) ^ it_tab[0][(unsigned __int8)v29] ^ it_tab[2][(unsigned __int8)(v28 >> 16)] ^ it_tab[1][BYTE1(v27)] ^ it_tab[3][v30 >> 24];
	v33 = *((int *)CipherKey + 29) ^ it_tab[0][(unsigned __int8)v28] ^ it_tab[3][v27 >> 24] ^ it_tab[2][(unsigned __int8)(v29 >> 16)] ^ it_tab[1][BYTE1(v30)];
	v34 = *((int *)CipherKey + 32) ^ it_tab[0][(unsigned __int8)v30] ^ it_tab[3][v28 >> 24] ^ it_tab[2][(unsigned __int8)(v27 >> 16)] ^ it_tab[1][BYTE1(v29)];
	v35 = *((int *)CipherKey + 33) ^ it_tab[0][(unsigned __int8)v33] ^ it_tab[3][v31 >> 24] ^ it_tab[2][(unsigned __int8)(v32 >> 16)] ^ it_tab[1][BYTE1(v34)];
	v36 = *((int *)CipherKey + 34) ^ it_tab[0][(unsigned __int8)v31] ^ it_tab[1][BYTE1(v33)] ^ it_tab[3][v32 >> 24] ^ it_tab[2][(unsigned __int8)(v34 >> 16)];
	v37 = *((int *)CipherKey + 35) ^ it_tab[0][(unsigned __int8)v32] ^ it_tab[2][(unsigned __int8)(v33 >> 16)] ^ it_tab[1][BYTE1(v31)] ^ it_tab[3][v34 >> 24];
	v38 = *((int *)CipherKey + 36) ^ it_tab[0][(unsigned __int8)v34] ^ it_tab[3][v33 >> 24] ^ it_tab[2][(unsigned __int8)(v31 >> 16)] ^ it_tab[1][BYTE1(v32)];
	v39 = *((int *)CipherKey + 37) ^ it_tab[0][(unsigned __int8)v35] ^ it_tab[3][v36 >> 24] ^ it_tab[2][(unsigned __int8)(v37 >> 16)] ^ it_tab[1][BYTE1(v38)];
	v40 = *((int *)CipherKey + 38) ^ it_tab[0][(unsigned __int8)v36] ^ it_tab[1][BYTE1(v35)] ^ it_tab[3][v37 >> 24] ^ it_tab[2][(unsigned __int8)(v38 >> 16)];
	v41 = *((int *)CipherKey + 39) ^ it_tab[0][(unsigned __int8)v37] ^ it_tab[2][(unsigned __int8)(v35 >> 16)] ^ it_tab[1][BYTE1(v36)] ^ it_tab[3][v38 >> 24];
	v42 = *((int *)CipherKey + 40) ^ it_tab[0][(unsigned __int8)v38] ^ it_tab[3][v35 >> 24] ^ it_tab[2][(unsigned __int8)(v36 >> 16)] ^ it_tab[1][BYTE1(v37)];
	v43 = *((int *)CipherKey + 41) ^ it_tab[0][(unsigned __int8)v39] ^ it_tab[3][v40 >> 24] ^ it_tab[2][(unsigned __int8)(v41 >> 16)] ^ it_tab[1][BYTE1(v42)];
	v44 = *((int *)CipherKey + 42) ^ it_tab[0][(unsigned __int8)v40] ^ it_tab[1][BYTE1(v39)] ^ it_tab[3][v41 >> 24] ^ it_tab[2][(unsigned __int8)(v42 >> 16)];
	v45 = *((int *)CipherKey + 43) ^ it_tab[0][(unsigned __int8)v41] ^ it_tab[2][(unsigned __int8)(v39 >> 16)] ^ it_tab[1][BYTE1(v40)] ^ it_tab[3][v42 >> 24];
	v46 = *((int *)CipherKey + 44) ^ it_tab[0][(unsigned __int8)v42] ^ it_tab[3][v39 >> 24] ^ it_tab[2][(unsigned __int8)(v40 >> 16)] ^ it_tab[1][BYTE1(v41)];
	v47 = *((int *)CipherKey + 46) ^ it_tab[0][(unsigned __int8)v44] ^ it_tab[1][BYTE1(v43)] ^ it_tab[3][v45 >> 24] ^ it_tab[2][(unsigned __int8)(v46 >> 16)];
	v48 = *((int *)CipherKey + 45) ^ it_tab[0][(unsigned __int8)v43] ^ it_tab[3][v44 >> 24] ^ it_tab[2][(unsigned __int8)(v45 >> 16)] ^ it_tab[1][BYTE1(v46)];
	v49 = *((int *)CipherKey + 47) ^ it_tab[0][(unsigned __int8)v45] ^ it_tab[2][(unsigned __int8)(v43 >> 16)] ^ it_tab[1][BYTE1(v44)] ^ it_tab[3][v46 >> 24];
	b1_12 = *((int *)CipherKey + 48) ^ it_tab[0][(unsigned __int8)v46] ^ it_tab[3][v43 >> 24] ^ it_tab[2][(unsigned __int8)(v44 >> 16)] ^ it_tab[1][BYTE1(v45)];
	v51 = *((int *)CipherKey + 50) ^ it_tab[0][(unsigned __int8)v47] ^ it_tab[1][BYTE1(v48)] ^ it_tab[3][v49 >> 24] ^ it_tab[2][(unsigned __int8)(b1_12 >> 16)];
	v52 = *((int *)CipherKey + 51) ^ it_tab[0][(unsigned __int8)v49] ^ it_tab[2][(unsigned __int8)(v48 >> 16)] ^ it_tab[1][BYTE1(v47)] ^ it_tab[3][b1_12 >> 24];
	v53 = *((int *)CipherKey + 49) ^ it_tab[0][(unsigned __int8)v48] ^ it_tab[3][v47 >> 24] ^ it_tab[2][(unsigned __int8)(v49 >> 16)] ^ it_tab[1][BYTE1(b1_12)];
	b0_12 = *((int *)CipherKey + 52) ^ it_tab[0][(unsigned __int8)b1_12] ^ it_tab[3][v48 >> 24] ^ it_tab[2][(unsigned __int8)(v47 >> 16)] ^ it_tab[1][BYTE1(v49)];
	v55 = *((int *)CipherKey + 53) ^ it_tab[0][(unsigned __int8)v53] ^ it_tab[3][v51 >> 24] ^ it_tab[2][(unsigned __int8)(v52 >> 16)] ^ it_tab[1][BYTE1(b0_12)];
	v56 = *((int *)CipherKey + 54) ^ it_tab[0][(unsigned __int8)v51] ^ it_tab[1][BYTE1(v53)] ^ it_tab[3][v52 >> 24] ^ it_tab[2][(unsigned __int8)(b0_12 >> 16)];
	v57 = *((int *)CipherKey + 55) ^ it_tab[0][(unsigned __int8)v52] ^ it_tab[2][(unsigned __int8)(v53 >> 16)] ^ it_tab[1][BYTE1(v51)] ^ it_tab[3][b0_12 >> 24];
	v58 = *((int *)CipherKey + 56) ^ it_tab[0][(unsigned __int8)b0_12] ^ it_tab[3][v53 >> 24] ^ it_tab[2][(unsigned __int8)(v51 >> 16)] ^ it_tab[1][BYTE1(v52)];
	v59 = *((int *)CipherKey + 58) ^ il_tab[0][(unsigned __int8)v56] ^ il_tab[1][BYTE1(v55)] ^ il_tab[3][v57 >> 24] ^ il_tab[2][(unsigned __int8)(v58 >> 16)];
	v60 = *((int *)CipherKey + 59) ^ il_tab[0][(unsigned __int8)v57] ^ il_tab[2][(unsigned __int8)(v55 >> 16)] ^ il_tab[1][BYTE1(v56)] ^ il_tab[3][v58 >> 24];
	v61 = *((int *)CipherKey + 60) ^ il_tab[0][(unsigned __int8)v58] ^ il_tab[3][v55 >> 24] ^ il_tab[2][(unsigned __int8)(v56 >> 16)] ^ il_tab[1][BYTE1(v57)];
	*(int *)Data = *((int *)CipherKey + 57) ^ il_tab[0][(unsigned __int8)v55] ^ il_tab[3][v56 >> 24] ^ il_tab[2][(unsigned __int8)(v57 >> 16)] ^ il_tab[1][BYTE1(v58)];
	*((int *)Data + 1) = v59;
	*((int *)Data + 2) = v60;
	*((int *)Data + 3) = v61;
}

char AESCipher::OFB_DecFinal(AES_ALG_INFO * AlgInfo, char * PlainTxt, unsigned int * PlainTxtLen)
{
	unsigned int v3; // edi@1
	AES_ALG_INFO *v4; // eax@2
	unsigned int v5; // esi@2

	v3 = AlgInfo->BufLen;
	*PlainTxtLen = v3;
	AES_Encrypt(AlgInfo->RoundKey, (char*)AlgInfo->ChainVar);
	if (v3)
	{
		v4 = AlgInfo;
		v5 = v3;
		do
		{
			v4->ChainVar[PlainTxt - (char *)AlgInfo] = v4->ChainVar[0] ^ v4->Buffer[0];
			v4 = (AES_ALG_INFO *)((char *)v4 + 1);
			--v5;
		} while (v5);
	}
	*PlainTxtLen = v3;
	return 1;
}

char AESCipher::OFB_DecUpdate(AES_ALG_INFO * AlgInfo, char * CipherTxt, unsigned int CipherTxtLen, char * PlainTxt, unsigned int * PlainTxtLen)
{
	AES_ALG_INFO *v5; // esi@1
	unsigned int v6; // ebx@1
	size_t v7; // ebx@5
	char *v8; // edi@5
	bool v9; // zf@7
	char *tmpCipherTxt = nullptr; // [sp+Ch] [bp-4h]@0
	//char memAllocate_3; // [sp+1Bh] [bp+Bh]@1
	char *CipherTxta; // [sp+1Ch] [bp+Ch]@5
	unsigned int CipherTxtLena; // [sp+20h] [bp+10h]@6

	v5 = AlgInfo;
	v6 = AlgInfo->BufLen;
	//memAllocate_3 = 0;
	*PlainTxtLen = v6 + CipherTxtLen;
	if (v6 + CipherTxtLen > 0x10)
	{
		if (CipherTxt == PlainTxt)
		{
			tmpCipherTxt = (char *)malloc(CipherTxtLen);
			memcpy(tmpCipherTxt, CipherTxt, CipherTxtLen);
			CipherTxt = tmpCipherTxt;
			//memAllocate_3 = 1;
		}
		*PlainTxtLen = v6 + CipherTxtLen;
		memcpy(&v5->Buffer[v6], CipherTxt, 16 - v6);
		CipherTxta = &CipherTxt[16 - v6];
		v7 = CipherTxtLen + v6 - 16;
		AES_Encrypt(v5->RoundKey, (char*)v5->ChainVar);
		*(int *)PlainTxt = *(int *)&v5->ChainVar[0] ^ *(int *)&v5->Buffer[0];
		v8 = PlainTxt + 16;
		*((int *)v8 - 3) = *(int *)&v5->ChainVar[4] ^ *(int *)&v5->Buffer[4];
		*((int *)v8 - 2) = *(int *)&v5->ChainVar[8] ^ *(int *)&v5->Buffer[8];
		*((int *)v8 - 1) = *(int *)&v5->ChainVar[12] ^ *(int *)&v5->Buffer[12];
		if (v7 > 0x10)
		{
			CipherTxtLena = (v7 - 1) >> 4;
			do
			{
				AES_Encrypt(v5->RoundKey, (char*)v5->ChainVar);
				*(int *)v8 = *(int *)CipherTxta ^ *(int *)&v5->ChainVar[0];
				v8 += 16;
				*((int *)v8 - 3) = *(int *)&v5->ChainVar[4] ^ *((int *)CipherTxta + 1);
				v7 -= 16;
				*((int *)v8 - 2) = *(int *)&v5->ChainVar[8] ^ *((int *)CipherTxta + 2);
				v9 = CipherTxtLena-- == 1;
				*((int *)v8 - 1) = *(int *)&v5->ChainVar[12] ^ *((int *)CipherTxta + 3);
				CipherTxta += 16;
			} while (!v9);
		}
		memcpy(v5->Buffer, CipherTxta, v7);
		v5->BufLen = v7 + (v5->BufLen & 0xF0000000);
		*PlainTxtLen -= v7;
		if (tmpCipherTxt)
			free(tmpCipherTxt);
	}
	else
	{
		memcpy(&v5->Buffer[v6], CipherTxt, CipherTxtLen);
		v5->BufLen += CipherTxtLen;
		*PlainTxtLen = 0;
	}
	return 1;
}

char AESCipher::OFB_EncFinal(AES_ALG_INFO * AlgInfo, char * CipherTxt, unsigned int * CipherTxtLen)
{
	unsigned int v3; // edi@1
	AES_ALG_INFO *v4; // eax@2
	unsigned int v5; // esi@2

	v3 = AlgInfo->BufLen;
	*CipherTxtLen = 16;
	AES_Encrypt(AlgInfo->RoundKey, (char*)AlgInfo->ChainVar);
	if (v3)
	{
		v4 = AlgInfo;
		v5 = v3;
		do
		{
			v4->ChainVar[CipherTxt - (char *)AlgInfo] = v4->ChainVar[0] ^ v4->Buffer[0];
			v4 = (AES_ALG_INFO *)((char *)v4 + 1);
			--v5;
		} while (v5);
	}
	*CipherTxtLen = v3;
	return 1;
}

char AESCipher::OFB_EncUpdate(AES_ALG_INFO * AlgInfo, char * PlainTxt, unsigned int PlainTxtLen, char * CipherTxt, unsigned int * CipherTxtLen)
{
	AES_ALG_INFO *v5; // esi@1
	unsigned int v6; // ebx@1
	unsigned int v7; // ebx@5
	char *v8; // edi@5
	bool v9; // zf@7
	char *tmpPlainTxt = nullptr; // [sp+Ch] [bp-4h]@0
	//char memAllocate_3; // [sp+1Bh] [bp+Bh]@1
	char *PlainTxta; // [sp+1Ch] [bp+Ch]@5
	unsigned int PlainTxtLena; // [sp+20h] [bp+10h]@6

	v5 = AlgInfo;
	v6 = AlgInfo->BufLen;
	//memAllocate_3 = 0;
	*CipherTxtLen = v6 + PlainTxtLen;
	if (v6 + PlainTxtLen >= 0x10)
	{
		if (PlainTxt == CipherTxt)
		{
			tmpPlainTxt = (char *)malloc(PlainTxtLen);
			memcpy(tmpPlainTxt, PlainTxt, PlainTxtLen);
			PlainTxt = tmpPlainTxt;
		}
		memcpy(&v5->Buffer[v6], PlainTxt, 16 - v6);
		PlainTxta = &PlainTxt[16 - v6];
		v7 = PlainTxtLen + v6 - 16;
		AES_Encrypt(v5->RoundKey, (char*)v5->ChainVar);
		*(int *)CipherTxt = *(int *)&v5->Buffer[0] ^ *(int *)&v5->ChainVar[0];
		v8 = CipherTxt + 16;
		*((int *)v8 - 3) = *(int *)&v5->ChainVar[4] ^ *(int *)&v5->Buffer[4];
		*((int *)v8 - 2) = *(int *)&v5->ChainVar[8] ^ *(int *)&v5->Buffer[8];
		*((int *)v8 - 1) = *(int *)&v5->ChainVar[12] ^ *(int *)&v5->Buffer[12];
		if (v7 >= 0x10)
		{
			PlainTxtLena = v7 >> 4;
			do
			{
				AES_Encrypt(v5->RoundKey, (char*)v5->ChainVar);
				*(int *)v8 = *(int *)PlainTxta ^ *(int *)&v5->ChainVar[0];
				v8 += 16;
				*((int *)v8 - 3) = *(int *)&v5->ChainVar[4] ^ *((int *)PlainTxta + 1);
				v7 -= 16;
				*((int *)v8 - 2) = *(int *)&v5->ChainVar[8] ^ *((int *)PlainTxta + 2);
				v9 = PlainTxtLena-- == 1;
				*((int *)v8 - 1) = *(int *)&v5->ChainVar[12] ^ *((int *)PlainTxta + 3);
				PlainTxta += 16;
			} while (!v9);
		}
		memcpy(v5->Buffer, PlainTxta, v7);
		v5->BufLen = v7 + (v5->BufLen & 0xF0000000);
		*CipherTxtLen -= v7;
		if (tmpPlainTxt)
			free(tmpPlainTxt);
	}
	else
	{
		memcpy(&v5->Buffer[v6], PlainTxt, PlainTxtLen);
		v5->BufLen += PlainTxtLen;
		*CipherTxtLen = 0;
	}
	return 1;
}

AESCipher::AES_ALG_INFO::AES_ALG_INFO()
{
	memset(this, 0, sizeof(AES_ALG_INFO));
}
