#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


unsigned char * memory;
int progcounter;
int size = 0;
unsigned char inputA;
unsigned char inputB;


int registerA;
int registerB;

int displacement;


char registers[8][5];

union converter{
	unsigned int integer;
	unsigned char bytes[4];
	
	
	
};

struct file{
	char * line;
	size_t length;
	ssize_t read;
	char* directive;
	char* address;
	char*content;
};

struct file data;

int dis(int x){//calculates displacement
	char bytes[12];
	x = (int) strtol(bytes,NULL,16);
	return x;
}
int lowerregistermove(unsigned char x, int z){//moves the lower register
	x = (memory[z] & 0xf0) >> 4;
	int rA = (int)x;
	
	return rA;
}
int upperregistermove(unsigned char y, int z){//moves the upper register
	y = (memory[z] & 0x0f);
	int rB = (int)y;
	return rB;

}

void rrmovl(){//moves rA into rB
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	progcounter+=1;
		
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
		
		
		
	printf("RRMOVL %s, %s\n",registers[registerA],registers[registerB]);
	progcounter+=1;
}

void l(){
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
}

void irmovl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;

	l();
       	progcounter+=1;
	int valRegs = (int) memory[progcounter];
	valRegs = valRegs - 0xf0;
		
	progcounter+=1;
			
		
	sprintf(bytes, "0x%.02x%.02x%.02x%.02x", memory[progcounter+3], memory[progcounter+2], memory[progcounter+1], memory[progcounter+0]);

	result = (int)strtol(bytes,NULL,16);
		
	printf("IRMOVL %d, %s\n",result,registers[valRegs]);
	progcounter+=4;
		
			
	
}



void rmmovl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;



	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	
	
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
	
	
		
	progcounter+=1;
		
	
	sprintf(bytes, "0x%.02x%.02x%.02x%.02x", memory[progcounter+3], memory[progcounter+2], memory[progcounter+1], memory[progcounter+0]);
	
	int displacement = dis(displacement);
		
	printf("RMMOVL  %s, (%s) %d\n",registers[registerA], registers[registerB], displacement);
	
	progcounter+=4;
		
}



void mrmovl()
{
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	
	progcounter+=1;
	
	
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
	
	
		
	progcounter+=1;
		
	
	
		
	printf("MRMOVL %d + %s, %s\n", displacement, registers[registerB], registers[registerA]);
	progcounter+=4;
		
}


void addl(){//prints items that were added
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("ADDL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;
}




void subl(){//prints items that were subtracted
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("SUBL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;
}




void andl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("ANDL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;
	
}




void xorl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("XORL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;
}


void mull(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("MULL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;
	

}



void cmpl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
		
	printf("CMPL %s, %s\n", registers[registerA], registers[registerB] );
	progcounter+=1;

}
//jump statements print the appropriate instruction with the displacement based on the PC


void jmp(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");
	
	progcounter+=1;

		
	
	
		
	printf("JMP, %d\n",displacement);
	progcounter+=4;
	

}



void jle(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		
	
	
		
	printf("JLE, %d\n",displacement);
	progcounter+=4;


}



void jl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		
	
	
		
	printf("JL, %d\n",displacement);
	progcounter+=4;
	

}



void je(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		

	
	
	
	printf("JE, %d\n",displacement);
	progcounter+=4;
	

}



void jne(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		
	
	
		
	printf("JNE, %d\n",displacement);
	progcounter+=4;
	

}



void jge(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		
	
	
		
	printf("JGE, %d\n",displacement);
	progcounter+=4;
	

}



void jg(){
	
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	int instruction = (int)memory[progcounter];
	
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

		
	
	
		
	printf("JG, %d\n",displacement);
	progcounter+=4;

}



void call(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;

	
	
			
	printf("CALL, %d\n", displacement);
	progcounter+=4;

}


void ret(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	printf("RET\n");
	

}


void pushl(){
	
	int instruction = (int)memory[progcounter];
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;
	
	
	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	printf("PUSHL, %s\n", registers[registerA]);
	progcounter+=1;


}


void popl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement;

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	printf("POPL, %s\n", registers[registerA]);
	progcounter+=1;
	

}




void readb(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	
	progcounter+=1;
	
	
		
	printf("READB, %d + %s\n",displacement, registers[registerA]);
	progcounter+=4;


}


void readl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	
	progcounter+=1;

	
	
		
	printf("READL, %d + %s\n",displacement, registers[registerA]);
	progcounter+=4;
	

}


void writeb(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	progcounter+=1;
	
	
		
	printf("WRITEB, %d + %s\n",displacement, registers[registerA]);
	progcounter+=4;
	

}


void writel(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	progcounter+=1;
	
	
		
	printf("WRITEL, %d + %s\n",displacement, registers[registerA]);
	progcounter+=4;
	

}


void movsbl(){
	int instruction = (int)memory[progcounter];
	
	unsigned char inputA;
	unsigned char inputB;


	int registerA;
	int registerB;

	int displacement = dis(displacement);

	char bytes[12];

	int result;
	strcpy(registers[0],"%eax");
	strcpy(registers[1],"%ecx");
	strcpy(registers[2],"%edx");
	strcpy(registers[3],"%ebx");
	strcpy(registers[4],"%esp");
	strcpy(registers[5],"%ebp");
	strcpy(registers[6],"%esi");
	strcpy(registers[7],"%edi");

	progcounter+=1;
	registerA = lowerregistermove(inputA,progcounter);
	registerB = upperregistermove(inputA,progcounter);
	progcounter+=1;

	
	
	printf("MOVSBL %d + %s, %s\n", displacement, registers[registerB], registers[registerA]);
	progcounter+=4;
	

}

