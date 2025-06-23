#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "function.h"

int main(void) {
    
    /* ��o��Ƽƥ� 
        num : �O���`�@���X�����  
    */ 
    int num = 0; 
    int i;
    
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
        */ 
        char inputKey[17] = {'\0'};
        _Bool key[64] = {0};
        _Bool roundKeys[16][48] = {0};
    
        // Ū�����_ 
        readFile("key.txt", inputKey, i);
        
        // 16 �i���� 2 �i�� 
        HexadecimalToBinary(inputKey, key);
        
        // ���ͦ^�X���_  
        keyGenerator(key, roundKeys);
        
        // �L�X�^�X���_�w���ͪ��T��  
        printf("Success: Round key generated.\n\n");
        
    /*---------------------------------------------------------------------------*/
    
        
    /*---------------------------------------------------------------------------*/
        
        printf("--------------------------------------------------\n");
    }
    
    system("pause");  // �{�������e�|���Ȱ��A���p�®ؤ��|�������� 
    
    return 0;
}

