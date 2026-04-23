/*
 This is really ugly ZX emulation code which can barely run manic miner.

 Please do not distribute outside Bytefest hackathon group until the code is fixed
 
THIS SOFTWARE IS PROVIDED BY an anoumous author AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

*/

#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>

#include <linux/rtc.h>
#include <sys/time.h>

#include "z80emu.h"
#include "zxem.h"
#include "gui.h"
#include "pax.h"

unsigned char *memory;  // memory space including ROM, screenbuffer, etc.
unsigned char kbdlines[8];	// keyboard lines, as per http://www.breakintoprogram.co.uk/hardware/computers/zx-spectrum/keyboard

int emurun=0;		// Defines the status of emulation: 0=stopped, 1=running
int fntsel=0;		// Select font

int rotlcd=1;		// Rotate screen and shrink
int ba=0;		// Border active
int prtsz=0;		// Print size
int snden=0;		// Sound enable

unsigned long int lms,ms;	// Last millis and current millis
struct rtc_time rtc_tm; 
struct timeval tp;
static int rtc_fd = -1;

// Directory entries variables
DIR *d;
struct dirent *dir;
//char *filesList[n];

long dly;

int kbdjoy=0;		// Keyboard or joystick
int joyval=0;		// Default joy value
int intcnt=0;		// Interrupt counter
int flstate;		// Actual status of flash (0 or 1 alternates between INK and PAPER)
int firstrun=1;		// Are we running for 1st time? If yes, load snap and run emulator

char datetime[80];
int sndstate=0;
#define SNDBUF_SIZE 2*48
uint8_t sndbuf[SNDBUF_SIZE];
int rpl;

void *handle_touch( void *ptr );

static const char default_rtc[] = "/dev/rtc0";

Z80_STATE state;
/*
 Z80 in-port
*/
int
zxin (int port)
{
  int i, v;
  
  /*
    tehere is some kind of bug that prevents me from masking keystrokes
    if manic miner is reading multiple keylines at one time. this happens 
    at 0x8c5c, see https://skoolkid.github.io/manicminer/asm/8BDD.html
    so we force the last keyline with space to allow jumping
    
    this should be fixed for other games or if it becomes obvious that manic
    miner needs more keys than implemented now :)
  */
  if (port == 0x7efe)
    port = 0x7ffe;

  
  if (port == 0x00fe)
    port = 0xbffe;

  if ((port & 0xFF) == 0xFE)
    {
      v = 0x1f;
      for (i = 0; i < 8; i++)
	if ((port & (1 << (i + 8))) == 0)
	  {
	    v &= kbdlines[i];
#ifndef __arm__
          if (i != 7)
	    kbdlines[i] = 0xff;	//reset key
#endif
                return v;
	  }
      //return v;
    }

  // Handle joystick value
  if ((port & 0x001f) == 0x001f) {
	  return joyval;
  }
  // Debug information
  printf ("IN %X\n", port);
  return 0;
}

/*
 ZX OUT port
 
 this should be implemented for sound
*/
void
zxout (int port, int val)
{
  int x,y,color;
  static int pb=0;
  static int ofs=0;

  if ( ((port & 0xFF) == 0xFE) && (snden == 1)  ) {
//	  sndstate = val & (1<<4);
	  sndstate = val & 0x10;
	  sndbuf[ofs++] = sndstate ? 0x1f : 0x00;
	  if (ofs == SNDBUF_SIZE / 2) dsp_sound_synth(&sndbuf[0],SNDBUF_SIZE/2);
	  if (ofs >= SNDBUF_SIZE) {
		dsp_sound_synth(&sndbuf[SNDBUF_SIZE/2],SNDBUF_SIZE/2);
	  	ofs=0;
	  }

  }

  if ((port == 0xFE && ba == 1) && (rotlcd ==0)) {
	
  	color = val&0x07; 
	if (color !=pb ) {
	   for(y=0;y<25;y++)  { 
		for(x=0;x<=320;x++) {
			putpx(x,y,color);
			putpx(x,y+192+24,color);
		} // end X
	   } // end Y
	   //for(x=0;x<=320;x++) putpx(x,240,0);
	   for(x=0;x<33;x++) {
		for(y=24;y<192+24;y++) {
			putpx(x,y,color);
			putpx(x+256+32,y,color);
		} // end Y
	   } // end X
	} // end if

	pb=color;
  }
}

