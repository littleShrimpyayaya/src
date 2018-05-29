#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adpcm/adpcm.h"

static struct adpcm_state en_state_L,en_state_R,de_state_L,de_state_R;

#define FRAME_SIZE (1600)

// for encode, which includes left channel and right channel.
unsigned char inbuf[FRAME_SIZE];
short inencL[FRAME_SIZE/2];
short inencR[FRAME_SIZE/2];
unsigned char encbufL[FRAME_SIZE/2];
unsigned char encbufR[FRAME_SIZE/2];

// for decode
short decbufL[FRAME_SIZE];
short decbufR[FRAME_SIZE];
unsigned char outbufL[FRAME_SIZE*4];
unsigned char outbufR[FRAME_SIZE*4];

int main(int argc, char *argv[])
{
	FILE *f, *f2;
	char fname[64],fname2[64];

	if (argc < 3 )
	{
		printf("Usage: %s infile outfile\r\n", argv[0]);
		exit(1);
	}
	strcpy(fname, argv[1]);
	strcpy(fname2, argv[2]);

	if (NULL == (f2=fopen(fname2,"wb")) )
	{
		printf("Error opening '%s', terminating..\r\n", fname2);
		exit(4);
	}
	if (NULL == (f=fopen(fname,"rb")) )
	{
		printf("Error opening '%s', terminating..\r\n", fname);
		exit(4);
	}
	en_state_L.valprev = 0;
	en_state_L.index = 0;
	en_state_R.valprev = 0;
	en_state_R.index = 0;
	de_state_L.valprev = 0;
	de_state_L.index = 0;
	while(fread(inbuf, sizeof(unsigned char), sizeof(inbuf), f))
	{
		for(int i = 0; i < FRAME_SIZE; i += 2)
		{
			//inencL[i/4] = inbuf[i] | (inbuf[i+1] << 8);
			encbufL[i/2] = inbuf[i];
			encbufR[i/2] = inbuf[i+1];
		}
		printf("decode\n");
		adpcm_decoder(encbufL, decbufL, FRAME_SIZE, &de_state_L);
		adpcm_decoder(encbufR, decbufR, FRAME_SIZE, &de_state_R);

		for(int i=0;i < FRAME_SIZE;i++){ 
			outbufL[i*4] = decbufL[i] & 0xff; 
			outbufL[i*4+1] = decbufL[i] >> 8; 
			outbufL[i*4+2] = decbufR[i] & 0xff; 
			outbufL[i*4+3] = decbufR[i] >> 8; 
		}
		fwrite(outbufL, sizeof(unsigned char), sizeof(outbufL), f2);
	}
	fclose(f);
	fclose(f2);
	printf("Final valprev=%d, index=%d\r\n",
			en_state_L.valprev, en_state_L.index);

	return 0;
}
