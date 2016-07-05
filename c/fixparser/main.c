#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MSG_TYPE 35

struct my_context
{
	char* msg_type;
	int msg_type_len;
};

void on_start(void* context, int len)
{
	printf("start: %d\n", len);
}

void on_tag(void* context, int tag, char* value, int value_len)
{
	printf("tag: %d (%.*s)\n", tag, value_len, value);

	struct my_context* ctx = (struct my_context*) context;

	switch (tag)
	{
		case MSG_TYPE:
			ctx->msg_type = value;
			ctx->msg_type_len = value_len;
	}
}

void on_end(void* context)
{
	printf("end\n");
}

int main(int argc, char** argv)
{
	char msg[] = "8=FIX.4.2\x01""9=173\x01""35=D\x01""34=14\x01""49=LMAXMT\x01"
	             "52=20080108-19:41:12.859\x01""56=87654321\x01""57=MINIDEMO\x01"
	             "1=00217844\x01""11=12345678\x01""38=10000\x01""40=3\x01""54=1\x01"
	             "55=EUR/USD\x01""59=1\x01""60=20080108-19:41:12\x01""99=1.46909\x01"
	             "9041=7670249\x01""10=143\x01";
	struct my_context ctx;
	int result = push(msg, strlen(msg), (void*) &ctx, on_start, on_tag, on_end);

	printf("result: %d\n", result);
	if (result > 0)
	{
		printf("extracted: MSG_TYPE = %.*s\n", ctx.msg_type_len, ctx.msg_type);		
	}
}
