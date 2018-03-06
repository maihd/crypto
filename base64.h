#ifndef __BASE64_H__
#define __BASE64_H__

#define __base64__

__base64__ int base64_encode(const unsigned char* src,
			     unsigned char* dst,
			     int src_size,
			     int dst_size);

__base64__ int base64_decode(const unsigned char* src,
			     unsigned char* dst,
			     int src_size,
			     int dst_size);

#ifdef BASE64_IMPLEMENTATION
/* BEGIN OF IMPLEMENTATION */

#define base64_value(idx) (base64_table[idx])

static const char base64_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/',
};

static unsigned char base64_index(unsigned int c)
{
    switch (c)
    {
    case '+': return 62;
    case '/': return 63;
    case '=': return  0;
    }

    if (c >= 'A' && c <= 'Z') return c - 'A' +  0;
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;

    return 0;
}

int base64_encode(const unsigned char* src,
		  unsigned char* dst,
		  int src_size,
		  int dst_size)
{
    int i, n;
    unsigned int   val[4];    /* 0 -> 2 : src, 3 : sum of 0 -> 2 */
    unsigned char* ptr = dst; /* result */
    
    /* The size of destination storage must be greater equal 
     * than the size of source storage
     */
    if (dst_size < (src_size / 3 + src_size % 3 ? 1 : 0) * 4)
    {
	return -1;
    }

    /* Convert 24bits segment to a 4bytes string
     */
    n = src_size - src_size % 3;
    for (i = 0; i < n; i += 3)
    {
	val[0] = *src++ << 16;
	val[1] = *src++ <<  8;
	val[2] = *src++ <<  0;
	val[3] = (val[0] | val[1] | val[2]);

	*ptr++ = base64_value((val[3] >> 18) & 0x3F);
	*ptr++ = base64_value((val[3] >> 12) & 0x3F);
	*ptr++ = base64_value((val[3] >>  6) & 0x3F);
	*ptr++ = base64_value((val[3] >>  0) & 0x3F);
    }

    /* Add padding characters 
     */
    switch (src_size % 3)
    {
    case 2:
	val[0] = *src++ << 8;
	val[1] = *src++ << 0;
	val[3] = val[0] | val[1];

	*ptr++ = base64_value((val[3] >> 10) & 0x3F);
	*ptr++ = base64_value((val[3] >>  4) & 0x3F);
	*ptr++ = base64_value((val[3] <<  2) & 0x3F);
	*ptr++ = '=';
	break;
	
    case 1:
	val[3] = *src;

	*ptr++ = base64_value((val[3] >>  2) & 0x3F);
	*ptr++ = base64_value((val[3] <<  4) & 0x3F);
	*ptr++ = '=';
	*ptr++ = '=';
	break;
    }
    
    return (int)(ptr - dst);
}

int base64_decode(const unsigned char* src,
		  unsigned char* dst,
		  int src_size,
		  int dst_size)
{
    int i, n;
    unsigned char  val[4];    /* Converted */
    unsigned char* ptr = dst; /* Result    */
    
    /* The size of destination storage must be greater equal 
     * than the size of source storage
     */
    if (dst_size < src_size / 4 * 3)
    {
	return -1;
    }

    /* Convert 4bytes string to a 24bits segment
     */
    n = src_size;
    for (i = 0; i < n; i += 4)
    {
	val[0] = (unsigned char)base64_index(*src++);
	val[1] = (unsigned char)base64_index(*src++);
	val[2] = (unsigned char)base64_index(*src++);
	val[3] = (unsigned char)base64_index(*src++);
	
	*ptr++ = (val[0] << 2) | (val[1] >> 4);
	*ptr++ = (val[1] << 4) | (val[2] >> 2);
	*ptr++ = (val[2] << 6) | (val[3] >> 0);
    }
    
    return (int)(ptr - dst);
}

#undef base64_value

/* END   OF IMPLEMENTATION */
#endif

#endif /* __BASE64_H__ */
