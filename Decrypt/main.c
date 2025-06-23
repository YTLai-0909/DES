#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
    
    /* 獲得資料數目 
        num : 記錄總共有幾筆資料  
    */ 
    int num = 0; 
    int i;
    
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
        */ 
        char inputKey[17] = {'\0'};
        _Bool key[64] = {0};
        _Bool roundKeys[16][48] = {0};
    
        // 讀取金鑰 
        readFile("key.txt", inputKey, i);
        
        // 16 進位轉 2 進位 
        HexadecimalToBinary(inputKey, key);
        
        // 產生回合金鑰  
        keyGenerator(key, roundKeys);
        
        // 印出回合金鑰已產生的訊息  
        printf("Success: Round key generated.\n\n");
        
    /*---------------------------------------------------------------------------*/
    
        
    /*---------------------------------------------------------------------------*/
        
        printf("--------------------------------------------------\n");
    }
    
    system("pause");  // 程式結束前會先暫停，讓小黑框不會直接消失 
    
    return 0;
}

