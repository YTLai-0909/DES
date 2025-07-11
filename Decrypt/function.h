// 函式的原型宣告  
void readFile(char *name, char *data, int num);
void HexadecimalToBinary(char *input, _Bool *output);
void keyGenerator(_Bool *key, _Bool roundKeys[16][48]);
void permute(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *outputBlock, int *table);
void split(int inputBitNum, int outputBitNum, _Bool *inputBlock, _Bool *leftBlock, _Bool *rightBlock);
void shiftLeft(_Bool *block, int numOfShifts);
void combine(int inputBitNum, int outputBitNum, _Bool *leftBlock, _Bool *rightBlock, _Bool *outputBlock);
void Decrypt(_Bool *cipherBlock, _Bool roundKeys[16][48], _Bool *plainBlock);
void mixer(_Bool *leftBlock, _Bool *rightBlock, _Bool *roundKey);
void copy(int inputBitNum, _Bool *input, _Bool *output);
void function(_Bool *inputBlock, _Bool *roundKey, _Bool *outputBlock);
void exclusiveOr(int inputBitNum, _Bool *inputA, _Bool *inputB, _Bool *output);
void substitue(_Bool *inputBlock, _Bool *outputBlock);
void swapper(_Bool *leftBlock, _Bool *rightBlock);
void BinaryToHexadecimal(int inputBitNum, _Bool *input, char *output);
void writeFile(char *name, char *data, int num);

