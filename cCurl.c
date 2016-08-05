// Copyright 2016 Wolf (Wolf9466)

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
	int ret = run_test(-1);
	printf("Running Test on [-1]: %s %d\n",  ret == 0 ? "PASS": "FAIL", ret);
	
	ret = run_test(0);
	printf("Running Test on [0]: %s %d\n",   ret == 0 ? "PASS": "FAIL", ret);
	
	ret = run_test(1);
	printf("Running Test on [1]: %s %d\n", ret == 0 ? "PASS": "FAIL", ret);

	return 0;
}

int8_t TestVector0[] = { -1, 0, -1, 1, -1, -1, 0, 0, -1, -1, 0, 1, -1, 1, 0, 0, 1, 1, 1, -1, 1, -1, 1, 0, 0, -1, 0, -1, 1, 1, 1, 0, 1, 0, 0, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 0, 1, 0, -1, -1, -1, 0, -1, 1, 1, 1, -1, -1, 1, 1, 1, 1, 1, 0, -1, 0, 0, 1, 0, -1, 1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, -1, 1, 0, 1, 1, 0, 0, 1, -1, 1, 0, 1, 1, 1, 1, -1, -1, 0, 0, 0, -1, -1, 1, 0, 0, -1, 0, 1, 1, -1, -1, -1, 0, 1, -1, 1, -1, 1, 1, 1, 0, -1, -1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, -1, 1, -1, 1, 1, 1, 0, -1, 1, 1, -1, 0, 0, 1, 0, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 0, 1, 1, 0, -1, -1, 0, 1, 0, 0, -1, -1, -1, 1, 0, 0, 1, -1, -1, 0, 1, 0, 0, 0, 1, -1, 1, -1, 1, -1, 1, 0, 1, 0, 1, 1, 0, 1, -1, 1, 0, -1, 0, -1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, -1, 1, -1, 1, 1, 0, -1, 0, 0, -1, 1, -1, -1, 0, -1, 0, 1, -1, -1, 1, 1, 0, -1, 0};
int8_t TestVector1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int8_t TestVector2[] = { -1, 1, 1, 0, -1, -1, 0, 1, 1, -1, 1, -1, 1, -1, 1, 0, 0, 0, 0, -1, 0, 1, 0, -1, 1, 1, 0, -1, 1, -1, 1, -1, 0, -1, 0, 1, 0, -1, 1, 0, -1, 0, -1, -1, -1, 1, 1, -1, 1, -1, 0, 0, 0, 1, -1, 0, -1, 1, 0, -1, -1, 0, 1, -1, 0, 1, 1, 0, -1, -1, 0, 0, 0, -1, 1, 1, 1, 0, 1, 0, 1, 0, -1, -1, 0, -1, 0, -1, -1, 1, 1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 0, 0, 1, 0, -1, -1, 0, 0, -1, -1, -1, -1, 1, -1, 1, 0, 0, -1, -1, 1, 0, -1, 1, -1, -1, 0, 0, 1, -1, 1, -1, 1, -1, -1, -1, 0, 1, 1, -1, 1, 1, 0, 0, -1, 0, 1, -1, 1, 0, 1, -1, 1, 1, 0, -1, 0, -1, -1, 0, 0, 1, -1, -1, 1, -1, -1, 0, -1, 1, 0, 1, -1, -1, 1, 1, -1, 1, -1, 0, 1, 0, 1, -1, 1, 1, 0, 1, -1, -1, 1, 0, 1, -1, -1, -1, -1, 0, -1, 1, 0, -1, 1, 0, 1, 0, -1, 1, -1, 0, 1, 0, 1, -1, -1, -1, 0, -1, 1, 1, 0, -1, 1, 0, 0, -1, 1, -1, 0, 1, 1, -1, 0, 0, 0, 0};


int run_test(int8_t in)
{
	CurlCtx ctx;
	int8_t input[HASH_SIZE], output[HASH_SIZE];
	
	if (in < -1 || in > 1)
	{
		fprintf(stderr, "Invalid input\n");
		return(-1);
	}

	CurlInit(&ctx);
	memset(input, in, HASH_SIZE);
	memset(output, 0x00, HASH_SIZE);

	Curl(&ctx, input, HASH_SIZE);
	CurlSqueeze(&ctx, output);
	
	switch(in)
	{
		case -1:
			return(memcmp(TestVector0, output, HASH_SIZE));
		case 0:
			return(memcmp(TestVector1, output, HASH_SIZE));
		case 1:
			return(memcmp(TestVector2, output, HASH_SIZE));
	}
}

