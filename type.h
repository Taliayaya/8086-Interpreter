#ifndef _TYPE_H
#define _TYPE_H

#include <sys/types.h>

typedef void (*sighandler_t)(int16_t);


/* Types relating to messages. */
#define M1                 1
#define M3                 3
#define M4                 4
#define M3_STRING         14

/* MINIX 2 IS A 32-BYTE OPERATING SYSTEM
 * Hence, we have two adjust. 
 * - int => int16_t
 * - int * => uint16_t | /!\ pointers are 8 bytes here, but should be 4 in minix 
 */

typedef struct {
	int16_t m1i1; 
	int16_t m1i2; 
	int16_t m1i3; 

	uint16_t m1p1; 
	uint16_t m1p2; 
	uint16_t m1p3;
} __attribute__((packed))mess_1;

// uint8_t * maybe i am uint32
typedef struct {int16_t m2i1, m2i2, m2i3; int32_t m2l1, m2l2; uint32_t m2p1;} __attribute__((packed))mess_2;
typedef struct {int16_t m3i1, m3i2; uint16_t m3p1; char m3ca1[M3_STRING];} __attribute__((packed))mess_3;
typedef struct {int32_t m4l1, m4l2, m4l3, m4l4, m4l5;} __attribute__((packed))mess_4;
typedef struct {int8_t m5c1, m5c2; int16_t m5i1, m5i2; int32_t m5l1, m5l2, m5l3;}__attribute__((packed))mess_5;
typedef struct {int16_t m6i1, m6i2, m6i3; int32_t m6l1; sighandler_t m6f1;} __attribute__((packed))mess_6;

typedef struct {
  int16_t m_source;			/* who sent the message */
  int16_t m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
	mess_6 m_m6;
  } m_u;
}  message;


 /* The following defines provide names for useful members. */
 #define m1_i1  m_u.m_m1.m1i1
 #define m1_i2  m_u.m_m1.m1i2
 #define m1_i3  m_u.m_m1.m1i3
 #define m1_p1  m_u.m_m1.m1p1
 #define m1_p2  m_u.m_m1.m1p2
 #define m1_p3  m_u.m_m1.m1p3
 
 #define m2_i1  m_u.m_m2.m2i1
 #define m2_i2  m_u.m_m2.m2i2
 #define m2_i3  m_u.m_m2.m2i3
 #define m2_l1  m_u.m_m2.m2l1
 #define m2_l2  m_u.m_m2.m2l2
 #define m2_p1  m_u.m_m2.m2p1
 
 #define m3_i1  m_u.m_m3.m3i1
 #define m3_i2  m_u.m_m3.m3i2
 #define m3_p1  m_u.m_m3.m3p1
 #define m3_ca1 m_u.m_m3.m3ca1
 
 #define m4_l1  m_u.m_m4.m4l1
 #define m4_l2  m_u.m_m4.m4l2
 #define m4_l3  m_u.m_m4.m4l3
 #define m4_l4  m_u.m_m4.m4l4
 #define m4_l5  m_u.m_m4.m4l5
 
 #define m5_c1  m_u.m_m5.m5c1
 #define m5_c2  m_u.m_m5.m5c2
 #define m5_i1  m_u.m_m5.m5i1
 #define m5_i2  m_u.m_m5.m5i2
 #define m5_l1  m_u.m_m5.m5l1
 #define m5_l2  m_u.m_m5.m5l2
 #define m5_l3  m_u.m_m5.m5l3
 
 #define m6_i1  m_u.m_m6.m6i1
 #define m6_i2  m_u.m_m6.m6i2
 #define m6_i3  m_u.m_m6.m6i3
 #define m6_l1  m_u.m_m6.m6l1
 #define m6_f1  m_u.m_m6.m6f1

#endif

