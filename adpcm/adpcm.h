#ifndef _DVI4_ADPCM_H_
#define _DVI4_ADPCM_H_
/*
** adpcm.h - include file for adpcm coder.
**
** Version 1.0, 7-Jul-92.
*/

#ifdef __cplusplus
extern "C"
{
#endif

struct adpcm_state {
    short	valprev;	/* Previous output value */
    char	index;	 /* Index into stepsize table */
};

void adpcm_coder(short indata[], char outdata[], int len, struct adpcm_state *state);
void adpcm_decoder(char indata[], short outdata[], int len, struct adpcm_state *state);

#ifdef __cplusplus
}
#endif

#endif