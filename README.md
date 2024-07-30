# 8086-Interpreter

## NAME

mmvm - Basic program that allows to disassemble or interpret an Intel 8086 Assembly program compiled in 16-bit.

## Installation

In the root of the project, execute
```
make
```
This command will create new directories. The output binary is located at `bin/z124043_mmvm`

## SYNOPSIS

```
./bin/z124043_mmvm binary [binary options]
./bin/z124043_mmvm -m binary [binary options]
./bin/z124043_mmvm -d binary [binary options]
```

Execute the binary program. 

- `-m`  Execute and display the program command and registers/flags states
- `-d`  Display the binary as a dissambled program. 

## STRUCTURE
```
├── bin                                   // binary output
├── build
├── docs                                  // intel specifications
├── include
├── lib                            
│   └── minix
│       └── include                       // headers modified to simulate minix2 environment
│           └── minix
├── src
└── tests
    ├── tests_bin                         // precompiled 16-bit 8086 ASM binary
    │   └── asm
    ├── tests_ref
    │   ├── disassemble
    │   └── interpret_debug
    └──  tests_source
```

## EXAMPLE

```
$ ./bin/z124043_mmvm tests/tests_bin/5.c.out arg1 arg2 arg3
argv[0]=tests/tests_bin/5.c.out
argv[1]=arg1
argv[2]=arg2
argv[3]=arg3

````

```
$ ./bin/z124043_mmvm -d tests/tests_bin/5.c.out arg1 arg2 arg3
0000: 31ed         xor bp, bp
0002: 89e3         mov bx, sp
0004: 8b07         mov ax, [bx]
0006: 8d5702       lea dx, [bx+2]
0009: 8d4f04       lea cx, [bx+4]
000c: 01c1         add cx, ax
000e: 01c1         add cx, ax
0010: bb1600       mov bx, 0016
0013: 81fb1a02     cmp bx, 021a
0017: 730f         jnb 0028
0019: f6c301       test  bl, 1
001c: 750a         jne 0028
001e: 813f5353     cmp [bx], 5353
0022: 7504         jne 0028
0024: 891e0200     mov [0002], bx
0028: 8b1e0200     mov bx, [0002]
...
12de: 0000         add [bx+si], al
12e0: 00           (undefined)
```

```
./bin/z124043_mmvm -m tests/tests_bin/5.c.out arg1 arg2 arg3
 AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP
0000 0000 0000 0000 ffb5 0000 0000 0000 ---- 0000:31ed         xor bp, bp
0000 0000 0000 0000 ffb5 0000 0000 0000 --Z- 0002:89e3         mov bx, sp
0000 ffb5 0000 0000 ffb5 0000 0000 0000 --Z- 0004:8b07         mov ax, [bx] ;[ffb5]0004
0004 ffb5 0000 0000 ffb5 0000 0000 0000 --Z- 0006:8d5702       lea dx, [bx+2] ;[ffb7]ffc5
0004 ffb5 0000 ffb7 ffb5 0000 0000 0000 --Z- 0009:8d4f04       lea cx, [bx+4] ;[ffb9]ffdd
0004 ffb5 ffb9 ffb7 ffb5 0000 0000 0000 --Z- 000c:01c1         add cx, ax
0004 ffb5 ffbd ffb7 ffb5 0000 0000 0000 -S-- 000e:01c1         add cx, ax
0004 ffb5 ffc1 ffb7 ffb5 0000 0000 0000 -S-- 0010:bb1600       mov bx, 0016
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 -S-- 0013:81fb1a02     cmp bx, 021a
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 -S-C 0017:730f         jnb 0028
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 -S-C 0019:f6c301       test  bl, 1
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 001c:750a         jne 0028
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 001e:813f5353     cmp [bx], 5353 ;[0016]5353
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 0022:7504         jne 0028
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 0024:891e0200     mov [0002], bx ;[0002]0238
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 0028:8b1e0200     mov bx, [0002] ;[0002]0016
0004 0016 ffc1 ffb7 ffb5 0000 0000 0000 --Z- 002c:890f         mov [bx], cx ;[0016]5353
...
0000 ff8b 0003 0001 ff75 ff75 ff8b 0000 --Z- 10ba:cd20         int 20
<exit(13)>

```
