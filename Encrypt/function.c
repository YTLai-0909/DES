#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "table.h"


/* Ū�� 
	name : �ɮצW�� 
	data : Ū������� 
	num : Ū�����O�ĴX�����  
	�^�� : data  
*/ 
void readFile(char *name, char *data, int num) {
	
	FILE *rptr = fopen(name, "rb");  // �H�줸��( Byte )�����B�z�ɮ� 
	
	if(rptr == NULL) {
		printf("Read %s failed.\n\n", name);
		system("pause");  // ���p�®ؤ��|�]���{�������N���� 
		exit(EXIT_FAILURE);  // �����{��  
	} else {
		printf("Read %s success.\n\n", name);
		fseek(rptr, 18 * num, SEEK_CUR);  // �U�@���ƪ��}�Y�O 18 �����ơA�]���@���Ʀ� 17 �Ӧr��( 18 Bytes = 16 �Ӧr��( 16 Bytes ) + 1 �Ӵ���( 2 Bytes, \n = \r\n ) ) 
		fgets(data, 17, rptr);  // �]���|�۰ʸɤW '\0'�A�ҥHŪ�����r�����׭n�� 17 ( 16 �Ӧr�� + 1 �� '\0' )
	}
	
//	printf("readFile = %s\n", data);
	
	fclose(rptr);
}

/* 16 �i���� 2 �i�� 
	input : 16 �i�쪺�r��  
	output : �ন 2 �i�쪺��  
	�^�� : output   
*/
void HexadecimalToBinary(char *input, _Bool *output) {
	
	/*
		len : 16 �i�쪺�r����� 
		value : 16 �i���ন 10 �i�쪺��  
	*/
	
	int len = 0, value = 0;
	int i, j;
	
	len = strlen(input);
	for(i = 0; i < len; i++) {
		// 16 �i�� -> 10 �i��
		if(input[i] >= '0' && input[i] <= '9') {  // �Q���i�� 0-9 -> �Q�i��n�� 48( 0 �� ASCII = 48 )  
			value = input[i] - '0';
		} else if(input[i] >= 'A' && input[i] <= 'F') {  // �Q���i�� A-F -> �Q�i��n�� 55( A �� ASCII = 65 ) 
			value = input[i] - 55;
		}
		// 10 �i�� -> 2 �i��  
		for(j = 3; j >= 0; j--) {
			output[4 * i + j] = value % 2;
			value /= 2;
		}
	}
	
//	// Test
//	printf("HexadecimalToBinary:\n");
//	for(i = 0; i < len * 4; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", output[i]);
//	}
//	printf("\n");

}

/* �^�X���_���;�  
	key : ��l���_ 
	roundKeys : ���ͪ��^�X���_�A16 �� 
	�^�� : roundKeys  
*/
void keyGenerator(_Bool *key, _Bool roundKeys[16][48]) {
	
	/*
		tmpKey : �ΨӼȦs���_�����P�줸�����᪺���G  
		leftKey : �����᪺����϶�  
		rightKey : �����᪺�k��϶�  
		tmpRoundKey : �ΨӼȦs���k�϶����X���٨S�����_���Y�����G  
	*/
	
	_Bool tmpKey[56] = {0};
	_Bool leftKey[28] = {0}, rightKey[28] = {0};
	_Bool tmpRoundKey[56] = {0};
	int i;
	
	// �P�줸������  
	permute(64, 56, key, tmpKey, ParityDropTable);
	
	// ���� 
	split(56, 28, tmpKey, leftKey, rightKey);
	
	// ���ͦ^�X���_�A16 ��  
	for(i = 0; i < 16; i++) {
		// ���첾  
		shiftLeft(leftKey, ShiftTable[i]);
		shiftLeft(rightKey, ShiftTable[i]);
		// ���X  
		combine(28, 56, leftKey, rightKey, tmpRoundKey);
		// ���_���Y��  
		permute(56, 48, tmpRoundKey, roundKeys[i], KeyCompressionTable);		
	}
}

