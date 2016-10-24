#include <stdio.h>
#include <stdlib.h>

int Mem[100]={0};
int Reg[16]={0};
int program_count=0;

#define TRUE 1
#define FALSE 0

int IF(int ifid_reg[]);
void ID(int idex_reg[]);
void EX(int exmem_reg[]);
void MEM(int memwb_reg[]);
void WB(int wb_reg[]);

int main(void)
{
  Mem[0]=14593;
  Mem[1]=23186;
  Mem[2]=27555;
  Mem[3]=19636;
  Mem[4]=0;

  Reg[0]=17;
  Reg[1]=1;
  Reg[2]=3;
  Reg[3]=2;
  Reg[4]=23;


  int reg[4]={0};
  int i;
  int flag=TRUE;
  int op=0;

  while(flag==TRUE){
    flag=IF(reg);
    ID(reg);
    EX(reg);
    MEM(reg);
    WB(reg);
  }
for(i=0;i<16;i++){
  printf("Reg[%d]=%d\n",i,Reg[i]);
}


}

int IF(int ifid_reg[])
{//0~29までを命令空間,30~99までをデータ空間

  int op=0;
  ifid_reg[0]=Mem[program_count];
  program_count++;
  if(program_count<30){
    return TRUE;
  }else{
    return FALSE;
  }
}

void ID(int idex_reg[])
{
  //idex_reg[0]~idex_reg[3]
  int op=(idex_reg[0]>>12)&0x0f;

  if(op==0x1){//load
    idex_reg[1]=(idex_reg[0]>>8)&0x0f;//下位４bit
    idex_reg[2]=(idex_reg[0])&0xff;//下位８bit
    idex_reg[3]=0;
  }else if(op==0x2){//store
    idex_reg[1]=(idex_reg[0]>>8)&0x0f;
    idex_reg[2]=(idex_reg[0])&0xff;
    idex_reg[3]=0;
  }else{//ALU
    idex_reg[1]=(idex_reg[0]>>8)&0x0f;//A=B+C A
    idex_reg[2]=Reg[(idex_reg[0]>>4)&0x0f];//B
    idex_reg[3]=Reg[(idex_reg[0])&0x0f];//C
    //printf("%d\n%d\n%d\n\n",idex_reg[1],(idex_reg[0]>>4)&0x0f,(idex_reg[0])&0x0f);
  }
}

void EX(int exmem_reg[])
{
  //exmem_reg[0]~exmem_reg[3]
  int op=(exmem_reg[0]>>12)&0x0f;

  if(op==0x1){//load
    exmem_reg[2]=exmem_reg[2];
  }else if(op==0x2){//store
    exmem_reg[2]=exmem_reg[2];
  }else{
    if(op==0x3){//add
      exmem_reg[2]=exmem_reg[2]+exmem_reg[3];
    }else if(op==0x4){//sub
      exmem_reg[2]=exmem_reg[2]-exmem_reg[3];
    }else if(op==0x5){//mul
      exmem_reg[2]=exmem_reg[2]*exmem_reg[3];
    }else if(op==0x6){//div
      exmem_reg[2]=exmem_reg[2]/exmem_reg[3];
    }
  }

}

void MEM(int memwb_reg[])
{
  //memwb_reg[0]~memwb_reg[3]
  int op=(memwb_reg[0]>>12)&0x0f;
  if(op==0x1){//load
    memwb_reg[3]=Mem[memwb_reg[2]];
  }else if(op==0x2){//store
    Mem[memwb_reg[2]]=Reg[memwb_reg[1]];
  }

}

void WB(int wb_reg[]){
  int op=(wb_reg[0]>>12)&0x0f;

  if(op==0x1){//load
    Reg[wb_reg[1]]=wb_reg[3];
  }else if(op!=0x2){//store以外＝ALU
    Reg[wb_reg[1]]=wb_reg[2];
  }


}
