/* by Shuhei Ajimura */

#ifdef __cplusplus
extern "C" {
#endif

int rbcp_ini(const char *, unsigned int);
int rbcp_fin();
int rbcp_wr(unsigned int , unsigned char* , unsigned char);
int rbcp_rd(unsigned int , unsigned char* , unsigned char);

#ifdef __cplusplus
};
#endif