/* �ƦC 
	inputBitNum : �ƦC�e���줸�� 
	outputBitNum : �ƦC�᪺�줸�� 
	inputBlock : �ƦC�e���϶� 
	outputBlock : �ƦC�᪺�϶� 
	table : �ƦC���  
	�^�� : outputBlock  
*/ 
void permute(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *outputBlock, int *table) {
	
	int i;
	
	for(i = 0; i < outputBitNum; i++) {
		outputBlock[i] = inputBlock[table[i] - 1];  // table[i] - 1 : �]���}�C�q 0 �}�l�A�� table �q 1 �}�l�O��  
	}
	
//	// Test
//	printf("permute:\n");
//	for(i = 0; i < outputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", outputBlock[i]);
//	}
//	printf("\n");
	
}

/* ���� 
	inputBitNum : �����e���줸�� 
	outputBitNum : �����᪺�줸�� 
	inputBlock : �����e���϶� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	�^�� : leftBlock, rightBlock 
*/ 
void split(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *leftBlock, _Bool *rightBlock) {
	
	int i;
	
	for(i = 0; i < inputBitNum; i++) {
		if(i < inputBitNum / 2) {  // ����϶�  
			leftBlock[i] = inputBlock[i];	
		} else {  // �k��϶�  
			rightBlock[i - outputBitNum] = inputBlock[i];  // ������AinputBlock �������줸( = outputBitNum )�ܦ� rightBlock ���� 0 �줸  
		}
	}
	
//	// Test 
//	printf("split:\n");
//	for(i = 0; i < outputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", leftBlock[i]);
//	}
//	printf("\n");
//	for(i = 0; i < outputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", rightBlock[i]);
//	}
//	printf("\n");
	
}

/* ���첾 
	block : �n�첾���϶� 
	numOfShifts : �n�����X�� 
	�^�� : block  
*/
void shiftLeft(_Bool *block, int numOfShifts) {
	
	/*
		tmp : �Ȧs�}�C�� 0 �Ӧ�m���ȡA���������첾��A�A���}�C������  
	*/
	
	_Bool tmp = 0;
	int i, j;
	
	for(i = 0; i < numOfShifts; i++) {
		tmp = block[0];  // �s�}�C�� 0 �Ӧ�m����  
		for(j = 1; j < 28; j++) {
			block[j - 1] = block[j];  // ���첾  
		}
		block[27] = tmp;  // �N�� 0 �Ӧ�m���ȩ��}�C������  
	}
	
//	// Test 
//	printf("shiftLeft:\n");
//	for(i = 0; i < 28; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", block[i]);
//	}
//	printf("\n");

}

/* ���X  
	inputBitNum : ���X�e���줸�� 
	outputBitNum : ���X�᪺�줸�� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	outputBlock : ���X���϶� 
	�^�� : outputBlock   
*/
void combine(int inputBitNum, int outputBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outputBlock) {
	
	int i;
	
	for(i = 0; i < outputBitNum; i++) {
		if(i < outputBitNum / 2) {  // ����϶�  
			outputBlock[i] = leftBlock[i];
		} else {  // �k��϶�  
			outputBlock[i] = rightBlock[i - inputBitNum];  // ���X��AoutputBlock �������줸( = inputBitNum )��o�O rightBlock ���� 0 �줸  
		}
	}
	
//	// Test 
//	printf("combine:\n");
//	for(i = 0; i < outputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", outputBlock[i]);
//	}
//	printf("\n");

}

/* DES �[�K�k 
	plainBlock : ���� 
	roundKeys : �^�X���_ 
	cipherBlock : �K�� 
	�^�� : cipherBlock  
*/
void Encrypt(_Bool *plainBlock, _Bool roundKeys[16][48], _Bool *cipherBlock) {
	
	/*
		tmpPlainBlock : �Ȧs������l�ƦC�����G  
		leftBlock : �����᪺����϶�  
		rightBlock : �����᪺�k��϶�  
		tmpCipherBlock : �Ȧs���k�϶����X���٨S���̲ױƦC�����G  
	*/
	
	_Bool tmpPlainBlock[64] = {0};
	_Bool leftBlock[32] = {0}, rightBlock[32] = {0};
	_Bool tmpCipherBlock[64] = {0};
	int i;
	
	// ��l�ƦC  
	permute(64, 64, plainBlock, tmpPlainBlock, InitialPermutationTable);
	
	// ���� 
	split(64, 32, tmpPlainBlock, leftBlock, rightBlock);

	// DES �[�K�k�A16 �Ӧ^�X  
	for(i = 0; i < 16; i++) {
		// �V�X��  
		mixer(leftBlock, rightBlock, roundKeys[i]);
		if(i != 15) {  // �̫�@�^�X�����洫��  
			// �洫��  
			swapper(leftBlock, rightBlock);
		}
	}
	
	// ���X  
	combine(32, 64, leftBlock, rightBlock, tmpCipherBlock);
	
	// �̲ױƦC  
	permute(64, 64, tmpCipherBlock, cipherBlock, FinalPermutationTable);
}

