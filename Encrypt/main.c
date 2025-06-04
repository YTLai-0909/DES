#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
	
	/* 獲得資料數目 */  
	int num = 0;
	int i, j;
	
	printf("Input number of data : ");
	scanf("%d", &num);  // 總共有幾筆資料  
	printf("\n");
	
/*---------------------------------------------------------------------------*/
	
	for(i = 0; i < num; i++) {
		printf("Data %d :\n", i + 1);
		/* 回合金鑰產生器 */ 
		char inputKey[17] = {'\0'};
		_Bool key[64] = {0};
		_Bool roundKeys[16][48] = {0};
		char outputKey[17] = {'\0'};
	
		// 讀取金鑰 
		openFile("key.txt", inputKey, i);
		
		// 16 進位轉 2 進位 
		HexadecimalToBinary(inputKey, key);
		
		// 產生回合金鑰  
		keyGenerator(key, roundKeys);
		
		// 印出回合金鑰  
		printf("RoundKey\n");
		for(j = 0; j < 16; j++) {
			// 2 進位轉 16 進位 
			BinaryToHexadecimal(48, roundKeys[j], outputKey);
			printf("%s\n", outputKey);
		}
		printf("Key Generator Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/
	
		/* DES 加密法 ( Encrypt )*/  
		char inputP[17] = {'\0'};
		_Bool encryptPlainText[64] = {0};
		_Bool encryptCipherText[64] = {0};
		char outputC[17] = {'\0'}; 
		
		// 讀取明文 
		openFile("input.txt", inputP, i);
		
		// 16 進位轉 2 進位  
		HexadecimalToBinary(inputP, encryptPlainText);
		
		// DES 加密  
		Encrypt(encryptPlainText, roundKeys, encryptCipherText);
		
		// 2 進位轉 16 進位 
		BinaryToHexadecimal(64, encryptCipherText, outputC);
		
		// 印出加密結果 
		printf("Encrypt = %s\n", outputC);
		
		// 產生結果檔 
		writeFile("output.txt", outputC, i);
		printf("Encrypt Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/		
		
		printf("-------------------------------------------------\n");
	}
	
	system("pause");  // 讓小黑框不會因為程式結束就消失 
	
	return 0;
}