// Redraw block for attr change
//
void
redrawblock(int addr, int val) {

      int x,y,i,j,v;

      x = (addr - 0x5800) & 31;
      y = (addr - 0x5800)/32;

      // redraw 8x8 block
      for (j = 0; j < 8; j++) {
        v = memory[0x4000 + x + ( (((y*8+j)&7)<<8) | ((((y*8+j)>>3)&7)<<5) | ((((y*8+j)>>6)&3)<<11)  ) ];
      for (i = 0; i < 8; i++) { 
	//if( attr && 0x40 == 0x40 ) b=1; else b=0;
	//
	  if ( ((val & 0x80) == 0x80) && (flstate == 1) ) {
		  putpix ( (x * 8 + i), (y*8)+j, ((v << i) & 0x80) ? (val>>3&7) : (val&7));
	  } else {
		  putpix ( (x * 8 + i), (y*8)+j, ((v << i) & 0x80) ? (val&7) : ((val>>3)&7));
	  }
	  }
	 
	}
}

/*
 This handles writes to low memory to allow screen redraw. All changes are
 stored in global memory[] buffer
*/
void
z80lowmemwrite (int addr, int val)
{
  int x, y, i, attr;

  // http://www.breakintoprogram.co.uk/hardware/computers/zx-spectrum/screen-memory-layout

  // handle pixels
  if (addr >= 0x4000 && addr < (0x4000 + (192 * 32)))
    {
      x = (addr - 0x4000) & 31;
      y = (addr - 0x4000);
      y = ((y >> 8) & 7) | (((y >> 5) & 7) << 3) | ((y >> 11) & 3) << 6;
      
      attr = memory[0x5800 + x + 32*(y/8)];

      for (i = 0; i < 8; i++) {
	//if( attr && 0x40 == 0x40 ) b=1; else b=0;
	putpix ( (x * 8 + i), y, ((val << i) & 0x80) ?  (attr&7) : ((attr>>3)&7));
	}
    }
  else
  // handle color attributes
  if (addr >= 0x5800 && addr < (0x5800 + ((192/8) * 32))) redrawblock(addr,val);
    
  else
    {
     // this should not happen :)
      printf ("Unhandled lowmemwrite addr=%X val= %X\n", addr, val);
    }
}

#define Z80_SPH          13
#define Z80_SPL          12

#ifdef __arm__
#define main _init
#endif

int GetTime() {
	int retval;
	/* Read the RTC time/date */
        retval = ioctl(rtc_fd, RTC_RD_TIME, &rtc_tm);
        if (retval == -1) {
	                perror("RTC_RD_TIME ioctl");
	                exit(errno);
        }
	return retval;
}

