#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
	
	/* 獲得資料數目 
		num : 記錄總共有幾筆資料  
	*/ 
	int num = 0; 
	int i, j;
	
	// 輸入資料數  
	printf("Input number of data : ");
	scanf("%d", &num); 
	printf("\n");
	
	
	/* 產生回合金鑰 + DES 加密法 */ 
	for(i = 0; i < num; i++) {
		
		printf("Data %d :\n", i + 1);
		
	/*---------------------------------------------------------------------------*/
		/* 回合金鑰產生器 
			inputKey : 存讀取的金鑰內容( 字元形式 )
			key : 金鑰( 位元形式 )
			roundKeys : 回合金鑰( 位元形式 )
			outputKey : 存回合金鑰的內容( 字元形式 )
		*/ 
		char inputKey[17] = {'\0'};
		_Bool key[64] = {0};
		_Bool roundKeys[16][48] = {0};
		char outputKey[17] = {'\0'};
	
		// 讀取金鑰 
		readFile("key.txt", inputKey, i);
		
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
	
		/* DES 加密法( Encrypt ) 
			inputPlainText : 存讀取的明文內容( 字元形式 )
			plainText : 明文( 位元形式 )
			cipherText : 密文( 位元形式 )
			outputCipherText : 存密文的內容( 字元形式 ) 
		*/ 
		char inputPlainText[17] = {'\0'};
		_Bool plainText[64] = {0};
		_Bool cipherText[64] = {0};
		char outputCipherText[17] = {'\0'}; 
		
		// 讀取明文 
		readFile("input.txt", inputPlainText, i);
		
		// 16 進位轉 2 進位  
		HexadecimalToBinary(inputPlainText, plainText);
		
		// DES 加密  
		Encrypt(plainText, roundKeys, cipherText);
		
		// 2 進位轉 16 進位 
		BinaryToHexadecimal(64, cipherText, outputCipherText);
		
		// 印出加密結果 
		printf("Encrypt = %s\n", outputCipherText);
		
		// 將密文寫入結果檔中 
		writeFile("output.txt", outputCipherText, i);
		
		printf("Encrypt Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/
		
		printf("--------------------------------------------------\n");
	}
	
	system("pause");  // 讓小黑框不會因為程式結束就消失 
	
	return 0;
}