/* �V�X��  
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	roundKey : �^�X���_ 
	�^�� : leftBlock  
*/
void mixer(_Bool *leftBlock, _Bool *rightBlock, _Bool *roundKey) {
	
	/*
		tmp1 : �Ȧs�k��϶������e  
		tmp2 : �Ȧs���� DES ��ƪ����G  
		tmp3 : �Ȧs���� XOR �B�⪺���G  
	*/
	
	_Bool tmp1[32] = {0};
	_Bool tmp2[32] = {0};
	_Bool tmp3[32] = {0};
	
	// �ƻs�k��϶������e  
	copy(32, rightBlock, tmp1);
	
	// DES ���  
	function(tmp1, roundKey, tmp2);
	
	// XOR �B��  
	exclusiveOr(32, leftBlock, tmp2, tmp3);
	
	// �N mixer �����G�ƻs�쥪��϶�  
	copy(32, tmp3, leftBlock);
}

/* �ƻs���e�禡 
	inputBitNum : input ���줸��  
	input : �n�ƻs�����e 
	output : �s�ƻs�����e 
	�^�� : output  
*/
void copy(int inputBitNum, _Bool *input, _Bool *output) {
	
	int i;
	
	for(i = 0; i < inputBitNum; i++) {
		output[i] = input[i];
	}
	
//	// Test  
//	printf("copy:\n");
//	for(i = 0; i < inputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", output[i]);
//	}
//	printf("\n");

}

/* DES ��� 
	inputBlock : ��J���϶� 
	roundKey : �^�X���_ 
	outputBlock : ��X���϶� 
	�^�� : outputBlock 
*/ 
void function(_Bool *inputBlock, _Bool *roundKey, _Bool *outputBlock) {

	/*
		tmp1 : �Ȧs�����X�i�� P-box �����G  
		tmp2 : �Ȧs���� XOR �B�⪺���G  
		tmp3 : �Ȧs���� S-box �����G  
	*/
	
	_Bool tmp1[48] = {0};
	_Bool tmp2[48] = {0};
	_Bool tmp3[32] = {0};
	
	// �X�i�� P-box  
	permute(32, 48, inputBlock, tmp1, ExpansionPermutationTable);
	
	// XOR �B��  
	exclusiveOr(48, tmp1, roundKey, tmp2);
	
	// S-box  
	substitue(tmp2, tmp3);
	
	// �s�� P-box( �зǱƦC ) 
	permute(32, 32, tmp3, outputBlock, StraightPermutationTable);
}

/* XOR �B�� 
	inputBitNum : input ���줸��  
	inputA : �� XOR �B�⪺�}�C 
	inputB : �� XOR �B�⪺�}�C  
	output : ���� XOR �B�⪺���G 
	�^�� : output  
*/ 
void exclusiveOr(int inputBitNum, _Bool *inputA, _Bool *inputB, _Bool *output) {
	
	int i;
	
	for(i = 0; i < inputBitNum; i++) {
		output[i] = inputA[i] ^ inputB[i];
	}
	
//	// Test 
//	printf("exclusiveOr:\n");
//	for(i = 0; i < inputBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", output[i]);
//	}
//	printf("\n");

}

