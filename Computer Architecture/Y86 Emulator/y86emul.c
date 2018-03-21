#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "y86emul.h"
#define AOK "Executing properly\n"
#define HLT "Halt Instruction\n"
#define ADR "Invalid address\n"
#define INS "Invalid instruction was given\n"



unsigned char * memory;

int last = 0;
int action = 0;
char cread = ' ';
boolean of;
boolean zf;
boolean sf;
int memsize;
#define ESP 4
struct hold {
	int rgs[8];
	int pc;
};


union converter{
	unsigned int integer;
	unsigned char bytes[4];
	
	
	
};
struct hold registers;
struct file{
	char * line;
	size_t length;
	ssize_t read;
	char* directive;
	char* address;
	char*content;
};
struct file data;


void rrmovl(){//sets up registers and moves A into B
	unsigned char registerA;
	unsigned char registerB;
	int result = 0;
	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	registers.rgs[registerB]=registers.rgs[registerA];
	registers.pc+=2;
}


void irmovl(){//indirect
       	unsigned char registerA;
	unsigned char registerB;
	int result = 0;
  	registerB=memory[registers.pc+1]&0x0f;
	result=0;
				
	result =memory[registers.pc+2] +(memory[registers.pc+3]<<8) +(memory[registers.pc+4]<<16) +(memory[registers.pc+5]<<24);
	registers.rgs[registerB]=result;
	registers.pc+=6;
			
	
}



void rmmovl(){

	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
  	result=0;
	result =memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
						
	memory[registers.rgs[registerB]+result]=(registers.rgs[registerA]&0x000000ff);
	memory[registers.rgs[registerB]+result+1]=(registers.rgs[registerA]&0x0000ff00)>>8;
	memory[registers.rgs[registerB]+result+2]=(registers.rgs[registerA]&0x00ff0000)>>16;
	memory[registers.rgs[registerB]+result+3]=(registers.rgs[registerA]&0xff000000)>>24;
	registers.pc+=6;
	
}



void mrmovl()
{
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result =memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
				
	registers.rgs[registerA]=(memory[registers.rgs[registerB]+result]) + (memory[registers.rgs[registerB]+result+1]<<8) + (memory[registers.rgs[registerB]+2]<<16) + (memory[registers.rgs[registerB]+3]<<24);
	registers.pc+=6;
	
}


void addl(){//calculates add instruction, sets flags
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=registers.rgs[registerA]+registers.rgs[registerB];
	of=false;
	zf=false;
	sf=false;
	if((registers.rgs[registerA]>0&&registers.rgs[registerB]>0&&result<0)||(registers.rgs[registerA]<0&&registers.rgs[registerB]<0&&result>0)){
		of=true;
	}
	if(result<0){
		sf=true;
	}
	if(result==0){
		zf=true;
	}
	registers.rgs[registerB]=result;
	registers.pc+=2;
}




void subl(){//calculate subtraction instructions, set flags
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=registers.rgs[registerB]-registers.rgs[registerA];
	of=false;
	zf=false;
	sf=false;
	if((registers.rgs[registerA]<0&&registers.rgs[registerB]>0&&result<0)|| (registers.rgs[registerA]>0&&registers.rgs[registerB]<0&&result>0)){
		of=true;
	}
	if(result<0){
		sf=true;
	}
	if(result==0){
		zf=true;
	}
				
			
	registers.rgs[registerB]=result;
	registers.pc+=2;
	
}




void andl(){//calculates andl intructions, sets flags
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;

	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=registers.rgs[registerA]&registers.rgs[registerB];
	zf=false;
	sf=false;
	if(result<0){
		sf=true;
	}
	if(result==0){
		zf=true;
	}
	registers.pc+=2;
	
}




void xorl(){//moves registers A and B, calculates result of XORL of the registers, sets flags
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=registers.rgs[registerA]^registers.rgs[registerB];
	zf=false;
	sf=false;
	if(result<0){
		sf=true;
	}
	if(result==0){
		zf=true;
	}
	registers.pc+=2;
}


void mull(){//moves registers A and B, multiplies them, sets flag, stores result in register B
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=registers.rgs[registerA]*registers.rgs[registerB];
	of=false;
	zf=false;
	sf=false;
	if((registers.rgs[registerA]>0 && registers.rgs[registerB]>0 && result<0)|| (registers.rgs[registerA]<0 && registers.rgs[registerB]<0 && result<0)||(registers.rgs[registerA]>0 && registers.rgs[registerB]<0 && result>0)||(registers.rgs[registerA]<0 && registers.rgs[registerB]>0 && result>0)){
		of=true;
	}
	if(result<0){
		sf=true;
	}
	if(result==0){
		zf=true;
	}

	registers.rgs[registerB]=result;
	registers.pc+=2;
	

}



