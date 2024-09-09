#define MAXBUFSIZ 256

#ifdef __cplusplus
extern "C" {
#endif

#define A16 0x0000
#define A24 0x0100
#define A32 0x0200

#define D8  0x0000
#define D16 0x0400
#define D32 0x0800

#define BLT 0x0020
#define FIX 0x0080

#define VMEGread_a16d8(AA,BB,CC,DD) VMEGread(AA,A16|D8,BB,CC,DD)
#define VMEGread_a24d8(AA,BB,CC,DD) VMEGread(AA,A24|D8,BB,CC,DD)
#define VMEGread_a32d8(AA,BB,CC,DD) VMEGread(AA,A32|D8,BB,CC,DD)
#define VMEGread_a16d16(AA,BB,CC,DD) VMEGread(AA,A16|D16,BB,CC,DD)
#define VMEGread_a24d16(AA,BB,CC,DD) VMEGread(AA,A24|D16,BB,CC,DD)
#define VMEGread_a32d16(AA,BB,CC,DD) VMEGread(AA,A32|D16,BB,CC,DD)
#define VMEGread_a16d32(AA,BB,CC,DD) VMEGread(AA,A16|D32,BB,CC,DD)
#define VMEGread_a24d32(AA,BB,CC,DD) VMEGread(AA,A24|D32,BB,CC,DD)
#define VMEGread_a32d32(AA,BB,CC,DD) VMEGread(AA,A32|D32,BB,CC,DD)

#define VMEGwrite_a16d8(AA,BB,CC,DD) VMEGwrite(AA,A16|D8,BB,CC,DD)
#define VMEGwrite_a24d8(AA,BB,CC,DD) VMEGwrite(AA,A24|D8,BB,CC,DD)
#define VMEGwrite_a32d8(AA,BB,CC,DD) VMEGwrite(AA,A32|D8,BB,CC,DD)
#define VMEGwrite_a16d16(AA,BB,CC,DD) VMEGwrite(AA,A16|D16,BB,CC,DD)
#define VMEGwrite_a24d16(AA,BB,CC,DD) VMEGwrite(AA,A24|D16,BB,CC,DD)
#define VMEGwrite_a32d16(AA,BB,CC,DD) VMEGwrite(AA,A32|D16,BB,CC,DD)
#define VMEGwrite_a16d32(AA,BB,CC,DD) VMEGwrite(AA,A16|D32,BB,CC,DD)
#define VMEGwrite_a24d32(AA,BB,CC,DD) VMEGwrite(AA,A24|D32,BB,CC,DD)
#define VMEGwrite_a32d32(AA,BB,CC,DD) VMEGwrite(AA,A32|D32,BB,CC,DD)

#define VMEGread_a16d8_fix(AA,BB,CC,DD) VMEGread(AA,A16|D8|FIX,BB,CC,DD)
#define VMEGread_a24d8_fix(AA,BB,CC,DD) VMEGread(AA,A24|D8|FIX,BB,CC,DD)
#define VMEGread_a32d8_fix(AA,BB,CC,DD) VMEGread(AA,A32|D8|FIX,BB,CC,DD)
#define VMEGread_a16d16_fix(AA,BB,CC,DD) VMEGread(AA,A16|D16|FIX,BB,CC,DD)
#define VMEGread_a24d16_fix(AA,BB,CC,DD) VMEGread(AA,A24|D16|FIX,BB,CC,DD)
#define VMEGread_a32d16_fix(AA,BB,CC,DD) VMEGread(AA,A32|D16|FIX,BB,CC,DD)
#define VMEGread_a16d32_fix(AA,BB,CC,DD) VMEGread(AA,A16|D32|FIX,BB,CC,DD)
#define VMEGread_a24d32_fix(AA,BB,CC,DD) VMEGread(AA,A24|D32|FIX,BB,CC,DD)
#define VMEGread_a32d32_fix(AA,BB,CC,DD) VMEGread(AA,A32|D32|FIX,BB,CC,DD)

#define VMEGwrite_a16d8_fix(AA,BB,CC,DD) VMEGwrite(AA,A16|D8|FIX,BB,CC,DD)
#define VMEGwrite_a24d8_fix(AA,BB,CC,DD) VMEGwrite(AA,A24|D8|FIX,BB,CC,DD)
#define VMEGwrite_a32d8_fix(AA,BB,CC,DD) VMEGwrite(AA,A32|D8|FIX,BB,CC,DD)
#define VMEGwrite_a16d16_fix(AA,BB,CC,DD) VMEGwrite(AA,A16|D16|FIX,BB,CC,DD)
#define VMEGwrite_a24d16_fix(AA,BB,CC,DD) VMEGwrite(AA,A24|D16|FIX,BB,CC,DD)
#define VMEGwrite_a32d16_fix(AA,BB,CC,DD) VMEGwrite(AA,A32|D16|FIX,BB,CC,DD)
#define VMEGwrite_a16d32_fix(AA,BB,CC,DD) VMEGwrite(AA,A16|D32|FIX,BB,CC,DD)
#define VMEGwrite_a24d32_fix(AA,BB,CC,DD) VMEGwrite(AA,A24|D32|FIX,BB,CC,DD)
#define VMEGwrite_a32d32_fix(AA,BB,CC,DD) VMEGwrite(AA,A32|D32|FIX,BB,CC,DD)

#define VMEGread_a16d8_blt(AA,BB,CC,DD) VMEGread(AA,A16|D8|BLT,BB,CC,DD)
#define VMEGread_a24d8_blt(AA,BB,CC,DD) VMEGread(AA,A24|D8|BLT,BB,CC,DD)
#define VMEGread_a32d8_blt(AA,BB,CC,DD) VMEGread(AA,A32|D8|BLT,BB,CC,DD)
#define VMEGread_a16d16_blt(AA,BB,CC,DD) VMEGread(AA,A16|D16|BLT,BB,CC,DD)
#define VMEGread_a24d16_blt(AA,BB,CC,DD) VMEGread(AA,A24|D16|BLT,BB,CC,DD)
#define VMEGread_a32d16_blt(AA,BB,CC,DD) VMEGread(AA,A32|D16|BLT,BB,CC,DD)
#define VMEGread_a16d32_blt(AA,BB,CC,DD) VMEGread(AA,A16|D32|BLT,BB,CC,DD)
#define VMEGread_a24d32_blt(AA,BB,CC,DD) VMEGread(AA,A24|D32|BLT,BB,CC,DD)
#define VMEGread_a32d32_blt(AA,BB,CC,DD) VMEGread(AA,A32|D32|BLT,BB,CC,DD)

#define VMEGwrite_a16d8_blt(AA,BB,CC,DD) VMEGwrite(AA,A16|D8|BLT,BB,CC,DD)
#define VMEGwrite_a24d8_blt(AA,BB,CC,DD) VMEGwrite(AA,A24|D8|BLT,BB,CC,DD)
#define VMEGwrite_a32d8_blt(AA,BB,CC,DD) VMEGwrite(AA,A32|D8|BLT,BB,CC,DD)
#define VMEGwrite_a16d16_blt(AA,BB,CC,DD) VMEGwrite(AA,A16|D16|BLT,BB,CC,DD)
#define VMEGwrite_a24d16_blt(AA,BB,CC,DD) VMEGwrite(AA,A24|D16|BLT,BB,CC,DD)
#define VMEGwrite_a32d16_blt(AA,BB,CC,DD) VMEGwrite(AA,A32|D16|BLT,BB,CC,DD)
#define VMEGwrite_a16d32_blt(AA,BB,CC,DD) VMEGwrite(AA,A16|D32|BLT,BB,CC,DD)
#define VMEGwrite_a24d32_blt(AA,BB,CC,DD) VMEGwrite(AA,A24|D32|BLT,BB,CC,DD)
#define VMEGwrite_a32d32_blt(AA,BB,CC,DD) VMEGwrite(AA,A32|D32|BLT,BB,CC,DD)


int VMEGopen(char *, int);
void VMEGclose(int);

int VMEGread(int, unsigned short, unsigned int, void *, int);
int VMEGwrite(int, unsigned short, unsigned int, void *, int);

int VMEGread_req(int, unsigned short, unsigned int, void *, int, int);
int VMEGread_rcv(int, unsigned short, unsigned int, void *, int, int);

#ifdef __cplusplus
};
#endif
