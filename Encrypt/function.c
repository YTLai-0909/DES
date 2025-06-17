#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "table.h"


/* 讀檔 
	name : 檔案名稱 
	data : 讀取的資料 
	num : 讀取的是第幾筆資料  
	回傳 : data  
*/ 
void readFile(char *name, char *data, int num) {
	
	FILE *rptr = fopen(name, "rb");  // 以位元組( Byte )為單位處理檔案 
	
	if(rptr == NULL) {
		printf("Read %s failed.\n\n", name);
		system("pause");  // 讓小黑框不會因為程式結束就消失 
		exit(EXIT_FAILURE);  // 結束程式  
	} else {
		printf("Read %s success.\n\n", name);
		fseek(rptr, 18 * num, SEEK_CUR);  // 下一行資料的開頭是 18 的倍數，因為一行資料有 17 個字元( 18 Bytes = 16 個字母( 16 Bytes ) + 1 個換行( 2 Bytes, \n = \r\n ) ) 
		fgets(data, 17, rptr);  // 因為會自動補上 '\0'，所以讀取的字元長度要放 17 ( 16 個字元 + 1 個 '\0' )
	}
	
//	printf("readFile = %s\n", data);
	
	fclose(rptr);
}

/* 16 進位轉 2 進位 
	input : 16 進位的字串  
	output : 轉成 2 進位的值  
	回傳 : output   
*/
void HexadecimalToBinary(char *input, _Bool *output) {
	
	/*
		len : 16 進位的字串長度 
		value : 16 進位轉成 10 進位的值  
	*/
	
	int len = 0, value = 0;
	int i, j;
	
	len = strlen(input);
	for(i = 0; i < len; i++) {
		// 16 進位 -> 10 進位
		if(input[i] >= '0' && input[i] <= '9') {  // 十六進位 0-9 -> 十進位要減 48( 0 的 ASCII = 48 )  
			value = input[i] - '0';
		} else if(input[i] >= 'A' && input[i] <= 'F') {  // 十六進位 A-F -> 十進位要減 55( A 的 ASCII = 65 ) 
			value = input[i] - 55;
		}
		// 10 進位 -> 2 進位  
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

/* 回合金鑰產生器  
	key : 初始金鑰 
	roundKeys : 產生的回合金鑰，16 個 
	回傳 : roundKeys  
*/
void keyGenerator(_Bool *key, _Bool roundKeys[16][48]) {
	
	/*
		tmpKey : 用來暫存金鑰做完同位元移除後的結果  
		leftKey : 分裂後的左邊區塊  
		rightKey : 分裂後的右邊區塊  
		tmpRoundKey : 用來暫存左右區塊結合後還沒做金鑰壓縮的結果  
	*/
	
	_Bool tmpKey[56] = {0};
	_Bool leftKey[28] = {0}, rightKey[28] = {0};
	_Bool tmpRoundKey[56] = {0};
	int i;
	
	// 同位元移除表  
	permute(64, 56, key, tmpKey, ParityDropTable);
	
	// 分裂 
	split(56, 28, tmpKey, leftKey, rightKey);
	
	// 產生回合金鑰，16 個  
	for(i = 0; i < 16; i++) {
		// 左位移  
		shiftLeft(leftKey, ShiftTable[i]);
		shiftLeft(rightKey, ShiftTable[i]);
		// 結合  
		combine(28, 56, leftKey, rightKey, tmpRoundKey);
		// 金鑰壓縮表  
		permute(56, 48, tmpRoundKey, roundKeys[i], KeyCompressionTable);		
	}
}

/* 排列 
	inputBitNum : 排列前的位元數 
	outputBitNum : 排列後的位元數 
	inputBlock : 排列前的區塊 
	outputBlock : 排列後的區塊 
	table : 排列表格  
	回傳 : outputBlock  
*/ 
void permute(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *outputBlock, int *table) {
	
	int i;
	
	for(i = 0; i < outputBitNum; i++) {
		outputBlock[i] = inputBlock[table[i] - 1];  // table[i] - 1 : 因為陣列從 0 開始，但 table 從 1 開始記錄  
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

/* 分裂 
	inputBitNum : 分裂前的位元數 
	outputBitNum : 分裂後的位元數 
	inputBlock : 分裂前的區塊 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	回傳 : leftBlock, rightBlock 
*/ 
void split(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *leftBlock, _Bool *rightBlock) {
	
	int i;
	
	for(i = 0; i < inputBitNum; i++) {
		if(i < inputBitNum / 2) {  // 左邊區塊  
			leftBlock[i] = inputBlock[i];	
		} else {  // 右邊區塊  
			rightBlock[i - outputBitNum] = inputBlock[i];  // 分裂後，inputBlock 的中間位元( = outputBitNum )變成 rightBlock 的第 0 位元  
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

/* 左位移 
	block : 要位移的區塊 
	numOfShifts : 要左移幾格 
	回傳 : block  
*/
void shiftLeft(_Bool *block, int numOfShifts) {
	
	/*
		tmp : 暫存陣列第 0 個位置的值，等做完左位移後，再放到陣列的尾端  
	*/
	
	_Bool tmp = 0;
	int i, j;
	
	for(i = 0; i < numOfShifts; i++) {
		tmp = block[0];  // 存陣列第 0 個位置的值  
		for(j = 1; j < 28; j++) {
			block[j - 1] = block[j];  // 左位移  
		}
		block[27] = tmp;  // 將第 0 個位置的值放到陣列的尾端  
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
	inputBitNum : 結合前的位元數 
	outputBitNum : 結合後的位元數 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	outputBlock : 結合的區塊 
	回傳 : outputBlock   
*/
void combine(int inputBitNum, int outputBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outputBlock) {
	
	int i;
	
	for(i = 0; i < outputBitNum; i++) {
		if(i < outputBitNum / 2) {  // 左邊區塊  
			outputBlock[i] = leftBlock[i];
		} else {  // 右邊區塊  
			outputBlock[i] = rightBlock[i - inputBitNum];  // 結合後，outputBlock 的中間位元( = inputBitNum )放得是 rightBlock 的第 0 位元  
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

/* DES 加密法 
	plainBlock : 明文 
	roundKeys : 回合金鑰 
	cipherBlock : 密文 
	回傳 : cipherBlock  
*/
void Encrypt(_Bool *plainBlock, _Bool roundKeys[16][48], _Bool *cipherBlock) {
	
	/*
		tmpPlainBlock : 暫存做完初始排列的結果  
		leftBlock : 分裂後的左邊區塊  
		rightBlock : 分裂後的右邊區塊  
		tmpCipherBlock : 暫存左右區塊結合後還沒做最終排列的結果  
	*/
	
	_Bool tmpPlainBlock[64] = {0};
	_Bool leftBlock[32] = {0}, rightBlock[32] = {0};
	_Bool tmpCipherBlock[64] = {0};
	int i;
	
	// 初始排列  
	permute(64, 64, plainBlock, tmpPlainBlock, InitialPermutationTable);
	
	// 分裂 
	split(64, 32, tmpPlainBlock, leftBlock, rightBlock);

	// DES 加密法，16 個回合  
	for(i = 0; i < 16; i++) {
		// 混合器  
		mixer(leftBlock, rightBlock, roundKeys[i]);
		if(i != 15) {  // 最後一回合不做交換器  
			// 交換器  
			swapper(leftBlock, rightBlock);
		}
	}
	
	// 結合  
	combine(32, 64, leftBlock, rightBlock, tmpCipherBlock);
	
	// 最終排列  
	permute(64, 64, tmpCipherBlock, cipherBlock, FinalPermutationTable);
}

/* 混合器  
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	roundKey : 回合金鑰 
	回傳 : leftBlock  
*/
void mixer(_Bool *leftBlock, _Bool *rightBlock, _Bool *roundKey) {
	
	/*
		tmp1 : 暫存右邊區塊的內容  
		tmp2 : 暫存做完 DES 函數的結果  
		tmp3 : 暫存做完 XOR 運算的結果  
	*/
	
	_Bool tmp1[32] = {0};
	_Bool tmp2[32] = {0};
	_Bool tmp3[32] = {0};
	
	// 複製右邊區塊的內容  
	copy(32, rightBlock, tmp1);
	
	// DES 函數  
	function(tmp1, roundKey, tmp2);
	
	// XOR 運算  
	exclusiveOr(32, leftBlock, tmp2, tmp3);
	
	// 將 mixer 的結果複製到左邊區塊  
	copy(32, tmp3, leftBlock);
}

/* 複製內容函式 
	inputBitNum : input 的位元數  
	input : 要複製的內容 
	output : 存複製的內容 
	回傳 : output  
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

/* DES 函數 
	inputBlock : 輸入的區塊 
	roundKey : 回合金鑰 
	outputBlock : 輸出的區塊 
	回傳 : outputBlock 
*/ 
void function(_Bool *inputBlock, _Bool *roundKey, _Bool *outputBlock) {

	/*
		tmp1 : 暫存做完擴展的 P-box 的結果  
		tmp2 : 暫存做完 XOR 運算的結果  
		tmp3 : 暫存做完 S-box 的結果  
	*/
	
	_Bool tmp1[48] = {0};
	_Bool tmp2[48] = {0};
	_Bool tmp3[32] = {0};
	
	// 擴展的 P-box  
	permute(32, 48, inputBlock, tmp1, ExpansionPermutationTable);
	
	// XOR 運算  
	exclusiveOr(48, tmp1, roundKey, tmp2);
	
	// S-box  
	substitue(tmp2, tmp3);
	
	// 連續的 P-box( 標準排列 ) 
	permute(32, 32, tmp3, outputBlock, StraightPermutationTable);
}

/* XOR 運算 
	inputBitNum : input 的位元數  
	inputA : 做 XOR 運算的陣列 
	inputB : 做 XOR 運算的陣列  
	output : 做完 XOR 運算的結果 
	回傳 : output  
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
	inputBlock : 輸入的區塊 
	outputBlock : 輸出的區塊 
	回傳 : outputBlock  
*/
void substitue(_Bool *inputBlock, _Bool *outputBlock) {
	
	/*
		row : SubstituteTable 的列( 由每個區塊的第一和第六位元組成 ) 
		col : SubstituteTable 的行( 由每個區塊的第二到第五位元組成 ) 
		value : 根據表格的編號、列、行，取得 SubstituteTable 對應的值  
	*/
	
	int row = 0, col = 0;
	int value = 0;
	int i, j;
	
	for(i = 0; i < 8; i++) {
		row = 2 * inputBlock[6 * i] + inputBlock[6 * i + 5];
		col = 8 * inputBlock[6 * i + 1] + 4 * inputBlock[6 * i + 2] + 2 * inputBlock[6 * i + 3] + inputBlock[6 * i + 4];
		value = SubstituteTable[i][row][col];
		for(j = 3; j >= 0; j--) {  // 將 10 進位的值轉成 2 進位，且每個區塊都倒著放入陣列中  
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

/* 交換器 
	leftBlock : 左邊區塊 
	rightBlock : 右邊區塊 
	回傳 : leftBlock, rightBlock 
*/
void swapper(_Bool *leftBlock, _Bool *rightBlock) {
	
	/*
		tmp : 做交換時用來當暫存用  
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

/* 2 進位轉 16 進位 
	inputBitNum : input 的位元數  
	input : 2 進位的值  
	output : 轉成 16 進位的字串 
	回傳 : output  
*/
void BinaryToHexadecimal(int inputBitNum, _Bool *input, char *output) {
	
	/*
		len : 2 進位轉成 16 進位後的字串長度 
		value : 2 進位轉成 10 進位的值  
	*/
	
	int len = 0, value = 0;
	int i;
	
	len = inputBitNum / 4;  // 2 進位的 4 個 bit = 16 進位的 1 個 bit，所以 inputBitNum / 4   
	
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
	num : 寫入的是第幾筆資料  
*/
void writeFile(char *name, char *data, int num) {
	
	FILE *wptr = fopen(name, "a");
	
	if(wptr == NULL) {
		printf("Write %s failed.\n\n", name);
		system("pause");  // 讓小黑框不會因為程式結束就消失 
		exit(EXIT_FAILURE);  // 結束程式  
	} else {
		printf("Write %s success.\n\n", name);
		fprintf(wptr, "%d. %s\n", num + 1, data);  // +1 : 因為陣列的第一個位置 index = 0  
	}
	fclose(wptr);
}
