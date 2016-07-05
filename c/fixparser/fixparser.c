#include <string.h>

const char* INITIAL_HEADER = "8=FIX.4.2\x01""9=";
const int INITIAL_HEADER_LEN = 12;
const int CHECK_SUM_LEN = 7;

static int calculate_checksum(char* s, int len)
{
	int sum;

	for (int i = 0; i < len; i++)
	{
		sum += s[i];
	}

	return sum & 255;
}

static int get_checksum(char* s)
{
	return ((s[0] - '0') * 100) + ((s[1] - '0') * 10) + (s[2] - '0');
}

static int validate_checksum(char* s, int len, char* chksum)
{
	int calculated = calculate_checksum(s, len);
	int stored = get_checksum(chksum);

	return calculated - stored;
}

int push(char* buf, int len, 
		 void* context, 
		 void (*startp)(void*, int),
		 void (*tagp)(void*, int, char*, int),
		 void (*endp)(void*))
{
	int position = 0;

	if (INITIAL_HEADER_LEN > len)
	{
		return 0;
	}

	char* current = buf;

	if (strncmp(current, INITIAL_HEADER, INITIAL_HEADER_LEN) != 0)
	{
		return -1;
	}

	position += INITIAL_HEADER_LEN;

	int msg_len = 0;

	while (position < len)
	{
		char c = buf[position];
		position++;

		if ('0' <= c && c <= '9')
		{
		 	msg_len = (msg_len * 10) + (c - '0');
		}
		else if (c == '\x01')
        {
            break;
        }
        else
        {
        	return -2;
        }
	}

	if (0 == msg_len || msg_len > (len - position) - CHECK_SUM_LEN)
	{
		return 0;
	}

	if (validate_checksum(&buf[position], msg_len, &buf[position + msg_len + 3]) != 0)
	{
		return -2;
	}

	(startp)(context, msg_len);

    do
    {
        int tag = 0;
        while (position < len)
        {
            char b = buf[position];
            position++;
            if ('0' <= b && b <= '9')
            {
                tag = (tag * 10) + (b - '0');
            }
            else if (b == '=')
            {
                break;
            }
            else
            {
                return -3;
            }            
        }
        
        int valueStartPosition = position;
        int valueEndPosition = -1;
        
        while (position < len)
        {
            char b = buf[position];
            position++;
            if (b == '\x01')
            {
                valueEndPosition = position - 1;
                break;
            }
        }
            
        if (valueEndPosition == -1)
        {
            return -4;
        }
        
        (tagp)(context, tag, &buf[valueStartPosition], valueEndPosition - valueStartPosition);
    }
    while (position < len);

    (endp)(context);

    return position;
}

