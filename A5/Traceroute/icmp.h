/**
 * Author: Rye Yao
 * E-mail: rye.y.cn@gmail.com
 * Date: 2013/04/08
 */
#ifndef __ICMP_H
#define __ICMP_H

#define ICMP_HEADER_LEN 8

short in_cksum( short *addr, register int len, short csum);

#endif
