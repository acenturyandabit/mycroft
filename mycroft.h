 #include <cstdlib>
 #include <algorithm>
 #include <utility>
 #include <cstring>
 #include <ctime>
 #include <cstdio>
 #include <vector>

 
FILE * fp1;
long int place = 0; //where in the stream we are.

int pentatonic[] = {
	45 - 12,
	48 - 12,
	50 - 12,
	52 - 12,
	55 - 12,
	57 - 12,
	45,
	48,
	50,
	52,
	55,
	57,
};

int program_number[10];
void play_notes(void);

typedef struct soundItem{
	int channel, pitch, volume;
	int operator< (const soundItem& other)const{
		return channel<other.channel;
	}
}soundItem;
std::vector<std::pair<unsigned long,soundItem> > playEvents;


std::vector<char>playBuffer;



void write(void * data, int len){
	for (int i=0;i<len;i++){
		char p=((char*)data)[i];
		playBuffer.push_back(p);
	}
}


//l: length since previosus event
//c: channel no.
//p: pitch?
//v: velocity.

int play(unsigned long int l, int c, int p, int v, bool abs=false){
	static unsigned long lastPos=0;
	//if channel is -1 then reset
	if (c==-1){
		lastPos=0;
		return lastPos;
	}
	
	//is the position absolute?
	//relative positions are stored.
	
	if (!abs)lastPos=lastPos+l;
	else lastPos=l;
	soundItem i={c,p,v};
	std::pair<unsigned long, soundItem> ni;
	ni.first=lastPos;
	ni.second=i;
	playEvents.push_back(ni);	
	return lastPos;
}

void playToBuf(unsigned long int l, int c, int p, int v) {
	unsigned char varbyte[4]={0,0,0,0};
	//convert to varbyte
	//for each block of 7:
  unsigned char abyte[4];
  unsigned char byte[4];
  memcpy(abyte,&l,4);
  for (int i=0;i<4;i++){
  	byte[i]=abyte[3-i];
  }

  unsigned char oldbuffer=0;
  unsigned char newbuffer;
  unsigned char temp1;
  for (int i=1;i<5;i++){
  	if (i!=4)newbuffer = (unsigned char) (byte[4-i] >> 8-i);
  	temp1 = byte[4-i];
  	temp1 = (unsigned char) (temp1 << i);
  	temp1 = (unsigned char) (temp1 >> 1);
  	temp1 = temp1 | oldbuffer;
  	varbyte[4-i] = temp1;
  	
  	oldbuffer=newbuffer;
  }
  /* now set the 7th bit */
  bool pre=false;
  for (int i=0;i<3;i++){
  	if (varbyte[i] > 0 || pre){
  		varbyte[i] = (unsigned char) (varbyte[i] | 0x80);
  		pre=true;
  		//print the last block
		write(&varbyte[i],1);
  	}
    
  }
	write(&varbyte[3],1);
	write(&c, 1);
	write(&p, 1);
	write(&v, 1);
}






int main() {
	srand(time(NULL));

	//something to do with encoding midi?
	char towrite[] = {0, 240, 5, 126, 127, 9, 1, 247};
	write(&towrite, 8);

	
	play(0, 152 - 16, 100, 0); /* Handshake */
	
	//actual songing goes here
	play_notes();
	//Transcription of a play array into a sequence of events.
	//sort the play array.	
	
	std::sort(playEvents.begin(),playEvents.end());

	//Calculate differences and play everything
	int preTime=0;
	for (int i=0;i<playEvents.size();i++){
		playEvents[i].first=playEvents[i].first-preTime;
		preTime=preTime + playEvents[i].first;
		playToBuf(playEvents[i].first, playEvents[i].second.channel, playEvents[i].second.pitch, playEvents[i].second.volume);
	}
	
	
	///////closing
	char end[] = { 0, 255, 47, 0};
	write (&end, 4);
	fp1=fopen("output.mid","wb");
		
	//chuck in the beginning of the file
	unsigned char beginning[]={77, 84, 104, 100, 0, 0, 0, 6, 0, 0, 0, 1, 0, 96, 77, 84, 114, 107};
	fwrite (&beginning, 1, 18, fp1);
	//write the length of the buffer here, including the end.
	long size=playBuffer.size();
	
	
	unsigned char abyte[4];
	unsigned char byte[4];
	memcpy(abyte,&size,4);
	for (int i=0;i<4;i++){
		byte[i]=abyte[3-i];
	}
	fwrite(&byte,1,4,fp1);
	
	char p;
	//now dump everything into the file
	for (long i=0;i<size;i++){
		p=playBuffer[i];
		fwrite(&p,1,1,fp1);
	}
	fclose(fp1);
	system("output.mid");
	return (1);
}


////instrument setup