void filememory (FILE* file){//populates the same way as in y86emul
	
	if (file == NULL){
		printf("No file could be opened.\n");
		exit(0);
	}
	
	data.line = NULL;
	data.length = 0;
	data.read;
	
	while((data.read = getline(&data.line, &data.length, file)) != -1){
		

	
	
		data.directive = strtok(data.line," 	");

		data.address = strtok(NULL,"	 ");
		size_t n =strlen(data.address)-1;
			if(data.address[n]=='\n'){
				data.address[n] = '\0';
			}

	
		data.content = strtok(NULL,"	");
		if(data.content!=NULL){
			size_t allocate = strlen(data.content)-1;
			if(data.content[allocate]=='\n'){
				data.content[allocate]='\0';
			}
		}	
	
	
		if (strcmp(data.directive,".size")==0){
			int size = (int)strtol (data.address, NULL, 16);
		
			
		
		
			 memory = (unsigned char *) malloc(sizeof(char)*size);
		}


		else if(strcmp(data.directive,".text")==0){
			int create = (int) strtol (data.address,NULL,16);
			progcounter = create;
		
			char byte[3];
		
		
		
		
			while(*data.content!='\0'){
			
				byte[0] = *data.content;
				data.content++;
				byte[1] = *data.content;
				data.content++;
				byte[2] = '\0';
				memory[create] = (char) strtol(byte,NULL,16);
				create++;
			}
			size = create;
		

		}

		else if(strcmp(data.directive,".string")==0){
			int create = (int) strtol (data.address,NULL,16);
			data.content ++;
			memory[create] = *data.content;
			data.content++;

			int i =1;
			while(*data.content!='\0'){
			
				memory[create+i] = *data.content;
				i++;
				data.content++;
			}
		}

		else if(strcmp(data.directive,".long")==0){
			int add = (int) strtol (data.address,NULL,16);
		

		
		
			unsigned int value = (int) strtol (data.content,NULL,16);
			union converter con;
		
			con.integer = value;
		
		

		

		
			memory[add] = con.bytes[0];
			memory[add+1] = con.bytes[1];
			memory[add+2] = con.bytes[2];
			memory[add+3] = con.bytes[3];
		
		
		
		}

		else if(strcmp(data.directive,".byte")==0){
			int add = (int) strtol (data.address,NULL,16);
		
		
			int bytedata = (int) strtol (data.content,NULL,16);
		
		
			memory[add] = (char) bytedata;
		
		
		}
	
		
		
		
		
	}
}

int main(int argc, char ** argv){
	

	if (strcmp(argv[1],"-h")==0){
		
		printf("Usage: y86emu file.y86.\n");
		exit(0);
		
	}
	if(argc!=2){
		
		printf("ERROR: Incorrect number of argumentss.\n");
		exit(0);
		
	}
	
	
	
	FILE *file;
	file = fopen(argv[1], "r");
	filememory(file);
	while(1){
		int instruction = (int)memory[progcounter];
	
		if (progcounter>=size){
			exit(0);
		}
    
	
	
	
		if (instruction == 0x00){
	    		printf("NOP\n");
			progcounter+=1;
				 
		
	  	}
	  	else if (instruction == 0x10){	
			printf("HALT\n");
			progcounter+=1;
		}
		

	  	else if (instruction == 0x20){ //rrmovl
			rrmovl();
		}
		else if (instruction == 0x30){ //irmovl
			irmovl();
				
		}
		else if (instruction == 0x40){ //rmmovl
			rmmovl();
		}
		
    		else if (instruction == 0x50){ //mrmovl
			mrmovl();
		}

		else if (instruction == 0x60){ //addl
			addl();
		}

		else if (instruction == 0x61){ //subl
			subl();
		}

		else if (instruction == 0x62){ //andl
			andl();
		}
		else if (instruction == 0x63){ //xorl
			xorl();
		}
		else if (instruction == 0x64){ //mull
			mull();
		}
		else if (instruction == 0x65){ //cmpl
			cmpl();
		}
			
      		else if (instruction == 0x70){ //jmp
			jmp();
		}

		else if (instruction == 0x71){ //jle
			jle();
		}

		else if (instruction == 0x72){ //jl
			jl();
		}
			
      		else if (instruction == 0x73){ //je
			je();
		}

		else if (instruction == 0x74){ //jne
			jne();
		}

		else if (instruction == 0x75){ //jge
			jge();
		}


		else if (instruction == 0x76){ //jg
			jg();
		}

		else if (instruction == 0x80){ //call
			call();
		}

		else if (instruction == 0x90){ //ret
			ret();
		}

		else if (instruction == 0xa0){ //pushl
			pushl();
		}

		else if (instruction == 0xb0){ //pop
			popl();
		}

		else if (instruction == 0xc0){ //readb
			readb();
		}

		else if (instruction == 0xc1){ //readl
			readl();
		}

		else if (instruction == 0xd0){ //writeb
			writeb();
		}

		else if (instruction == 0xd1){ //writel
			writel();
		}

		else if (instruction == 0xe0){ //movsbl
			movsbl();
      		}

		else{
			printf("ERROR: Check instruction.");
		
		}

	}

	fclose(file);
	

	
	return 0;
}

	
	


