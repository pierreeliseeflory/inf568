#ifndef __POLY1305_CORE_H__
#define __POLY1305_CORE_H__

void poly1305_clamp(unsigned char [16]);

void poly1305(unsigned char [16], unsigned char [16], unsigned char *, unsigned int, unsigned char [16]);
    
#endif // __POLY1305_CORE_H__