enum instruments{
	DRUMS=-1,
	ACOUSTIC_GRAND_PIANO=1,
	BRIGHT_ACOUSTIC_PIANO,
	ELECTRIC_GRAND_PIANO,
	HONKY_TONK_PIANO,
	ELECTRIC_PIANO_1,
	ELECTRIC_PIANO_2,
	HARPSICHORD,
	CLAVINET,
	CELESTA,
	GLOCKENSPIEL,
	MUSIC_BOX,
	VIBRAPHONE,
	MARIMBA,
	XYLOPHONE,
	TUBULAR_BELLS,
	DULCIMER,
	DRAWBAR_ORGAN,
	PERCUSSIVE_ORGAN,
	ROCK_ORGAN,
	CHURCH_ORGAN,
	REED_ORGAN,
	ACCORDION,
	HARMONICA,
	TANGO_ACCORDION,
	ACOUSTIC_GUITAR_NYLON,
	ACOUSTIC_GUITAR_STEEL,
	ELECTRIC_GUITAR_JAZZ,
	ELECTRIC_GUITAR_CLEAN,
	ELECTRIC_GUITAR_MUTED,
	OVERDRIVEN_GUITAR,
	DISTORTION_GUITAR,
	GUITAR_HARMONICS,
	ACOUSTIC_BASS,
	ELECTRIC_BASS_FINGER,
	ELECTRIC_BASS_PICK,
	FRETLESS_BASS,
	SLAP_BASS_1,
	SLAP_BASS_2,
	SYNTH_BASS_1,
	SYNTH_BASS_2,
	VIOLIN,
	VIOLA,
	CELLO,
	CONTRABASS,
	TREMOLO_STRINGS,
	PIZZICATO_STRINGS,
	ORCHESTRAL_HARP,
	TIMPANI,
	STRING_ENSEMBLE_1,
	STRING_ENSEMBLE_2,
	SYNTH_STRINGS_1,
	SYNTH_STRINGS_2,
	CHOIR_AAHS,
	VOICE_OOHS,
	SYNTH_CHOIR,
	ORCHESTRA_HIT,
	TRUMPET,
	TROMBONE,
	TUBA,
	MUTED_TRUMPET,
	FRENCH_HORN,
	BRASS_SECTION,
	SYNTH_BRASS_1,
	SYNTH_BRASS_2,
	SOPRANO_SAX,
	ALTO_SAX,
	TENOR_SAX,
	BARITONE_SAX,
	OBOE,
	ENGLISH_HORN,
	BASSOON,
	CLARINET,
	PICCOLO,
	FLUTE,
	RECORDER,
	PAN_FLUTE,
	BLOWN_BOTTLE,
	SHAKUHACHI,
	WHISTLE,
	OCARINA,
	LEAD_SQUARE,
	LEAD_SAWTOOTH,
	LEAD_CALLIOPE,
	LEAD_CHIFF,
	LEAD_CHARANG,
	LEAD_VOICE,
	LEAD_FIFTHS,
	LEAD_BASS_LEAD,
	PAD_NEW_AGE,
	PAD_WARM,
	PAD_POLYSYNTH,
	PAD_CHOIR,
	PAD_BOWED,
	PAD_METALLIC,
	PAD_HALO,
	PAD_SWEEP,
	FX_RAIN,
	FX_SOUNDTRACK,
	FX_CRYSTAL,
	FX_ATMOSPHERE,
	FX_BRIGHTNESS,
	FX_GOBLINS,
	FX_ECHOES,
	FX_SCI_FI,
	SITAR,
	BANJO,
	SHAMISEN,
	KOTO,
	KALIMBA,
	BAGPIPE,
	FIDDLE,
	SHANAI,
	TINKLE_BELL,
	AGOGO,
	STEEL_DRUMS,
	WOODBLOCK,
	TAIKO_DRUM,
	MELODIC_TOM,
	SYNTH_DRUM,
	REVERSE_CYMBAL,
	GUITAR_FRET_NOISE,
	BREATH_NOISE,
	SEASHORE,
	BIRD_TWEET,
	TELEPHONE_RING,
	HELICOPTER,
	APPLAUSE,
	GUNSHOT
};




int set_instr(int id){
	static int num_alloc_chan=-1;
	num_alloc_chan++;
	if (num_alloc_chan==9)num_alloc_chan++;
	if (id==DRUMS){
		char temp[3];
		temp[0]=0;
		temp[1]=0xc9;
		temp[2]=5;
		write (&temp, 3);
		return 0x99;
	}else{
		char temp[3];
		temp[0]=0;
		temp[1]=0xc0+num_alloc_chan;
		temp[2]=id;
		write (&temp, 3);
		return 0x90+num_alloc_chan;
	}
}//returns the channel to which the instrument was allocated.