/* S-box 
	inputBlock : ��J���϶� 
	outputBlock : ��X���϶� 
	�^�� : outputBlock  
*/
void substitue(_Bool *inputBlock, _Bool *outputBlock) {
	
	/*
		row : SubstituteTable ���C( �ѨC�Ӱ϶����Ĥ@�M�Ĥ��줸�զ� ) 
		col : SubstituteTable ����( �ѨC�Ӱ϶����ĤG��Ĥ��줸�զ� ) 
		value : �ھڪ�檺�s���B�C�B��A���o SubstituteTable ��������  
	*/
	
	int row = 0, col = 0;
	int value = 0;
	int i, j;
	
	for(i = 0; i < 8; i++) {
		row = 2 * inputBlock[6 * i] + inputBlock[6 * i + 5];
		col = 8 * inputBlock[6 * i + 1] + 4 * inputBlock[6 * i + 2] + 2 * inputBlock[6 * i + 3] + inputBlock[6 * i + 4];
		value = SubstituteTable[i][row][col];
		for(j = 3; j >= 0; j--) {  // �N 10 �i�쪺���ন 2 �i��A�B�C�Ӱ϶����˵۩�J�}�C��  
			outputBlock[4 * i + j] = value % 2;
			value /= 2;
		}
	}
	
//	// Test 
//	printf("substitue:\n");
//	for(i = 0; i < 32; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", outputBlock[i]);
//	}
//	printf("\n");

}

/* �洫�� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	�^�� : leftBlock, rightBlock 
*/
void swapper(_Bool *leftBlock, _Bool *rightBlock) {
	
	/*
		tmp : ���洫�ɥΨӷ�Ȧs��  
	*/
	
	_Bool tmp[32] = {0};
	
	copy(32, leftBlock, tmp);
	copy(32, rightBlock, leftBlock);
	copy(32, tmp, rightBlock);
	
//	// Test 
//	int i;
//	printf("left:\n");
//	for(i = 0; i < 32; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", leftBlock[i]);
//	}
//	printf("\n");
//	printf("right:\n");
//	for(i = 0; i < 32; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", rightBlock[i]);
//	}
//	printf("\n");

}

/* 2 �i���� 16 �i�� 
	inputBitNum : input ���줸��  
	input : 2 �i�쪺��  
	output : �ন 16 �i�쪺�r�� 
	�^�� : output  
*/
void BinaryToHexadecimal(int inputBitNum, _Bool *input, char *output) {
	
	/*
		len : 2 �i���ন 16 �i��᪺�r����� 
		value : 2 �i���ন 10 �i�쪺��  
	*/
	
	int len = 0, value = 0;
	int i;
	
	len = inputBitNum / 4;  // 2 �i�쪺 4 �� bit = 16 �i�쪺 1 �� bit�A�ҥH inputBitNum / 4   
	
	for(i = 0; i < len; i++) {
		// 2 �i�� -> 10 �i��  
		value = 8 * input[4 * i] + 4 * input[4 * i + 1] + 2 * input[4 * i + 2] + input[4 * i + 3];
		// 10 �i�� -> 16 �i��  
		if(value >= 0 && value <= 9) {  // �Q�i�� 0-9 -> �Q���i��n�[ 48 ( 0 �� ASCII = 48 ) 
			output[i] = value + 48;
		} else if(value >= 10 && value <= 15) {  // �Q���i�� A-F -> �Q�i��n�[ 55( A �� ASCII = 65 ) 
			output[i] = value + 55;
		}		
	}
	
//	// Test 
//	printf("BinaryToHexadecimal:\n");
//	for(i = 0; i < len; i++) {
//		printf("%c", output[i]);
//	}
//	printf("\n");

}

/* �g�� 
	name : �ɮצW�� 
	data : �g�J�����e  
	num : �g�J���O�ĴX�����  
*/
void writeFile(char *name, char *data, int num) {
	
	FILE *wptr = fopen(name, "a");
	
	if(wptr == NULL) {
		printf("Write %s failed.\n\n", name);
		system("pause");  // ���p�®ؤ��|�]���{�������N���� 
		exit(EXIT_FAILURE);  // �����{��  
	} else {
		printf("Write %s success.\n\n", name);
		fprintf(wptr, "%d. %s\n", num + 1, data);  // +1 : �]���}�C���Ĥ@�Ӧ�m index = 0  
	}
	fclose(wptr);
}
