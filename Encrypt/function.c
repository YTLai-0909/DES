#include <stdio.h>
#include <string.h>
#include "function.h"
#include "table.h"


/* 開檔 
    name : 檔案名稱 
	data : 讀取的資料 
	回傳 : data  
*/ 
void openFile(char *name, char *data, int num) {
	
	FILE *rptr = fopen(name, "r");
	char tmp = '\0';
	int i;
	
	if(rptr == NULL) {
		printf("Open file failed.\n");
	} else {
		fseek(rptr, 18 * num, SEEK_CUR);  // 下一行資料的開頭是 18 的倍數，因為一行資料有 17 個字元 ( 16 個字母 + 1 個換行 )  
		for(i = 0; i < 16; i++) { 
			fscanf(rptr, "%c", &tmp);
			data[i] = tmp;
		}
	}
	
//	printf("openFile = %s\n", data);
	
	fclose(rptr);
}

/* 16 進位轉 2 進位 
	input : 16 進位的字串  
	output : 轉成 2 進位的值  
	回傳 : output   
*/
void HexadecimalToBinary(char *input, _Bool *output) {
	
	int len = 0, value = 0;
	int i;
	
	len = strlen(input);
	for(i = 0; i < len; i++) {
		// 16 進位 -> 10 進位
		if(input[i] >= '0' && input[i] <= '9') {  // 十六進位 0-9 -> 十進位要減 48( 0 的 ASCII = 48 )  
			value = input[i] - '0';
		} else if(input[i] >= 'A' && input[i] <= 'F') {  // 十六進位 A-F -> 十進位要減 55( A 的 ASCII = 65 ) 
			value = input[i] - 55;
		}
		// 10 進位 -> 2 進位  
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

/* 回合金鑰產生器  
	keyWithParities : 初始金鑰 
	roundKeys : 產生的回合金鑰，16 個 
	shiftTable : 每回合位移數量表 
	回傳 : roundKeys  
*/
void keyGenerator(_Bool *keyWithParities, _Bool roundKeys[16][48], int *shiftTable) {
	
	_Bool cipherKey[56] = {0};
	_Bool leftKey[28] = {0}, rightKey[28] = {0};
	_Bool *splitKey[2];
	_Bool preRoundKey[56] = {0};
	int i;
	
	splitKey[0] = leftKey;
	splitKey[1] = rightKey;

	
	// 同位元移除表  
	permute(64, 56, keyWithParities, cipherKey, ParityDropTable);
	
	// 分裂 
	split(56, 28, cipherKey, leftKey, rightKey, splitKey);
	
	// 產生回合金鑰，16 個  
	for(i = 0; i < 16; i++) {
		// 左位移  
		shiftLeft(leftKey, shiftTable[i]);
		shiftLeft(rightKey, shiftTable[i]);
		// 結合  
		combine(28, 56, leftKey, rightKey, preRoundKey);
		// 金鑰壓縮表  
		permute(56, 48, preRoundKey, roundKeys[i], KeyCompressionTable);		
	}
	
}

/* 排列 
	startBitNum : 排列前的位元數 
	endBitNum : 排列後的位元數 
	startBlock : 排列前的區塊 
	endBlock : 排列後的區塊 
	table : 排列表格  
	回傳 : endBlock  
*/ 
void permute(int startBitNum, int endBitNum, _Bool *startBlock, _Bool *endBlock, int *table) {
	
	int i;
	
	for(i = 0; i < endBitNum; i++) {
		endBlock[i] = startBlock[table[i] - 1];  // table[i] - 1 : 因為陣列從 0 開始，但 table 從 1 開始記錄  
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

/* 分裂 
	startBitNum : 分裂前的位元數 
	endBitNum : 分裂後的位元數 
	inBlock : 分裂前的區塊 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	splitBlock : 用來存左邊區塊和右邊區塊的指標陣列 ( 函式一次回傳兩個數值 ) 
	回傳 : splitBlock  
*/ 
void split(int startBitNum, int endBitNum, _Bool *inBlock, _Bool *leftBlock, _Bool *rightBlock, _Bool **splitBlock) {
	
	int i;
	
	for(i = 0; i < startBitNum; i++) {
		if(i < startBitNum / 2) {  // 左邊區塊  
			leftBlock[i] = inBlock[i];	
		} else {  // 右邊區塊  
			rightBlock[i - endBitNum] = inBlock[i];  // 分裂後，inBlock 的第 32 位元 ( endBitNum = 32 ) 變成 rightBlock 的第 0 位元  
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

/* 左位移 
	block : 要位移的區塊 
	numOfShifts : 要左移幾格 
	回傳 : block  
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

/* 結合  
	startBitNum : 結合前的位元數 
	endBitNum : 結合後的位元數 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	outBlock : 結合的區塊 
	回傳 : outBlock  
*/
void combine(int startBitNum, int endBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outBlock) {
	
	int i;
	
	for(i = 0; i < endBitNum; i++) {
		if(i < endBitNum / 2) {  // 左邊區塊  
			outBlock[i] = leftBlock[i];
		} else {  // 右邊區塊  
			outBlock[i] = rightBlock[i - startBitNum];  // 結合後，outBlock 的第 28 位元 ( startBitNum = 28 ) 放得是 rightBlock 的第 0 位元  
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

/* DES 加密法 
	plainBlock : 明文 
	roundKeys : 回合金鑰 
	cipherBlock : 密文 
	回傳 : cipherBlock  
*/
void Encrypt(_Bool *plainBlock, _Bool roundKeys[16][48], _Bool *cipherBlock) {
	
	_Bool inBlock[64] = {0};
	_Bool leftBlock[32] = {0}, rightBlock[32] = {0};
	_Bool *splitBlock[2];
	_Bool outBlock[64] = {0};
	int i;
	
	splitBlock[0] = leftBlock;
	splitBlock[1] = rightBlock;
	
	// 初始排列  
	permute(64, 64, plainBlock, inBlock, InitialPermutationTable);
	
	// 分裂 
	split(64, 32, inBlock, leftBlock, rightBlock, splitBlock);

	// DES 加密法，16 個回合  
	for(i = 0; i < 16; i++){
		mixer(leftBlock, rightBlock, roundKeys[i]);
		if(i != 15) {  // 最後一回合不做交換器  
			swapper(leftBlock, rightBlock, splitBlock);
		}
	}
	
	// 結合  
	combine(32, 64, leftBlock, rightBlock, outBlock);
	
	// 最終排列  
	permute(64, 64, outBlock, cipherBlock, FinalPermutationTable); 

}

/* 混合器  
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	roundKey : 回合金鑰 
	回傳 : leftBlock  
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

/* 複製內容函式 
	 len : input 的長度  
	 input : 要複製的內容 
	 output : 存複製的內容 
	 回傳 : output  
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

/* DES 函數 
	inBlock : 輸入的區塊 
	roundKey : 回合金鑰 
	outBlock : 輸出的區塊  
	回傳 : outBlock  
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

/* XOR 運算 
	len : 做 XOR 運算的長度 
	inputA : 做 XOR 運算的陣列 
	inputB : 做 XOR 運算的陣列  
	output : 做完 XOR 運算的結果 
	回傳 : output  
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
	inBlock : 輸入的區塊 
	outBlock : 輸出的區塊 
	SubstitutionTables[8][4][16] : S-box 
	回傳 : outBlock  
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

/* 交換器 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	splitBlock : 用來存左邊區塊和右邊區塊的指標陣列 ( 函式一次回傳兩個數值 ) 
	回傳 : splitBlock 
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

/* 2 進位轉 16 進位 
	len : input 的長度 
	input : 2 進位的值  
	output : 轉成 16 進位的字串 
	回傳 : output  
*/
void BinaryToHexadecimal(int len, _Bool *input, char* output) {
	
	int value = 0;
	int i;
	
	len = len / 4;  // 2 進位的 4 個 bit = 16 進位的 1 個 bit，所以 len / 4   
	
	for(i = 0; i < len; i++) {
		// 2 進位 -> 10 進位  
		value = 8 * input[4 * i] + 4 * input[4 * i + 1] + 2 * input[4 * i + 2] + input[4 * i + 3];
		// 10 進位 -> 16 進位  
		if(value >= 0 && value <= 9) {  // 十進位 0-9 -> 十六進位要加 48 ( 0 的 ASCII = 48 ) 
			output[i] = value + 48;
		} else if(value >= 10 && value <= 15) {  // 十六進位 A-F -> 十進位要加 55( A 的 ASCII = 65 ) 
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

/* 寫檔 
	name : 檔案名稱 
	data : 寫入的內容  
*/
void writeFile(char *name, char *data, int num) {
	
	FILE *wptr = fopen(name, "a");
	int tmp = 0, count = 0;
	
	if(wptr == NULL) {
		printf("Open file failed.\n");
	} else {
		tmp = num + 1;
		count = 0;
		while(tmp != 0) {  // 計算要 print 的數字是幾位數  
			tmp = tmp / 10;
			count++;
		}
		count = count + 2;  // print 時，「.」和「空格」佔得位置   
		fseek(wptr, 18 * num + count , SEEK_CUR);  // 下一行資料的開頭是 18 的倍數加上格式調整，因為一行資料有 17 個字元 ( 16 個字母 + 1 個換行 )  
		fprintf(wptr, "%d. %s\n", num + 1, data);
	}
	fclose(wptr);
}
