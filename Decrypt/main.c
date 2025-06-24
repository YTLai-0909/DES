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
        
        /* DES 解密法( Decrypt ) 
            inputCipherText : 存讀取的密文內容( 字元形式 ) 
            cipherText : 密文( 位元形式 ) 
            plainText : 明文( 位元形式 ) 
            outputPlainText : 存明文的內容( 字元形式 ) 
        */
        char inputCipherText[17] = {'\0'};
        _Bool cipherText[64] = {0};
        _Bool plainText[64] = {0};
        char outputPlainText[17] = {'\0'};
        
        // 讀取密文 
        readFile("input.txt", inputCipherText, i);
        
        // 16 進位轉 2 進位 
        HexadecimalToBinary(inputCipherText, cipherText);
        
        // DES 解密 
        Decrypt(cipherText, roundKeys, plainText);
        
        // 2 進位轉 16 進位 
        BinaryToHexadecimal(64, plainText, outputPlainText);
        
        // 將明文寫入結果檔中 
        writeFile("output.txt", outputPlainText, i);
        
        // 印出解密完成的訊息 
        printf("Success: Decryption completed.\n\n");
        
    /*---------------------------------------------------------------------------*/
        
        printf("--------------------------------------------------\n");
    }
    
    system("pause");  // 程式結束前會先暫停，讓小黑框不會直接消失 
    
    return 0;
}

