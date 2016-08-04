/*
    2016 BTCDDev, based on Code from Come-from-Beyond
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cCurl.h"

void CurlInit(CurlCtx *ctx)
{
	for(int i = 0; i < STATE_SIZE + 1; ++i) ctx->Indices[i] = (i * 364) % 729;
	memset(ctx->State, 0x00, STATE_SIZE);
}

void CurlTransform(CurlCtx *ctx)
{
    for(int r = 0; r < 27; ++r)
    {
		int8_t StateCopy[STATE_SIZE + 1];
		memcpy(StateCopy, ctx->State, STATE_SIZE);
		
		for(int i = 0; i < STATE_SIZE; ++i)
			ctx->State[i] = T[StateCopy[ctx->Indices[i]] + (StateCopy[ctx->Indices[i + 1]] << 2) + 5];
	}
}

void Curl(CurlCtx *ctx, int8_t *input, int size)
{
	do
	{
		memcpy(ctx->State, input, size < HASH_SIZE ? size : HASH_SIZE);
		CurlTransform(ctx);
		input += HASH_SIZE;
		size -= HASH_SIZE;
	} while(size > 0);
}

void CurlSqueeze(CurlCtx *ctx, void *output)
{
    memcpy(output, ctx->State, HASH_SIZE);
    CurlTransform(ctx);
}

//Tests
/*
    To run:
    javac CurlReference.java
    uncomment from here on and run
*/

int run_test(int8_t in);

int main()
{

    printf("Running Test on [-1]: %s %d\n", run_test(-1) ==0 ? "PASS": "FAIL", run_test(-1));
    printf("Running Test on [0]: %s %d\n", run_test (0)  ==0 ? "PASS": "FAIL", run_test(0));
    printf("Running Test on [1]: %s %d\n", run_test (1)  ==0 ? "PASS": "FAIL", run_test(1));

    return 0;
}


int run_test(int8_t in)
{
	CurlCtx ctx;
	int8_t input[243], output[256], retVal = 1;
	char final_output[1024];
    if (in < -1 || in > 1)
    {
        fprintf(stderr, "Invalid input\n");
        return(-1);
    }
    
    CurlInit(&ctx);
    memset(input, in, HASH_SIZE);
    memset(output, 0x00, 256);
    
    memset(final_output, 0 , 1024 * sizeof(char));
    
    Curl(&ctx, input, HASH_SIZE);
    CurlSqueeze(&ctx, output);

    char temp[16];
    strcpy(final_output, "[");
    for(int i = 0; i < HASH_SIZE - 1; ++i)
    {
        sprintf(temp, "%d, ", output[i]);
        strcat(final_output, temp);
    }
    
    sprintf(temp, "%d]", output[HASH_SIZE-1]);
    strcat(final_output, temp);

    // Test
    
    FILE *fp;
    char ref[1024];
    memset(ref, 0, 1024);
    char sysCall[64];
    sprintf(sysCall, "java CurlReference %d", in);
    fp = popen(sysCall, "r");
    if(fp==NULL){
        fprintf(stderr, "Failed to run Reference Java Code\n");
        exit(1);
    }
    while(fgets(ref, sizeof(ref)-1, fp) != NULL)
        ;
    ref[strlen(ref)-1] = 0; //trailing \n from java
    pclose(fp);
    retVal = strcmp(final_output, ref);

    return retVal;
}

