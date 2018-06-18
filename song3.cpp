#include "mycroft.h"

int drumChan;
int baseChan;

enum chordTypes{
	CT_MAJOR,
	CT_MINOR,
	CT_DIMIS,
	CT_AUGMT
};
int chord(int base, int delta, int type=CT_MAJOR){
	int dev[][3]={
		{0,4,8},
		{0,4,7},
		{0,3,7},
		{0,3,6}
	};
	return base+(delta/3)*12+dev[type][delta%3];
	
}


int pentatonic2[] = {
	45 - 12,
	48 - 12,
	50 - 12,
	52 - 12,
	55 - 12,
	57 - 12,
};

void play_notes() {
	int finalLen=10000;
	const int volume = 127;
	/////////CHANNELS AND INSTRUMENTS
    drumChan=set_instr(DRUMS);
	//drums
	//reset everything
	play (0,-1,0,0);
	
	//pull another completely different rhythm?
	int k=0;
	
	const int drumLoopLen=8;
	const int drumMaxSteps=8;
	int seq[drumLoopLen];
	int bet[drumLoopLen];
	for (int i=0;i<drumLoopLen;i++)seq[i]=0;
	for (int i=0;i<drumLoopLen;i++)seq[rand()%drumMaxSteps]++;
	for (int i=0;i<drumLoopLen;i++)bet[i]=0;
	for (int i=0;i<drumLoopLen;i++)if (seq[i]){bet[k]=seq[i];k++;}
	//choose up to three instruments
	const int nDistInstr=4;
	int instr[nDistInstr];
	for (int i=0;i<nDistInstr;i++)instr[i]=rand()%55+35;
	for (int i=0;i<drumLoopLen;i++)seq[i]=instr[rand()%nDistInstr];
	int i=0;
	do{
		i++;
		play(0, drumChan, seq[i%k], volume);
	}while (play(25*bet[i%k], drumChan-0x10, seq[i%k], 0)<finalLen);
	
	
}

