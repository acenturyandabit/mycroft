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
    baseChan=set_instr(DRAWBAR_ORGAN+rand()%8);
    drumChan=set_instr(DRUMS);
    
	/////////////////END CHANNELS AND INSTRUMENTS
	
	//shuffle the pentatonics to get a random distinct 4 note sequence
	std::vector<int>seq;
	for (int i=0;i<6;i++){
		seq.push_back(pentatonic2[i]);
	}
	std::random_shuffle(seq.begin(),seq.end());
	for (int i=0;i<4;i++){
		seq[i]+=24;
	}
	//Base sequence -- chords repeated
	const int loopLen=4;
	int k=4;
	//randomly take a note from our pentatonic to accompany the beat.
	int ctype=rand()%CT_AUGMT;
	printf("%d",ctype);
	int i=0,p;
	int ctr;
	do{
		i++;
		play(0, baseChan, seq[i%k], volume);
		for (int j=0;j<3;j++)play(0, baseChan, chord(seq[i%k],j,ctype), volume);
		p=play(200, baseChan-0x10, chord(seq[i%k],0,ctype), volume);
		for (int j=1;j<3;j++)play(0, baseChan-0x10, chord(seq[i%k],j,ctype), volume);
	}while (p<finalLen);
	
	
	//drums
	//reset everything
	play (0,-1,0,0);
	int bet[4];
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

