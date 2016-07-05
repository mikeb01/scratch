int push(char* buf, int len, 
		 void* context, 
		 void (*startp)(void*, int),
		 void (*tagp)(void*, int, char*, int),
		 void (*endp)(void*));