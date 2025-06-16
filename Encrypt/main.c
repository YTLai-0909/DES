#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
	
	/* ��o��Ƽƥ� 
		num : �O���`�@���X�����  
	*/ 
	int num = 0; 
	int i, j;
	
	// ��J��Ƽ�  
	printf("Input number of data : ");
	scanf("%d", &num); 
	printf("\n");
	
	
	/* ���ͦ^�X���_ + DES �[�K�k */ 
	for(i = 0; i < num; i++) {
		
		printf("Data %d :\n", i + 1);
		
	/*---------------------------------------------------------------------------*/
		/* �^�X���_���;� 
			inputKey : �sŪ�������_���e( �r���Φ� )
			key : ���_( �줸�Φ� )
			roundKeys : �^�X���_( �줸�Φ� )
			outputKey : �s�^�X���_�����e( �r���Φ� )
		*/ 
		char inputKey[17] = {'\0'};
		_Bool key[64] = {0};
		_Bool roundKeys[16][48] = {0};
		char outputKey[17] = {'\0'};
	
		// Ū�����_ 
		readFile("key.txt", inputKey, i);
		
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
	
		/* DES �[�K�k( Encrypt ) 
			inputPlainText : �sŪ�������夺�e( �r���Φ� )
			plainText : ����( �줸�Φ� )
			cipherText : �K��( �줸�Φ� )
			outputCipherText : �s�K�媺���e( �r���Φ� ) 
		*/ 
		char inputPlainText[17] = {'\0'};
		_Bool plainText[64] = {0};
		_Bool cipherText[64] = {0};
		char outputCipherText[17] = {'\0'}; 
		
		// Ū������ 
		readFile("input.txt", inputPlainText, i);
		
		// 16 �i���� 2 �i��  
		HexadecimalToBinary(inputPlainText, plainText);
		
		// DES �[�K  
		Encrypt(plainText, roundKeys, cipherText);
		
		// 2 �i���� 16 �i�� 
		BinaryToHexadecimal(64, cipherText, outputCipherText);
		
		// �L�X�[�K���G 
		printf("Encrypt = %s\n", outputCipherText);
		
		// �N�K��g�J���G�ɤ� 
		writeFile("output.txt", outputCipherText, i);
		
		printf("Encrypt Complete.\n\n");
		
	/*---------------------------------------------------------------------------*/
		
		printf("--------------------------------------------------\n");
	}
	
	system("pause");  // ���p�®ؤ��|�]���{�������N���� 
	
	return 0;
}
