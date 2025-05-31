// 函式的原型宣告  
void openFile(char *name, char *data, int num);
void HexadecimalToBinary(char *input, _Bool *output);
void keyGenerator(_Bool *keyWithParities, _Bool roundKeys[16][48], int *shiftTable);
void permute(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *outputBlock, int *table);
void split(int startBitNum, int endBitNum, _Bool *inBlock, _Bool *leftBlock, _Bool *rightBlock, _Bool **splitBlock);
void shiftLeft(_Bool *block, int numOfShifts);
void combine(int startBitNum, int endBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outBlock);
void Encrypt(_Bool *plainBlock, _Bool roundKeys[16][48], _Bool *cipherBlock);
void mixer(_Bool *leftBlock, _Bool *rightBlock, _Bool *roundKey);
void copy(int len, _Bool *input, _Bool *output);
void function(_Bool *inBlock, _Bool *roundKey, _Bool *outBlock);
void exclusiveOr(int len, _Bool *inputA, _Bool *inputB, _Bool *output);
void substitue(_Bool *inBlock, _Bool *outBlock, int SubstituteTable[8][4][16]);
void swapper(_Bool *leftBlock, _Bool *rightBlock, _Bool **splitBlock);
void BinaryToHexadecimal(int len, _Bool *input, char* output);
void writeFile(char *name, char *data, int num);