void ShowTime() {
		//ZXPrint("System date/time:",0,192-16,fntsel,1,7);
		sprintf(datetime, "%02d-%02d-%4d",
				rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900);
		ZXPrint(datetime,176,0,fntsel,1,7);
		sprintf(datetime, "%02d:%02d:%02d",
			        rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
		ZXPrint(datetime,192,8,fntsel,1,7);
}

/* Save snapshot */ 
int SaveSNA(void) {
      int snasize = 49179;		//sb.st_size;
      int fd,res;

      unsigned char snadata[49179];

      char* fileName = "sna001.sna";

      fd = open(fileName,O_RDWR);

      if(fd == -1){
      	fprintf(stderr,"\nError Opening File!!\n");
	fflush(stderr);
        return(1);
      } else {   
        fprintf(stderr,"\nFile %s opened successfully!\n", fileName);
      }

      snadata[0] = state.i;
      snadata[1] = ((unsigned char *) state.alternates)[Z80_L];
      snadata[2] = ((unsigned char *) state.alternates)[Z80_H];
      snadata[3] = ((unsigned char *) state.alternates)[Z80_E];
      snadata[4] = ((unsigned char *) state.alternates)[Z80_D];
      snadata[5] = ((unsigned char *) state.alternates)[Z80_C];
      snadata[6] = ((unsigned char *) state.alternates)[Z80_B];
      snadata[7] = ((unsigned char *) state.alternates)[Z80_F];
      snadata[8] = ((unsigned char *) state.alternates)[Z80_A];
     
      snadata[9] = state.registers.byte[Z80_L];  
      snadata[10] = state.registers.byte[Z80_H];
      snadata[11] = state.registers.byte[Z80_E];
      snadata[12] = state.registers.byte[Z80_D];
      snadata[13] = state.registers.byte[Z80_C];
      snadata[14] = state.registers.byte[Z80_B];
      
      snadata[15] = state.registers.byte[Z80_IYL];
      snadata[16] = state.registers.byte[Z80_IYH];
      
      snadata[17] = state.registers.byte[Z80_IXL];
      snadata[18] = state.registers.byte[Z80_IXH];

      if (state.iff2 != 0) {
	      snadata[19] = 0;
      } else {
	      snadata[19] = 4;
      } 

      snadata[20] = state.r;
      snadata[21] = state.registers.byte[Z80_F];
      snadata[22] = state.registers.byte[Z80_A];
      
      snadata[23] = state.registers.byte[Z80_SPL];
      snadata[24] = state.registers.byte[Z80_SPH];
      snadata[25] = state.im;
      snadata[26] = 4;	// Border

      memcpy (&snadata[0x1b],&memory[0x4000],0xC000);

      fprintf(stderr,"Len: %d\n",sizeof(snadata));
      fflush(stderr);

      res = write(fd, snadata, snasize); 

      if (res != snasize) { 
	      fprintf(stderr,"SNA save error!\n");
      	      fflush(stderr);
      }

      close(fd); 

      return 0;
}

/* Load snap from given filename */
int LoadSNA(char *name) {

  unsigned char *snadata;
  int snasize;

  unsigned char *snap;
  int fd;

  snasize = 0;
  snadata = 0;

  if (name)
    {
      fd = open (name, O_RDONLY);
      //fstat(fd,&sb);
      snasize = 49179;		//sb.st_size;
      snadata = (unsigned char *) malloc (snasize);
      if (snasize != 49179)
	{
	  printf ("Warning: sna size not 49179 ! \n");
	  //exit (1);
	}
      if (read (fd, snadata, snasize) != snasize)
	printf ("sna error\n");
      close (fd);
    }

  if (snadata)
    {
      memcpy (&memory[0x4000], &snadata[0x1b], 0xC000);
      state.pc = 0x0072; // 0x0072;

      snap = snadata;
      
      state.i = *(snap++);
      ((unsigned char *) state.alternates)[Z80_L] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_H] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_E] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_D] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_C] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_B] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_F] = *(snap++); 
      ((unsigned char *) state.alternates)[Z80_A] = *(snap++); 
     
      state.registers.byte[Z80_L] = *(snap++); 
      state.registers.byte[Z80_H] = *(snap++); 
      state.registers.byte[Z80_E] = *(snap++); 
      state.registers.byte[Z80_D] = *(snap++); 
      state.registers.byte[Z80_C] = *(snap++); 
      state.registers.byte[Z80_B] = *(snap++); 
      
      state.registers.byte[Z80_IYL] = *(snap++); 
      state.registers.byte[Z80_IYH] = *(snap++); 
      
      state.registers.byte[Z80_IXL] = *(snap++); 
      state.registers.byte[Z80_IXH] = *(snap++); 
      
      if ((*(snap++) & 4) != 0)  // was 0
	{
	  state.iff1 = 1;
	  state.iff2 = 1;
	}
      else
	{
	  state.iff1 = 0;
	  state.iff2 = 0;
	}
      state.r = *(snap++);
      state.registers.byte[Z80_F] = *(snap++); 
      state.registers.byte[Z80_A] = *(snap++); 
      
      state.registers.byte[Z80_SPL] = *(snap++);  //was L
      state.registers.byte[Z80_SPH] = *(snap++);   // was H
      state.im =  *(snap++);
      zxout(254, *(snap++) & 7);

      state.pc = memory[state.registers.word[Z80_SP]];
      state.registers.word[Z80_SP]++; 
      state.pc += 256*memory[state.registers.word[Z80_SP]];
      state.registers.word[Z80_SP]++; 
    }

  return 0;
}

