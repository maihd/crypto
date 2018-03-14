#define CRC_IMPL
#define BASE64_IMPL
#define HAVE_CRC_PRINT_TABLE 1

#include <stdio.h>
#include "base64.h"
#include <string.h>
#include "crc.h"

int main(int argc, char* argv[])
{
    int n;
    char src[128] = "Hello world";
    char dst[128];
    
    if ((n = base64_encode(src, strlen(src), dst, sizeof(dst))) < 0)
    {
	printf("Error!\n");
	return 1;
    }
    dst[n] = 0;
    printf("n: %d\n", n);
    printf("Src: %s\n", src);
    printf("Dst: %s\n", dst);
    
    if ((n = base64_decode(dst, strlen(dst), src, sizeof(src))) < 0)
    {
	printf("Error!\n");
	return 1;
    }
    src[n] = 0;
    printf("n: %d\n", n);
    printf("Src: %s\n", dst);
    printf("Dst: %s\n", src);
    printf("Src CRC: %u\n", crc_calc(src, strlen(src)));
    printf("Dst CRC: %u\n", crc_calc(dst, strlen(dst)));

    //crc_init();
    //crc_print_table();
    return 0;
}