void cmpl(){//moves registers, sets flags based on result of comparison
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	if(registers.rgs[registerA]==registers.rgs[registerB]){
		zf=true;
	}else{
		zf=false;
	}
	if(registers.rgs[registerB]-registers.rgs[registerA]<0){
		sf=true;
	}else{
		sf=false;
	}
	registers.pc+=2;
	

}



void jmp(){//sets pc based shift
	unsigned char registerA;
	unsigned char registerB;
	int result = 0;
	result=0;
	result+=memory[registers.pc+1];
	result+=memory[registers.pc+2]<<8;
	result+=memory[registers.pc+3]<<16;
	result+=memory[registers.pc+4]<<24;
				
	registers.pc=result;
	

}



void jle(){//either sets pc to result if less than or equal too or moves to next instructions
	unsigned char registerA;
	unsigned char registerB;
	int result = 0;
	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if(zf==1||(sf!=of)){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}


}



void jl(){
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
 	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if((sf!=of)){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}
	

}



void je(){//jumps if equal or just moves on
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if(zf==1){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}
	

}



void jne(){//jumps if not equal
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if(zf==0){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}
	

}



void jge(){//jumps if greater than or equal to
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;

  	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if(zf==of){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}
	

}



void jg(){//jumps if greater
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
				
	if(zf==0&&(sf==of)){
		registers.pc=result;
	}else{
		registers.pc+=5;
	}
	

}



void call(){//moves stack pointer down as it calls.
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	result=0;
	result=memory[registers.pc+1] + (memory[registers.pc+2]<<8) + (memory[registers.pc+3]<<16) + (memory[registers.pc+4]<<24);
	registers.rgs[ESP]-=4;
				
	action=registers.pc+5;
	memory[registers.rgs[ESP]]=(action&0x000000ff);
	memory[registers.rgs[ESP]+1]=(action&0x0000ff00)>>8;
	memory[registers.rgs[ESP]+2]=(action&0x00ff0000)>>16;
	memory[registers.rgs[ESP]+3]=(action&0xff000000)>>24;
	registers.pc=result;

}


void ret(){//adds 4 back to stack pointer.
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registers.pc=(memory[registers.rgs[ESP]]) + (memory[registers.rgs[ESP]+1]<<8) + (memory[registers.rgs[ESP]+2]<<16) + (memory[registers.rgs[ESP]+3]<<24);
							
	registers.rgs[ESP]+=4;
	

}


void pushl(){//sets up registers, moves stack pointer down 4, moves pc up 2
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	registers.rgs[ESP]-=4;
				
	memory[registers.rgs[ESP]]=(registers.rgs[registerA]&0x000000ff);
	memory[registers.rgs[ESP]+1]=(registers.rgs[registerA]&0x0000ff00)>>8;
	memory[registers.rgs[ESP]+2]=(registers.rgs[registerA]&0x00ff0000)>>16;
	memory[registers.rgs[ESP]+3]=(registers.rgs[registerA]&0xff000000)>>24;
				
	registers.pc+=2;


}


void popl(){//sets up registers, sets up registeer A, moves stack pointer up 4
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
 	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	registers.rgs[registerA]=(memory[registers.rgs[ESP]]) + (memory[registers.rgs[ESP]+1]<<8) + (memory[registers.rgs[ESP]+2]<<16) + (memory[registers.rgs[ESP]+3]<<24);
							
	registers.rgs[ESP]+=4;
	registers.pc+=2;
	

}




void readb(){//sets up registers, if scanned correctly flag it true, cread as the result
	unsigned char registerA;
	unsigned char registerB;
	int result = 0;
	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
				
	if(EOF==scanf("%c",&(cread))){
		zf=true;
	}else{
		zf=false;
	}
	memory[registers.rgs[registerA]+result]=cread;
	registers.pc+=6;


}


void readl(){//same as readb except
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
  	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
				
	if(EOF==scanf("%i",&(action))){
		zf=true;
	}else{
		zf=false;
	}
				
	memory[registers.rgs[registerA]+result]=(action&0x000000ff);
	memory[registers.rgs[registerA]+1+result]=(action&0x0000ff00)>>8;
	memory[registers.rgs[registerA]+2+result]=(action&0x00ff0000)>>16;
	memory[registers.rgs[registerA]+3+result]=(action&0xff000000)>>24;
				
	registers.pc+=6;
	

}


void writeb(){//sets up registers and calculate and prints the result
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	registerA=memory[registers.pc+1]>> 4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
				
	printf("%c",memory[registers.rgs[registerA]+result]);
	registers.pc+=6;
	

}


void writel(){//same except
	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	int val=0;
	result=memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
				
	val=(memory[registers.rgs[registerA]+result]) + (memory[registers.rgs[registerA]+1+result]<<8) + (memory[registers.rgs[registerA]+2+result]<<16) + (memory[registers.rgs[registerA]+3+result]<<24);
							
	printf("%i",val);
	registers.pc+=6;
	

}