/* Cache directory content match the snap files and populate the list */
void Dir(void) {
    
    int n=0,i=0,x,m,cnt;
    char *ext;
    static int cur=0;

    d = opendir(".");
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        //zxprintf("%s\n", dir->d_name);
    	ext = strchr(dir->d_name, '.');
    	if ( ext && (!strcmp(ext, ".sna")) ) {
			printf("%s\n", dir->d_name);
			n++;
	}

      }
      rewinddir(d);

      char *fileList[n];

      while ((dir = readdir(d)) != NULL) {
	ext = strchr(dir->d_name, '.');

    	if ( ext && (!strcmp(ext, ".sna")) ) {
		fileList[i] = (char*) malloc (strlen(dir->d_name)+1);
	        strncpy (fileList[i],dir->d_name, strlen(dir->d_name) );	 
		i++;
	}
      }
    //for(i=0; i<=n; i++)
    //	      printf("%s\n", filesList[i]);
    
    ZXCls();
    ZXPrint("Select SNA to load:",0,0,fntsel,6,2);
    ZXPrint("<< FUNC, ALPHA >>, [0-9] Select",0,16,fntsel,0,7);
    
    char fname[30];
    char fnum[3];

    if (i < 10 ) {
	    cnt = i;
    } else {
	    cnt = (1+cur/10)*10;
    }

    for(m=0;m<30;m++) fname[m]='\0';
    /* Show one page of files if list is longer than 10 entries */
    if (cnt>10) cnt=10;
    for(x=0;x<cnt;x++) {
	    sprintf(fnum, "%2d", x);
	    ZXPrint(fnum,0,80+x*8,fntsel,0,7);

	    strncpy (fname, fileList[x+cur],strlen(fileList[x+cur]));
	    ZXPrint(fname,24,80+x*8,fntsel,0,7);
	    for(m=0;m<30;m++) fname[m]='\0';
    }

    sprintf(fname,"Entry: %d-%d of %d",cur+1,cur+10,n);
    ZXPrint(fname,0,192-8,fntsel,5,1);
    for(m=0;m<30;m++) fname[m]='\0';

    i = inkey();
    while (1) {
	    i = inkey();
      
/*
    if (cnt>10) cnt=10;
    for(i=0;i<cnt;i++) {
	    sprintf(fnum, "%2d", i);
	    ZXPrint(fnum,0,80+i*8,fntsel,0,7);

	    strncpy (fname, fileList[i+cur],strlen(fileList[i+cur]));
	    ZXPrint(fname,24,80+i*8,fntsel,0,7);
	    for(m=0;m<30;m++) fname[m]='\0';
    }*/
    if (i>1 && i<12) {
	   if (i==11) {
		  i=0;
	   } else {
		  i--;
	   }

	   if (i >= (n-cur) ) break;
	   //fname = fileList[i];
	   strncpy (fname, fileList[i+cur],strlen(fileList[i+cur]));
	   ZXPrint(fname,24,80+i*8,fntsel,4,0);
	   LoadSNA(fname);
	   ZXPrint("SNAP loaded, press 0 to start",24,40,fntsel,1,5);
	   break;

    }
    if (i == FUNC) {
	    if (cur >= 10) cur-=10;

    if (n-cur >= 10 ) cnt=10;

    ZXCls();
    ZXPrint("Select SNA to load:",0,0,fntsel,6,2);
    ZXPrint("<< FUNC, ALPHA >>, [0-9] Select",0,16,fntsel,0,7);
    if (cnt>10) cnt=10;
    for(x=0;x<cnt;x++) {
	    sprintf(fnum, "%2d", x);
	    ZXPrint(fnum,0,80+x*8,fntsel,0,7);

	    strncpy (fname, fileList[x+cur],strlen(fileList[x+cur]));
	    ZXPrint(fname,24,80+x*8,fntsel,0,7);
	    for(m=0;m<30;m++) fname[m]='\0';
    }
    sprintf(fname,"Entry: %d-%d of %d",cur+1,cur+10,n);
    ZXPrint(fname,0,192-8,fntsel,5,1);
    for(m=0;m<30;m++) fname[m]='\0';
    }

    if (i == ALPHA) {
	    if ((cur/10) < (n/10)) cur+=10;
    
    if ((cur/10) == (n/10)) cnt=n%10;

    ZXCls();
    ZXPrint("Select SNA to load:",0,0,fntsel,6,2);
    ZXPrint("<< FUNC, ALPHA >>, [0-9] Select",0,16,fntsel,0,7);
    for(x=0;x<cnt;x++) {
	    sprintf(fnum, "%2d", x);
	    ZXPrint(fnum,0,80+x*8,fntsel,0,7);

	    strncpy (fname, fileList[x+cur],strlen(fileList[x+cur]));
	    ZXPrint(fname,24,80+x*8,fntsel,0,7);
	    for(m=0;m<30;m++) fname[m]='\0';
    }
    sprintf(fname,"Entry: %d-%d of %d",cur+1,(cur+10 > n) ? n : cur+10,n);
    ZXPrint(fname,0,192-8,fntsel,5,1);
    for(m=0;m<30;m++) fname[m]='\0';
    }
    } // End of while
   } // End of select

    closedir(d);
}

