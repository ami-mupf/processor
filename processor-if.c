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
  Mem[0]=4126;
  Mem[1]=20752;
  Mem[2]=13109;
  Mem[3]=29236;
  Mem[4]=8479;

  Reg[0]=0;
  Reg[1]=1;
  Reg[2]=5;
  Reg[3]=0;
  Reg[4]=1;
  Reg[5]=1;

  Mem[30]=9;

  int reg[4]={0};
  int i,j=0;
  int flag=TRUE;
  int op=0;

  while(flag==TRUE){
  //while(j<11){
    flag=IF(reg);
    ID(reg);
    EX(reg);
    MEM(reg);
    printf("----------\n");
    for(i=0;i<4;i++){
      printf("reg[%d]=%d\n",i,reg[i]);
    }
    WB(reg);
    j++;
  }
for(i=0;i<16;i++){
  printf("Reg[%d]=%d\n",i,Reg[i]);
}
printf("-------------------\n");
for(i=30;i<40;i++){
  printf("Mem[%d]=%d\n",i,Mem[i]);
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
  }else if(op==0x7){//TODO:bne
    idex_reg[1]=Reg[(idex_reg[0]>>8)&0x0f];
    idex_reg[2]=Reg[(idex_reg[0]>>4)&0x0f];
    idex_reg[3]=Reg[(idex_reg[0])&0x0f];
  }else{//ALU
    idex_reg[1]=(idex_reg[0]>>8)&0x0f;
    idex_reg[2]=Reg[(idex_reg[0]>>4)&0x0f];
    idex_reg[3]=Reg[(idex_reg[0])&0x0f];
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
  }else if(op==0x7){//TODO:bne
    if(exmem_reg[1]!=exmem_reg[2]){
      program_count=exmem_reg[3];
    }
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
  }else if(op!=0x2 && op!=0x7){//TODO:bne//store以外＝ALUだとbne含まれるから条件追加
    Reg[wb_reg[1]]=wb_reg[2];
  }


}
