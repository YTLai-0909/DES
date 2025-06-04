#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
	
	/* ��o��Ƽƥ� */  
	int num = 0;
	int i, j;
	
	printf("Input number of data : ");
	scanf("%d", &num);  // �`�@���X�����  
	printf("\n");
	
/*---------------------------------------------------------------------------*/
	
	for(i = 0; i < num; i++) {
		printf("Data %d :\n", i + 1);
		/* �^�X���_���;� */ 
		char inputKey[17] = {'\0'};
		_Bool key[64] = {0};
		_Bool roundKeys[16][48] = {0};
		char outputKey[17] = {'\0'};
	
		// Ū�����_ 
		openFile("key.txt", inputKey, i);
		
		// 16 �i���� 2 �i�� 
		HexadecimalToBinary(inputKey, key);
		
		// ���ͦ^�X���_  
		keyGenerator(key, roundKeys);
		
		// �L�X�^�X���_  
		printf("RoundKey\n");
		for(j = 0; j < 16; j++) {
			// 2 �i���� 16 �i�� 
			BinaryToHexadecimal(48, roundKeys[j], outputKey);
			printf("%s\n", outputKey);
		}
		printf("Key Generator Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/
	
		/* DES �[�K�k ( Encrypt )*/  
		char inputP[17] = {'\0'};
		_Bool encryptPlainText[64] = {0};
		_Bool encryptCipherText[64] = {0};
		char outputC[17] = {'\0'}; 
		
		// Ū������ 
		openFile("input.txt", inputP, i);
		
		// 16 �i���� 2 �i��  
		HexadecimalToBinary(inputP, encryptPlainText);
		
		// DES �[�K  
		Encrypt(encryptPlainText, roundKeys, encryptCipherText);
		
		// 2 �i���� 16 �i�� 
		BinaryToHexadecimal(64, encryptCipherText, outputC);
		
		// �L�X�[�K���G 
		printf("Encrypt = %s\n", outputC);
		
		// ���͵��G�� 
		writeFile("output.txt", outputC, i);
		printf("Encrypt Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/		
		
		printf("-------------------------------------------------\n");
	}
	
	system("pause");  // ���p�®ؤ��|�]���{�������N���� 
	
	return 0;
}