/* Load default rom file */
int LoadROM(void) {

  int romsize;
  char *romfilename;
  romfilename = "zx48.rom";

  int fd;
  fd = open (romfilename, O_RDONLY);
  //fstat(fd,&sb);
  romsize = 16384;		//sb.st_size;
  if (read (fd, memory, romsize) != romsize || romsize == 0)
    {
      printf ("rom error\n");
      exit (1);
    }
  close (fd);

  return 0;
}

/* Read the value of touchscreen coordinates */
void *handle_touch( void *ptr ) {

     while  (1) 
       handle_event();

}

/* Emulate beeper */
void *do_sound( void *ptr ) {

//#define SNDBUF_SIZE 2*48000
//	uint8_t sndbuf[SNDBUF_SIZE];
	//int sndstate;
/*	struct timeval s_tp;
	unsigned long lusec,usec,ms,s=0;
	int off=0,pl_at=0;
	int lstate=0;
	//uint8_t tmpbuf[SNDBUF_SIZE];	// Buffer 2
	int chunk=48;
	int i,rpl=0;

  	gettimeofday(&s_tp, NULL);
  	lusec = s_tp.tv_usec;
*/
	while (1) {
	  	// Test if ready to play
		if (rpl > 0)  {
			//dsp_sound_synth(&sndbuf[(rpl -1) ? 0 : SNDBUF_SIZE / 2 ],SNDBUF_SIZE/2);
			rpl=0;
		}

		__asm__("nop");
		/*
//	memset(sndbuf,0x80,SNDBUF_SIZE);
        for(i=0;i<SNDBUF_SIZE;i++) {
		if (i % (48)) lstate = !lstate;
	        sndbuf[i] = lstate ? 0x1f : 0x00;
	}
	
	while(1)  {


		dsp_sound_synth(&sndbuf[0],SNDBUF_SIZE / 2);


	while (1) {
	   

	  	gettimeofday(&s_tp, NULL);
  		usec = s_tp.tv_usec;

		if (abs(usec-lusec) >= 1000) {
			ms++;
			lusec = usec;

		if (ms >= 1000) {
			ms=0;
			if (!s) pl_at=0; else pl_at=SNDBUF_SIZE / 2;
			s = !s;
			rpl=1;
		}
		}

	   	//for(lp=0;lp<2;lp++) __asm__("nop");
		//__asm__("nop");
		//if (off % chunk ) lstate = !lstate;
		//sndbuf[off] = ( sndstate == 0 ) ? 0x1f : 0x00;
		//sndbuf[off] = lstate ? 0x1f : 0x00;
		//off++;
		//lusec = usec;
			if (off == (SNDBUF_SIZE / 2)) {
				rpl=1;
				pl_at=0;
			}

		if (off >= SNDBUF_SIZE) {
			off=0;
			pl_at=SNDBUF_SIZE/2;
			rpl=2;
		}
		}

	  //}


*/	
	} // Endless while
} // End of pthread 

