#include "mycroft.h"

int bases[]={
	1,2,3,33,34,35,36,37,38,39,40
};

int percuss[]={
	5,6,13,19,28,41,47,7,8,32
};

int accomps[]={
	5,6,13,19,28,41,47,7,8,32
};

int synths[]={
	81,83,86,87,89,93,97,90,91
};


int drumChan;
int baseChan;
void play_notes() {
	
	/////////CHANNELS AND INSTRUMENTS
    baseChan=set_instr(ACOUSTIC_BASS+rand()%8);
    drumChan=set_instr(DRUMS);
    
/////////////////END CHANNELS AND INSTRUMENTS
	
	//Base sequence -- generally long
	const int loopLen=16;
	const int maxSteps=16;
	char seq[loopLen];
	int k=0;
	char bet[loopLen];
	for (int i=0;i<loopLen;i++){
		seq[i]=0;
		bet[i]=0;
	}
	for (int i=0;i<loopLen;i++)seq[rand()%maxSteps]++;
	//compressing beats
	for (int i=0;i<loopLen;i++)if (seq[i]){bet[k]=seq[i];k++;}
	
	//randomly take a note from our pentatonic to accompany the beat.
	for (int i=0;i<loopLen;i++)seq[i]=pentatonic[rand() % 6]+24;
	int finalLen=10000;
	const int volume = 127;
	int i=0;
	do{
		i++;
		play(0, baseChan, seq[i%k], volume);
	}while (play(25*bet[i%k], baseChan-0x10, seq[i%k], 0)<finalLen);
	
	
	//drums
	
	//reset everything
	play (0,-1,0,0);
	
	//pull another completely different rhythm?
	k=0;
	const int drumLoopLen=4;
	const int drumMaxSteps=4;
	for (int i=0;i<drumLoopLen;i++)seq[i]=0;
	for (int i=0;i<drumLoopLen;i++)seq[rand()%drumMaxSteps]++;
	for (int i=0;i<drumLoopLen;i++)bet[i]=0;
	for (int i=0;i<drumLoopLen;i++)if (seq[i]){bet[k]=seq[i];k++;}
	for (int i=0;i<drumLoopLen;i++)seq[i]=rand()%5+50;
	i=0;
	do{
		i++;
		play(0, drumChan, seq[i%k], volume);
	}while (play(25*bet[i%k], drumChan-0x10, seq[i%k], 0)<finalLen);
	
	
	
}

