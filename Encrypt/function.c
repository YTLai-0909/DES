#include <stdio.h>
#include <string.h>
#include "function.h"
#include "table.h"


/* �}�� 
    name : �ɮצW�� 
	data : Ū������� 
	�^�� : data  
*/ 
void openFile(char *name, char *data, int num) {
	
	FILE *rptr = fopen(name, "r");
	char tmp = '\0';
	int i;
	
	if(rptr == NULL) {
		printf("Open file failed.\n");
	} else {
		fseek(rptr, 18 * num, SEEK_CUR);  // �U�@���ƪ��}�Y�O 18 �����ơA�]���@���Ʀ� 17 �Ӧr�� ( 16 �Ӧr�� + 1 �Ӵ��� )  
		for(i = 0; i < 16; i++) { 
			fscanf(rptr, "%c", &tmp);
			data[i] = tmp;
		}
	}
	
//	printf("openFile = %s\n", data);
	
	fclose(rptr);
}

/* 16 �i���� 2 �i�� 
	input : 16 �i�쪺�r��  
	output : �ন 2 �i�쪺��  
	�^�� : output   
*/
void HexadecimalToBinary(char *input, _Bool *output) {
	
	int len = 0, value = 0;
	int i;
	
	len = strlen(input);
	for(i = 0; i < len; i++) {
		// 16 �i�� -> 10 �i��
		if(input[i] >= '0' && input[i] <= '9') {  // �Q���i�� 0-9 -> �Q�i��n�� 48( 0 �� ASCII = 48 )  
			value = input[i] - '0';
		} else if(input[i] >= 'A' && input[i] <= 'F') {  // �Q���i�� A-F -> �Q�i��n�� 55( A �� ASCII = 65 ) 
			value = input[i] - 55;
		}
		// 10 �i�� -> 2 �i��  
		output[4 * i] = value / 8;
		value = value % 8;
		output[4 * i + 1] = value / 4;
		value = value % 4;
		output[4 * i + 2] = value / 2;
		value = value % 2;
		output[4 * i + 3] = value;
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
	keyWithParities : ��l���_ 
	roundKeys : ���ͪ��^�X���_�A16 �� 
	shiftTable : �C�^�X�첾�ƶq�� 
	�^�� : roundKeys  
*/
void keyGenerator(_Bool *keyWithParities, _Bool roundKeys[16][48], int *shiftTable) {
	
	_Bool cipherKey[56] = {0};
	_Bool leftKey[28] = {0}, rightKey[28] = {0};
	_Bool *splitKey[2];
	_Bool preRoundKey[56] = {0};
	int i;
	
	splitKey[0] = leftKey;
	splitKey[1] = rightKey;

	
	// �P�줸������  
	permute(64, 56, keyWithParities, cipherKey, ParityDropTable);
	
	// ���� 
	split(56, 28, cipherKey, leftKey, rightKey, splitKey);
	
	// ���ͦ^�X���_�A16 ��  
	for(i = 0; i < 16; i++) {
		// ���첾  
		shiftLeft(leftKey, shiftTable[i]);
		shiftLeft(rightKey, shiftTable[i]);
		// ���X  
		combine(28, 56, leftKey, rightKey, preRoundKey);
		// ���_���Y��  
		permute(56, 48, preRoundKey, roundKeys[i], KeyCompressionTable);		
	}
	
}

/* �ƦC 
	startBitNum : �ƦC�e���줸�� 
	endBitNum : �ƦC�᪺�줸�� 
	startBlock : �ƦC�e���϶� 
	endBlock : �ƦC�᪺�϶� 
	table : �ƦC���  
	�^�� : endBlock  
*/ 
void permute(int startBitNum, int endBitNum, _Bool *startBlock, _Bool *endBlock, int *table) {
	
	int i;
	
	for(i = 0; i < endBitNum; i++) {
		endBlock[i] = startBlock[table[i] - 1];  // table[i] - 1 : �]���}�C�q 0 �}�l�A�� table �q 1 �}�l�O��  
	}
	
//	// Test
//	printf("permute:\n");
//	for(i = 0; i < endBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", endBlock[i]);
//	}
//	printf("\n");
	
}

/* ���� 
	startBitNum : �����e���줸�� 
	endBitNum : �����᪺�줸�� 
	inBlock : �����e���϶� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	splitBlock : �ΨӦs����϶��M�k��϶������а}�C ( �禡�@���^�Ǩ�Ӽƭ� ) 
	�^�� : splitBlock  
*/ 
void split(int startBitNum, int endBitNum, _Bool *inBlock, _Bool *leftBlock, _Bool *rightBlock, _Bool **splitBlock) {
	
	int i;
	
	for(i = 0; i < startBitNum; i++) {
		if(i < startBitNum / 2) {  // ����϶�  
			leftBlock[i] = inBlock[i];	
		} else {  // �k��϶�  
			rightBlock[i - endBitNum] = inBlock[i];  // ������AinBlock ���� 32 �줸 ( endBitNum = 32 ) �ܦ� rightBlock ���� 0 �줸  
		}
	}
	splitBlock[0] = leftBlock;
	splitBlock[1] = rightBlock; 
	
//	// Test 
//	int j;
//	printf("split:\n");
//	for(i = 0; i < 2; i++) {
//		for(j = 0; j < endBitNum; j++) {
//			if(j != 0 && j % 4 == 0) {
//				printf(" ");
//			}
//			printf("%d", splitBlock[i][j]);
//		}
//		printf("\n");		
//	}
	
}

/* ���첾 
	block : �n�첾���϶� 
	numOfShifts : �n�����X�� 
	�^�� : block  
*/
void shiftLeft(_Bool *block, int numOfShifts) {
	
	_Bool tmp;
	int i, j;
	
	for(i = 0; i < numOfShifts; i++) {
		tmp = block[0];
		for(j = 1; j < 28; j++) {
			block[j - 1] = block[j];
		}
		block[27] = tmp;
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
	startBitNum : ���X�e���줸�� 
	endBitNum : ���X�᪺�줸�� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	outBlock : ���X���϶� 
	�^�� : outBlock  
*/
void combine(int startBitNum, int endBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outBlock) {
	
	int i;
	
	for(i = 0; i < endBitNum; i++) {
		if(i < endBitNum / 2) {  // ����϶�  
			outBlock[i] = leftBlock[i];
		} else {  // �k��϶�  
			outBlock[i] = rightBlock[i - startBitNum];  // ���X��AoutBlock ���� 28 �줸 ( startBitNum = 28 ) ��o�O rightBlock ���� 0 �줸  
		}
	}
	
//	// Test 
//	printf("combine:\n");
//	for(i = 0; i < endBitNum; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", outBlock[i]);
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
	
	_Bool inBlock[64] = {0};
	_Bool leftBlock[32] = {0}, rightBlock[32] = {0};
	_Bool *splitBlock[2];
	_Bool outBlock[64] = {0};
	int i;
	
	splitBlock[0] = leftBlock;
	splitBlock[1] = rightBlock;
	
	// ��l�ƦC  
	permute(64, 64, plainBlock, inBlock, InitialPermutationTable);
	
	// ���� 
	split(64, 32, inBlock, leftBlock, rightBlock, splitBlock);

	// DES �[�K�k�A16 �Ӧ^�X  
	for(i = 0; i < 16; i++){
		mixer(leftBlock, rightBlock, roundKeys[i]);
		if(i != 15) {  // �̫�@�^�X�����洫��  
			swapper(leftBlock, rightBlock, splitBlock);
		}
	}
	
	// ���X  
	combine(32, 64, leftBlock, rightBlock, outBlock);
	
	// �̲ױƦC  
	permute(64, 64, outBlock, cipherBlock, FinalPermutationTable); 

}

/* �V�X��  
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	roundKey : �^�X���_ 
	�^�� : leftBlock  
*/
void mixer(_Bool *leftBlock, _Bool *rightBlock, _Bool *roundKey) {
	
	_Bool T1[32] = {0};
	_Bool T2[32] = {0};
	_Bool T3[32] = {0};
	
	copy(32, rightBlock, T1);
	function(T1, roundKey, T2);
	exclusiveOr(32, leftBlock, T2, T3);
	copy(32, T3, leftBlock);

}

/* �ƻs���e�禡 
	 len : input ������  
	 input : �n�ƻs�����e 
	 output : �s�ƻs�����e 
	 �^�� : output  
*/
void copy(int len, _Bool *input, _Bool *output) {
	
	int i;
	
	for(i = 0; i < len; i++) {
		output[i] = input[i];
	}
	
//	// Test  
//	printf("copy:\n");
//	for(i = 0; i < len; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", output[i]);
//	}
//	printf("\n");

}

/* DES ��� 
	inBlock : ��J���϶� 
	roundKey : �^�X���_ 
	outBlock : ��X���϶�  
	�^�� : outBlock  
*/ 
void function(_Bool *inBlock, _Bool *roundKey, _Bool *outBlock) {

	_Bool T1[48] = {0};
	_Bool T2[48] = {0};
	_Bool T3[32] = {0};
	
	permute(32, 48, inBlock, T1, ExpansionPermutationTable);
	exclusiveOr(48, T1, roundKey, T2);
	substitue(T2, T3, SubstituteTable);
	permute(32, 32, T3, outBlock, StraightPermutationTable);

}

/* XOR �B�� 
	len : �� XOR �B�⪺���� 
	inputA : �� XOR �B�⪺�}�C 
	inputB : �� XOR �B�⪺�}�C  
	output : ���� XOR �B�⪺���G 
	�^�� : output  
*/ 
void exclusiveOr(int len, _Bool *inputA, _Bool *inputB, _Bool *output) {
	
	int i;
	
	for(i = 0; i < len; i++) {
		output[i] = inputA[i] ^ inputB[i];
	}
	
//	// Test 
//	printf("exclusiveOr:\n");
//	for(i = 0; i < len; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", output[i]);
//	}
//	printf("\n");

}

/* S-box 
	inBlock : ��J���϶� 
	outBlock : ��X���϶� 
	SubstitutionTables[8][4][16] : S-box 
	�^�� : outBlock  
*/
void substitue(_Bool *inBlock, _Bool *outBlock, int SubstituteTable[8][4][16]) {
	
	int i;
	int row = 0, col = 0;
	int value = 0;
	
	for(i = 0; i < 8; i++) {
		row = 2 * inBlock[i * 6] + inBlock[i * 6 + 5];
		col = 8 * inBlock[i * 6 + 1] + 4 * inBlock[i * 6 + 2] + 2 * inBlock[i * 6 + 3] + inBlock[i * 6 + 4];
		value = SubstituteTable[i][row][col];
		outBlock[i * 4] = value / 8;
		value = value % 8;
		outBlock[i * 4 + 1] = value / 4;
		value = value % 4;
		outBlock[i * 4 + 2] = value / 2;
		value = value % 2;
		outBlock[i * 4 + 3] = value;
	}
	
//	// Test 
//	printf("substitue:\n");
//	for(i = 0; i < 32; i++) {
//		if(i != 0 && i % 4 == 0) {
//			printf(" ");
//		}
//		printf("%d", outBlock[i]);
//	}

}

/* �洫�� 
	leftBlock : ����϶� 
	rightBlock : �k��϶� 
	splitBlock : �ΨӦs����϶��M�k��϶������а}�C ( �禡�@���^�Ǩ�Ӽƭ� ) 
	�^�� : splitBlock 
*/
void swapper(_Bool *leftBlock, _Bool *rightBlock, _Bool **splitBlock) {
	
	_Bool T[32] = {0};
	
	copy(32, leftBlock, T);
	copy(32, rightBlock, leftBlock);
	copy(32, T, rightBlock);
	
	splitBlock[0] = leftBlock;
	splitBlock[1] = rightBlock;
	
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
	len : input ������ 
	input : 2 �i�쪺��  
	output : �ন 16 �i�쪺�r�� 
	�^�� : output  
*/
void BinaryToHexadecimal(int len, _Bool *input, char* output) {
	
	int value = 0;
	int i;
	
	len = len / 4;  // 2 �i�쪺 4 �� bit = 16 �i�쪺 1 �� bit�A�ҥH len / 4   
	
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
*/
void writeFile(char *name, char *data, int num) {
	
	FILE *wptr = fopen(name, "a");
	int tmp = 0, count = 0;
	
	if(wptr == NULL) {
		printf("Open file failed.\n");
	} else {
		tmp = num + 1;
		count = 0;
		while(tmp != 0) {  // �p��n print ���Ʀr�O�X���  
			tmp = tmp / 10;
			count++;
		}
		count = count + 2;  // print �ɡA�u.�v�M�u�Ů�v���o��m   
		fseek(wptr, 18 * num + count , SEEK_CUR);  // �U�@���ƪ��}�Y�O 18 �����ƥ[�W�榡�վ�A�]���@���Ʀ� 17 �Ӧr�� ( 16 �Ӧr�� + 1 �Ӵ��� )  
		fprintf(wptr, "%d. %s\n", num + 1, data);
	}
	fclose(wptr);
}