/* Main menu including emulation */
void *menu( void *ptr ) {

  int cycles;
  int i;
  unsigned char c;
  int curr_sec, prev_sec;
  char datetime[80];

  memory = (unsigned char *) malloc (0x10000);
  memset (memory, 0, 0x10000);

  // Init keyboard and emulate no key press initially
  kbd_init();
  //for (i = 0; i < 8; i++)
  //  kbdlines[i] = 0x1f;

  emurun=0;
  fntsel=1;

  screen_init ();

  flstate = 0;

  LoadROM();
  Z80Reset (&state);
  state.pc = 0x0000;

  ShowMenu();

  GetTime();
  prev_sec = rtc_tm.tm_sec;

  ShowTime();
  if (firstrun) LoadSNA("sna001.sna");

  /* Manage menu */
  while (1) {
     	if (!firstrun) i = inkey();
        if (i == 2) {
		zxout(254,0);
		scr2lcd(0);
		for(dly=0;dly<20000000;dly++) { __asm__("nop"); }
		zxout(254,7);
		ZXCls();
		ShowMenu();
	}

	if ( i == 3 ) {
		emurun = 0;
		kbd_init();
		SaveSNA();
	   	ZXPrint("sna001.sna saved, press 0 to ret",0,176,fntsel,1,5);
		emurun = 1;
	}

	if ( i == 4 ) Dir();
  	/* Manage options in menu */
	if ( i == 10 ) {
		ShowOpts();
		c = ( rotlcd == 0 ) ? '0' : '1';
		ZXChar(c,184,96,fntsel,6,2);
		c = ( fntsel == 0 ) ? '0' : '1';
		ZXChar(c,184,96-8,fntsel,6,2);
		c = ( ba == 0 ) ? '0' : '1';
		ZXChar(c,184,96+8,fntsel,6,2);
		c = 48 + prtsz; 
		ZXChar(c,184,96+16,fntsel,6,2);
		c = ( snden == 0 ) ? '0' : '1';
		ZXChar(c,184,96+24,fntsel,6,2);

		while (1) {
			i = inkey();
			if ( i == 3 ) {
				rotlcd = (rotlcd == 0) ? 1 : 0;
				if (rotlcd == 0) ba=1;
				ShowMenu();
				break;
			}

			if ( i == 2 ) {
				fntsel = (fntsel == 0) ? 1 : 0;
				ShowOpts();
				c = ( fntsel == 0 ) ? '0' : '1';
				ZXChar(c,184,96-8,fntsel,6,2);
			}

			if ( i == 5 ) {
				prtsz++;
				if (prtsz > 2) prtsz=0;
				ShowOpts();
				c = 48 + prtsz; 
				ZXChar(c,184,96+16,fntsel,6,2);
			}

			if ( i == 6 ) {
				snden = (snden == 0) ? 1 : 0;
				ShowOpts();
				c = ( snden == 0 ) ? '0' : '1';
				ZXChar(c,184,96+24,fntsel,6,2);
			}

			if ( i == 11 ) {
				ShowMenu();
				break;
			}
		}
	}
	if ( i == FUNC ) {

		if (kbdjoy != 0) {
			ZXChr('K',232,312,1,1,5);
			ZXChr('B',224,312,1,1,5);
			ZXChr('D',216,312,1,1,5);
			kbdjoy=0;
		} else {
			ZXChr('J',232,312,1,2,6);
			ZXChr('O',224,312,1,2,6);
			ZXChr('Y',216,312,1,2,6);
			kbdjoy=1;
		}

	}
	if ( (i == 6) || (i == 11) || firstrun ) 
/* Emulate ZX Spectrum */
{
 LoadROM();	// Load ROM again in case it was accidentialy overwritten

 if (rotlcd ==0) { 
  	scr2lcd(1);
  } else {
	tkbd2lcd(0);
	scr2lcd(1);
  	kbd_init();
  }

  cycles = 0;

  if (i == 6) { 
	state.pc = 0x0000; 
  	//Z80Reset (&state);
  }

  emurun=1;

  gettimeofday(&tp, NULL);
  lms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

  while (emurun==1)
    {
      gettimeofday(&tp, NULL);
      ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

      cycles += Z80Emulate (&state, 200 /*cycles */ , NULL);

      if (!snden) {
      	for (int o = 0; o < (300); o++) __asm__("nop"); //zpomlait
      } else {
      	for (int o = 0; o < (300); o++) __asm__("nop"); //zpomlait
      }

      /*
       This is really ugly hack to fire ZX refresh interrupt
       Changing this constant probably changes speed of the game. Who knows
       I don't know how the game should behave. 
       
       This also handles keyboard, but it should be done in
       an independent thread
       
	fprintf(stderr,"Before interrupt: b=%lums c=%lums\n",lms,ms);
	fflush(stderr);
	*/

        if  (abs(ms-lms) > 20 )
	//(cycles > 1024*10)
	{
	  handle_x(); // handle keyboard (keypad)
	  Z80Interrupt (&state, 0, NULL);
	  cycles = 0;
	  intcnt++;
	  lms = ms;
	  //fprintf(stderr,"Inside interrupt: %lums\n",ms);
	  //fflush(stderr);
	}

      	if (intcnt == 25) {
		flstate=1;
		doflash();

	}
	if (intcnt == 50) {
		flstate=0;
		doflash();
		intcnt=0;
	}
    }

  ShowMenu();
  i=0;
  prev_sec=-1;
  if (firstrun) firstrun=0;
}
/* End of emulation option */

	if (i == ESC) {
		i = 0;
		ZXPrint("Sure to exit? Green to confirm",0,184,fntsel,4,0);

		while (i <= 0) {
			i = inkey();
			if (i == ENTER ) exit (1);
		}
		//return 0;
  	}

	// BLOCKING! - ak sa nasledujuci riadok odkomentuje, bude sa vyzadovat 
	// kliknutie na touchscreen predtym ako sa otestuju kody klavesnice
	//handle_event();
	
	sprintf(datetime,"X= %d, Y= %d     ",realx,realy);
	ZXPrint(datetime,0,192-24,fntsel,0,7);

	GetTime();
	curr_sec = rtc_tm.tm_sec;
	if (curr_sec > prev_sec) {

		ShowTime();
		//ZXPrint("System date/time:",0,192-16,fntsel,1,7);
		sprintf(datetime, "%02d-%02d-%4d",
				rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900);
		ZXPrint(datetime,176,0,fntsel,1,7);
		sprintf(datetime, "%02d:%02d:%02d",
			        rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
		ZXPrint(datetime,192,8,fntsel,1,7);
		if (curr_sec == 59) {
			prev_sec = -1;
		} else {
			prev_sec = curr_sec;
		}


	}
  }
}

/* Main loop start */
int
main ()
{
  long x;

  const char *rtc = default_rtc;

  pthread_t thread1, thread2, thread3;
  char *message1 = "Touchpad";
  char *message2 = "Menu";
  char *message3 = "Sound";
  int  iret1, iret2, iret3,i;

  dev_init();			    // Initialize devices

  rtc_fd = open(rtc, O_RDONLY);
        if (rtc_fd ==  -1) {
                fprintf(stderr,"Error init RTC: %s!\n",rtc);
                fflush(stderr);
                exit(errno);
        }
	i = fcntl(rtc_fd, F_GETFL,0);
	fcntl(rtc_fd, F_SETFL, i | O_NONBLOCK);

  for(x=0;x<10000;x++) __asm__("nop"); // Wait cycle before running threads

  while (1) {
	/* Create independent threads each of which will execute function */

	iret1 = pthread_create( &thread1, NULL, handle_touch, (void*) message1);
	iret2 = pthread_create( &thread2, NULL, menu, (void*) message2);
	iret3 = pthread_create( &thread3, NULL, do_sound, (void*) message3);

	/* Wait till threads are complete before main continues. Unless we  */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);

        fprintf(stderr,"Thread 1 returns: %d\n",iret1);
        fprintf(stderr,"Thread 2 returns: %d\n",iret2);
        fprintf(stderr,"Thread 3 returns: %d\n",iret3);
        fflush(stderr);

  }
}
