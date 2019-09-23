#pragma once
#include <string>

class WzArchive;

class AESCipher
{
	unsigned int 
		il_tab[4][256],
		fl_tab[4][256],
		it_tab[4][256],
		ft_tab[4][256],
		rco_tab[10];

	char isb_tab[256], sbx_tab[256], pow_tab[256], log_tab[256];
	static const char bDefaultAESKeyValue[16];
	unsigned int UserKey[32] = {
		0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
		0x00, 0xB4, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00,
		0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00
	};
	bool m_bTableGen = false;


public:

	unsigned int BasicKey = 0xE9637DB9;
	struct AES_ALG_INFO
	{
		unsigned char ChainVar[16], Buffer[16];
		unsigned int BufLen;
		unsigned int RoundKey[68];
		AES_ALG_INFO();
	};

	AESCipher();
	~AESCipher();
	void Decrypt(char *pDest, char *pSrc, int nLen, unsigned int *pdwKey);
	void Encrypt(char *pDest, char *pSrc, int nLen, unsigned int *pdwKey);

	std::string DecodeString(WzArchive *pArchive);

	void GenTables();
	void RIJNDAEL_KeySchedule(unsigned int *UserKey, unsigned int *e_key);
	void AES_EncInit(AES_ALG_INFO *AlgInfo, unsigned int *pdwKey);
	void AES_EncKeySchedule(unsigned int *UserKey, AES_ALG_INFO *AlgInfo);
	void AES_Encrypt(void *CipherKey, char *Data);

	void AES_DecInit(AES_ALG_INFO *AlgInfo, unsigned int *pdwKey); 
	void AES_DecKeySchedule(unsigned int *UserKey, AES_ALG_INFO *AlgInfo);
	void AES_Decrypt(void *CipherKey, char *Data);

	char OFB_DecFinal(AES_ALG_INFO *AlgInfo, char *PlainTxt, unsigned int *PlainTxtLen);
	char OFB_DecUpdate(AES_ALG_INFO *AlgInfo, char *CipherTxt, unsigned int CipherTxtLen, char *PlainTxt, unsigned int *PlainTxtLen);
	char OFB_EncFinal(AES_ALG_INFO *AlgInfo, char *CipherTxt, unsigned int *CipherTxtLen);
	char OFB_EncUpdate(AES_ALG_INFO *AlgInfo, char *PlainTxt, unsigned int PlainTxtLen, char *CipherTxt, unsigned int *CipherTxtLen);
};