void movsbl(){
	unsigned char registerA;
  	unsigned char registerB;
	int result = 0;
	registerA=memory[registers.pc+1]>>4&0xff;
	registerB=memory[registers.pc+1]&0x0f;
	result=0;
	result=memory[registers.pc+2] + (memory[registers.pc+3]<<8) + (memory[registers.pc+4]<<16) + (memory[registers.pc+5]<<24);
	int x=0;
	int y=0;
	x=(memory[registers.rgs[registerB]+result]) + (memory[registers.rgs[registerB]+result+1]<<8) + (memory[registers.rgs[registerB]+2]<<16) + (memory[registers.rgs[registerB]+3]<<24);
	y=x&0x000000ff;
	registers.rgs[registerA]=y;
  	registers.pc+=6;
	

}

void computeinstructions(){//executes instructions based on opcode
	boolean execute=true;
  	unsigned char operationcode;
  	unsigned char registerA;
  	unsigned char registerB;
  	int result = 0;
	while(execute){
		operationcode=memory[registers.pc];
		if (operationcode == 0x00){ //nop
			registers.pc ++;
		}
		else if (operationcode == 0x10){ //halt
			printf("\nProgram stopped at %x. HLT\n", registers.pc);
			execute=false;
			registers.pc++;
		}
		else if (operationcode == 0x20){ //rrmovl
			rrmovl();
		}
		else if (operationcode == 0x30){ //irmovl
			irmovl();
				
		}
		else if (operationcode == 0x40){ //rmmovl
			 rmmovl();
		}
		
    		else if (operationcode == 0x50){ //mrmovl
			mrmovl();
		}

		else if (operationcode == 0x60){ //addl
			addl();
		}

		else if (operationcode == 0x61){ //subl
			subl();
		}

		else if (operationcode == 0x62){ //andl
			andl();
		}
		else if (operationcode == 0x63){ //xorl
			xorl();
		}
		else if (operationcode == 0x64){ //mull
			mull();
		}
		else if (operationcode == 0x65){ //cmpl
			cmpl();
		}
			
      		else if (operationcode == 0x70){ //jmp
			jmp();
		}

		else if (operationcode == 0x71){ //jle
			jle();
		}

		else if (operationcode == 0x72){ //jl
			jl();
		}
			
      		else if (operationcode == 0x73){ //je
			je();
		}

		else if (operationcode == 0x74){ //jne
			jne();
		}

		else if (operationcode == 0x75){ //jge
			 jge();
		}


		else if (operationcode == 0x76){ //jg
			jg();
		}

		else if (operationcode == 0x80){ //call
			call();
		}

		else if (operationcode == 0x90){ //ret
			ret();
		}

		else if (operationcode == 0xa0){ //pushl
			 pushl();
		}

		else if (operationcode == 0xb0){ //pop
			popl();
		}

		else if (operationcode == 0xc0){ //readb
			readb();
		}

		else if (operationcode == 0xc1){ //readl
			readl();
		}

		else if (operationcode == 0xd0){ //writeb
			writeb();
		}

		else if (operationcode == 0xd1){ //writel
			writel();
		}

		else if (operationcode == 0xe0){ //movsbl
			movsbl();
      		}
		else{
			fprintf(stderr, INS);
			execute=false;
		}
	}

	  registerA=0;
	  registerB=0;
}



void filememory (FILE* file){
	int size;
	if (file == NULL){
		printf("No file could be opened.\n");
		exit(0);
	}
	
	data.line = NULL;
	data.length = 0;
	data.read;
	
	while((data.read = getline(&data.line, &data.length, file)) != -1){//loop to populate memory
		

	
	
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
			int size = (int)strtol (data.address, NULL, 16);//allocate memory with size directive
		
			
		
		
			 memory = (unsigned char *) malloc(sizeof(char)*size);
		}


		else if(strcmp(data.directive,".text")==0){
			int create = (int) strtol (data.address,NULL,16);
			registers.pc = create;
		
			char byte[3];
		
		
		
		
			while(*data.content!='\0'){//organizes op codes
			
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
			while(*data.content!='\0'){//while not at \0 populate string directive
			
				memory[create+i] = *data.content;
				i++;
				data.content++;
			}
		}

		else if(strcmp(data.directive,".long")==0){//long directive
			int add = (int) strtol (data.address,NULL,16);
		

		
		
			unsigned int value = (int) strtol (data.content,NULL,16);
			union converter con;//converson makes it easier to populate
		
			con.integer = value;
		
		

		

		
			memory[add] = con.bytes[0];
			memory[add+1] = con.bytes[1];
			memory[add+2] = con.bytes[2];
			memory[add+3] = con.bytes[3];
		
		
		
		}

		else if(strcmp(data.directive,".byte")==0){//byte directive
			int add = (int) strtol (data.address,NULL,16);
		
		
			int bytedata = (int) strtol (data.content,NULL,16);
		
		
			memory[add] = (char) bytedata;
		
		
		}
	
		
		
		
		
	}
}

int main(int argc, char ** argv){//main
	

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
	computeinstructions();

	fclose(file);

	return 0;
}

