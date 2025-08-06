#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Memory[0x400000];
int Regs[32] = { 0, };
unsigned int PC = 0;
unsigned int nPC = 0;

int Forward[3] = { 0 };
int Forward_val[3] = { 0 };

int ctrl_dep_Jump = 0;
int ctrl_dep_Branch = 0;
int BTB_ex = 0;				// BTB Fetch에 적용 1 
int BTB_I = 0;

typedef struct
{
	unsigned int pc;
	// instruction
	int instruction;
	int opcode;
	int rs;
	int rt;
	int rd;
	int immediate;
	int funct;
	int shamt;

	int address;
	int SignExtImm;
	int ZeroExtImm;
	int BranchAddr;
	int JumpAddr;

	// ctrl_signal
	int RegDst;
	int Jump;
	int Branch;
	int MemRead;
	int Mem2Regs;
	int ALUop;
	int MemWrite;
	int ALUSrc;
	int RegWrite;

	// alu
	int ALU_ctrl_num;
	int ALU_result;

	// Register
	int Read_register1;
	int Read_register2;
	int Write_register;
	int R_Write_data;
	int Read_data1;
	int Read_data2;

	// Memory
	int Address;
	int M_Write_data;
	int Read_data;
}Latch;

typedef struct
{
	int cycle_num;

	int R_type_num;
	int I_type_num;
	int J_type_num;
	int Nop_num;

	int MEM_Access_num;
	int Jump_num;
	int Branch_num;
	int Branch_taken;
	int Branch_predict_O;
	int Branch_predict_X;
}Count_num;

// 선언 및 초기화
int BTB[0x100000][3] = { 0 };	// [0] = pc / [1] = target / [2] = Branch_ex
Latch latch[9] = { 0 };
Count_num count_num = { 0 };

// 함수원형 정의
void Regs_Set(int Regs[]);
void Latch_Set(Latch* latch);
void Inst_Set();

void Latch_copy(Latch* input, Latch* output);
void Regs_value(int Read_regs1, int Read_regs2, int Write_regs);
void Count_RIJ_type();
void Ctrl_signal_Set();

void ALU_Ctrl_signal(int opcode, int funct);
void ALU(int input1, int input2, int ALU_ctrl_num);

void Forwarding_unit_in(Latch* Latch, int i);
int F_mux(int r, int data);

int BTB_detection();
void BTB_Insert(unsigned int pc, int Address, int taken);

void Fetch();
void Decode();
void Execution();
void Memory_Access();
void Write_Back();

void Get_Instruction();
void print_Latch(Latch* latch, int type);
void Print_result();

// 함수 정의
void Regs_Set(int Regs[])
{
	for (int n = 0; n < 32; n++)
		Regs[n] = 0;
	Regs[29] = 0x100000;
	Regs[31] = 0xffffffff;
	return;
}


void Latch_Set(Latch* latch)
{
	latch->pc = 0;

	latch->instruction = 0;
	latch->opcode = 0;
	latch->rs = 0;
	latch->rt = 0;
	latch->rd = 0;
	latch->immediate = 0;
	latch->funct = 0;
	latch->shamt = 0;

	latch->address = 0;
	latch->SignExtImm = 0;
	latch->ZeroExtImm = 0;
	latch->BranchAddr = 0;
	latch->JumpAddr = 0;

	latch->RegDst = 0;
	latch->Jump = 0;
	latch->Branch = 0;
	latch->MemRead = 0;
	latch->Mem2Regs = 0;
	latch->ALUop = 0;
	latch->MemWrite = 0;
	latch->ALUSrc = 0;
	latch->RegWrite = 0;

	latch->ALU_ctrl_num = 0;
	latch->ALU_result = 0;

	latch->Read_register1 = 0;
	latch->Read_register2 = 0;
	latch->Write_register = 0;
	latch->R_Write_data = 0;
	latch->Read_data1 = 0;
	latch->Read_data2 = 0;

	latch->Address = 0;
	latch->M_Write_data = 0;
	latch->Read_data = 0;

}

//void BTB_Set(int i)
//{
//	BTB[i][0] = 0;
//	BTB[i][1] = 0;
//	BTB[i][2] = 0;
//}

//void Count_num_set(Count_num* count_num)
//{
//	count_num->cycle_num = 0;
//
//	count_num->R_type_num = 0;
//	count_num->I_type_num = 0;
//	count_num->J_type_num = 0;
//	count_num->Nop_num = 0;
//
//	count_num->MEM_Access_num = 0;
//	count_num->Jump_num = 0;
//	count_num->Branch_num = 0;
//	count_num->Branch_taken = 0;
//	count_num->Branch_predict_O = 0;
//	count_num->Branch_predict_X = 0;
//}

void Inst_Set()	// &latch[i]로 함수 호출
{
	latch[3].opcode = (latch[3].instruction >> 26) & 0x3f;
	latch[3].rs = (latch[3].instruction >> 21) & 0x1f;
	latch[3].rt = (latch[3].instruction >> 15) & 0x1f;
	latch[3].rd = (latch[3].instruction >> 11) & 0x1f;
	latch[3].shamt = (latch[3].instruction >> 6) & 0x1f;
	latch[3].funct = latch[3].instruction & 0x0000003f;
	latch[3].immediate = latch[3].instruction & 0x0000ffff;
	latch[3].address = latch[3].instruction & 0x03ffffff;
	latch[3].SignExtImm = (latch[3].immediate & 0x8000) ? (latch[3].immediate | 0xffff0000) : (latch[3].immediate);
	latch[3].ZeroExtImm = latch[3].SignExtImm & 0x0000ffff;
	latch[3].BranchAddr = latch[3].SignExtImm * 4;
	latch[3].JumpAddr = ((latch[3].pc + 4) & 0xf0000000) | (latch[3].address << 2);
}

void Latch_copy(Latch* input, Latch* output)
{
	memcpy(output, input, sizeof(Latch));
}

int Mux(int input1, int input2, int signal)
{
	if (signal == 1)
		return input2;
	else
		return input1;
}

void Regs_value(int Read_regs1, int Read_regs2, int Write_regs)
{
	latch[3].Read_register1 = Read_regs1;
	latch[3].Read_register2 = Read_regs2;
	latch[3].Write_register = Write_regs;	// 적을 Regs 주소 저장
	latch[3].Read_data1 = Regs[latch[3].Read_register1];
	latch[3].Read_data2 = Regs[latch[3].Read_register2];
	return;
}	// latch[3] == ID_output latch

void Count_RIJ_type()	// RIJ_type Count
{
	if (latch[3].instruction == 0)
	{
		count_num.Nop_num++;	// Nop count
	}
	else if (latch[3].opcode == 0)
	{
		count_num.R_type_num++;	// R-type count
	}
	else if ((latch[3].opcode == 0x2) || (latch[3].opcode == 0x3))
	{
		count_num.J_type_num++;	// J-type count
	}
	else
	{
		count_num.I_type_num++;	// I-type count
	}

	if ((latch[3].opcode == 0x0) && (latch[3].funct == 0x8))
	{
		count_num.Jump_num++;	// jump count
	}
}

void Ctrl_signal_Set()
{
	if (latch[3].opcode == 0x0)
	{
		latch[3].RegDst = 1;
		latch[3].ALUop = 1;
	}	// R-type inst
	else;
	if (((latch[3].opcode == 0x0) && (latch[3].funct == 0x8)) || (latch[3].opcode == 0x2) || (latch[3].opcode == 0x3))
	{
		latch[3].Jump = 1;
	}	// JR, Jump, JAL
	else;
	if ((latch[3].opcode == 0x4) || (latch[3].opcode == 0x5))
	{
		latch[3].Branch = 1;
	}	// BEQ, BNE
	else;
	if (latch[3].opcode == 0x23)
	{
		latch[3].MemRead = 1;
		latch[3].Mem2Regs = 1;
	}	// LW
	else;
	if (latch[3].opcode == 0x2b)
	{
		latch[3].MemWrite = 1;
	}	// SW
	else;
	if ((latch[3].opcode != 0) && (latch[3].opcode != 0x4) && (latch[3].opcode != 0x5))
	{
		latch[3].ALUSrc = 1;
	}	// Not R_type, BEQ, BNE
	else;
	if ((latch[3].opcode != 0x2b) && (latch[3].opcode != 0x4) && (latch[3].opcode != 0x5) && (latch[3].opcode != 0x2) && !((latch[3].opcode == 0x0) && (latch[3].funct == 0x8)))
	{
		latch[3].RegWrite = 1;
	}	// Not SW, BEQ, BNE, Jump, JR
	else {
		latch[3].RegWrite = 0;
	}
}

void ALU_Ctrl_signal(int opcode, int funct)
{
	if (latch[3].ALUop == 1)
	{
		switch (latch[3].funct)
		{
		case 0x20:	// Add
		case 0x21:	// AddU
			latch[3].ALU_ctrl_num = 1;	// +
			return;
		case 0x24:	// And
			latch[3].ALU_ctrl_num = 2;	// &
			return;
		case 0x8:	// JR
			latch[3].ALU_ctrl_num = 3;	// JR
			return;
		case 0x27:	// NOR
			latch[3].ALU_ctrl_num = 4;	// ~( | )
			return;
		case 0x25:	// OR
			latch[3].ALU_ctrl_num = 5;	// |
			return;
		case 0x2a:	// SLT
		case 0x2b:	// SLTI
			latch[3].ALU_ctrl_num = 6;	// ( < )? 1:0
			return;
		case 0x00:	// SLL
			latch[3].ALU_ctrl_num = 7;	// <<
			return;
		case 0x02:	// SRL
			latch[3].ALU_ctrl_num = 8;	// >>
			return;
		case 0x22:	// Sub
		case 0x23:	// SubU
			latch[3].ALU_ctrl_num = 9;	// -
			return;
		default:
			return;
		}
		// I-type
	}
	else
	{
		switch (opcode)
		{
		case 0x8:	// AddI
		case 0x9:	// AddIU
		case 0x23:	// LW
		case 0x2b:	// SW
			latch[3].ALU_ctrl_num = 1;	// +
			return;
		case 0xc:	// AndI
			latch[3].ALU_ctrl_num = 2;	// &
			return;
		case 0x4:	// BEQ
			latch[3].ALU_ctrl_num = 10;	// ( == )? 1:0
			return;
		case 0x5:	// BNQ
			latch[3].ALU_ctrl_num = 11;	// ( != )? 1:0
			return;
		case 0xf:	// LUI
			latch[3].ALU_ctrl_num = 12;	// LUI
			return;
		case 0xd:	// ORI
			latch[3].ALU_ctrl_num = 5;	// |
			return;
		case 0xa:	// SLTI
		case 0xb:	// SLTIU
			latch[3].ALU_ctrl_num = 6; // ( < )? 1:0
			return;
			// J-type
		case 0x2:	// Jump
		case 0x3:	// JAR
			return;
		default:
			return;
		}
	}
}	// jump_num ++ 없앴는데 왜 써둔건지 확인하기

void ALU(int input1, int input2, int ALU_ctrl_num)
{
	switch (ALU_ctrl_num)
	{
	case 0:
		latch[3].ALU_result = 0;
		return;
	case 1:
		latch[3].ALU_result = input1 + input2;
		return;
	case 2:
		latch[3].ALU_result = input1 & input2;
		return;
	case 3:
		latch[3].ALU_result = input1;
		return;
	case 4:
		latch[3].ALU_result = ~(input1 | input2);
		return;
	case 5:
		latch[3].ALU_result = input1 | input2;
		return;
	case 6:
		if (input1 < input2)
		{
			latch[3].ALU_result = 1;
		}
		else
		{
			latch[3].ALU_result = 0;
		}
		return;
	case 7:
		latch[3].ALU_result = input1 << input2;
		return;
	case 8:
		latch[3].ALU_result = input1 >> input2;
		return;
	case 9:
		latch[3].ALU_result = input1 - input2;
		return;
	case 10:
		count_num.Branch_num++;
		if (input1 == input2)
		{
			latch[3].ALU_result = 1;
			count_num.Branch_taken++;
		}
		else
		{
			latch[3].ALU_result = 0;
		}
		return;
	case 11:
		count_num.Branch_num++;
		if (input1 != input2)
		{
			latch[3].ALU_result = 1;
			count_num.Branch_taken++;
		}
		else
		{
			latch[3].ALU_result = 0;
		}
		return;
	case 12:
		latch[3].ALU_result = (input2 << 16) & 0xffff0000;
		return;
	default:
		return;
	}
}

void Forwarding_unit_in(Latch* Latch, int i)
{
	if (i == 1) {
		Forward[2] = Forward[1];
		Forward[1] = Forward[0];
		Forward[0] = Latch->Write_register;
		Forward_val[2] = Forward_val[1];
		Forward_val[1] = Forward_val[0];
		Forward_val[0] = Latch->Read_data;
		return;
	}
	else {
		if (Latch->RegWrite == 1) {
			if ((Latch->Mem2Regs == 0) && (Latch->Jump == 0)) {
				Forward[2] = Forward[1];
				Forward[1] = Forward[0];
				Forward[0] = Latch->Write_register;
				Forward_val[2] = Forward_val[1];
				Forward_val[1] = Forward_val[0];
				Forward_val[0] = Latch->ALU_result;
				return;
			}
			else if (Latch->Jump) {
				Forward[2] = Forward[1];
				Forward[1] = Forward[0];
				Forward[0] = Latch->Write_register;
				Forward_val[2] = Forward_val[1];
				Forward_val[1] = Forward_val[0];
				Forward_val[0] = Latch->pc + 8;
				return;
			}
			return;
		}
		else {
			Forward[2] = Forward[1];
			Forward[1] = Forward[0];
			Forward[0] = 0;
			Forward_val[2] = Forward_val[1];
			Forward_val[1] = Forward_val[0];
			Forward_val[0] = 0;
			return;
		}
	}
}

int F_mux(int r, int data)
{
	for (int i = 0; i < 3; i++) {
		if (Forward[i] == r) {
			return Forward_val[i];
		}
	}
	return data;
}

int BTB_detection()
{
	int i = 1;
	BTB[1][0] = 1;
	for (i = 1; i < 0x100000; i++)
	{
		if (latch[1].pc == BTB[i][0])
		{
			latch[1].pc = BTB[i][1];	// BTB기록된 address에 입력
			nPC = latch[1].pc;		// 기록된 adress를 pc에 입력

			BTB_ex = 1;
			return i;
		}
		else
		{
			BTB_ex = 0;
			return 0;
		}
	}
	return 0;
}

void BTB_Insert(unsigned int pc, int Address, int taken)
{
	for (int i = 1; i < 0x100000; i++)
	{
		if (BTB[i][0] == pc)
		{
			return;
		}
		else if (BTB[i][0] == 0)
		{
			BTB[i][0] = pc;			// pc값 입력
			BTB[i][1] = Address;	// address값 입력
			BTB[i][2] = taken;		// 1=jump, 2=not taken, 3=taken
		}
	}
}

void Fetch()	// latch[1] = IF_ouput_latch
{
	latch[1].pc = nPC;
	latch[1].instruction = Memory[nPC / 4];

	if ((latch[5].opcode == 0x0) && (latch[5].funct == 0x08))
	{
		nPC = latch[5].Read_data1;
	}	// JR은 rs를 기반으로 하니 BTB 기록X
	else
	{
		nPC = Mux(Mux(latch[1].pc + 4, (latch[5].BranchAddr + latch[5].pc + 4), (latch[5].Branch && latch[5].ALU_result)), latch[3].JumpAddr, latch[3].Jump);
	}	// Jump, Branch에 따른 => 주소값, else => pc + 4값 반환

	// 기록에 있으면 nPC값 덮어씌우기	
	BTB_I = BTB_detection();

	if (BTB_ex == 1);	// BTB에 기록이 있음
	else if (BTB_ex == 0)	// BTB에 기록이 없음 / 위의 nPC값에 따름
	{
		if ((ctrl_dep_Jump > 0) || (ctrl_dep_Branch > 0))
		{
			Latch_Set(&latch[1]);
			if (nPC == 0xffffffff) // 넘겨와 받은 값이 end일 경우 확인
			{
				PC = nPC;	// PC값을 WB에서 확인하고 while종료
				return;
			}
		}	// Jump = 1 or Branch = 1~3
		else; // Branch not taken & else
	}
	// 바뀐 pc값, 명령어 다시 입력해주기
	latch[1].pc = nPC;
	latch[1].instruction = Memory[nPC / 4];
}

void Decode()
{
	Latch_copy(&latch[2], &latch[3]);

	if (ctrl_dep_Jump > 0)	// Jump 시 
	{
		Latch_Set(&latch[3]);
		ctrl_dep_Jump--;
	}
	if (ctrl_dep_Branch > 0)// Branch 시
	{
		Latch_Set(&latch[3]);
	}
	else;					// 예외처리

	Inst_set();
	Count_RIJ_type();
	ALU_Ctrl_signal(latch[3].opcode, latch[3].funct);
	Ctrl_signal_Set();

	if (BTB_ex == 1)		// BTB 기록 O
	{
		// ID에서 실행해 Ex에 들어가 Branch, jump를 실행방지
		if (BTB[BTB_I][2] == 1)			// jump의 기록
		{
			latch[3].Jump = 0;
		}
		else;
	}
	else if (BTB_ex == 0)	// BTB 기록 X
	{
		// Jump
		if (latch[3].Jump == 1)
		{
			ctrl_dep_Jump = 1;
			if (latch[3].RegDst == 1);
			else
			{
				BTB_Insert(latch[3].pc, latch[3].JumpAddr, 1);
			}
		}
	}

	// imm = signExtimm 처리
	if ((latch[3].opcode == 0xc) || (latch[3].opcode == 0xd) || (latch[3].opcode == 0xf));
	// AndI, LUI, ORI 제외
	else
	{
		latch[3].immediate = latch[3].SignExtImm;
	}

	// Reg_value 입력
	if (latch[3].opcode == 0x3)
	{
		Regs_value(latch[3].rs, latch[3].rt, 31);
	}	// JAL
	// imm~ 등은 Reg를 통하지 않아서 제외
	else
	{
		Regs_value(latch[3].rs, latch[3].rt, Mux(latch[3].rt, latch[3].rd, latch[3].RegDst));
	}	// else
}

void Execution()
{
	Latch_copy(&latch[4], &latch[5]);

	// jump는 1번만 버블
	if (ctrl_dep_Branch > 0)// Branch 시
	{
		Latch_Set(&latch[5]);
		ctrl_dep_Branch--;
	}
	else;					// 예외처리

	// forwarding 해서 불러온 값 F_Mux로 재정의
	latch[5].Read_data1 = F_mux(latch[5].Read_register1, latch[5].Read_data1);
	latch[5].Read_data2 = F_mux(latch[5].Read_register2, latch[5].Read_data2);
	if ((latch[5].opcode == 0xc) || (latch[5].opcode == 0xd))
	{
		ALU(latch[5].Read_data1, Mux(latch[5].Read_data2, latch[5].ZeroExtImm, latch[5].ALUSrc), latch[5].ALU_ctrl_num);
		Forwarding_unit_in(&latch[5], 0);
		return;
	}	// AndI, ORI = ALU(rs, Z_imm)
	else if ((latch[5].opcode == 0x0) && ((latch[5].funct == 0x0) || (latch[5].funct = 0x2)))
	{
		ALU(latch[5].Read_data2, latch[5].shamt, latch[5].ALU_ctrl_num);
		Forwarding_unit_in(&latch[5], 0);
		return;
	}	// SLL, SRL = ALU(rt, shamt)
	else
	{
		ALU(latch[5].Read_data1, Mux(latch[5].Read_data2, latch[5].immediate, latch[5].ALUSrc), latch[5].ALU_ctrl_num);
		Forwarding_unit_in(&latch[5], 0);

		if (BTB_ex == 1)		// 기존의 BTB 기록 O
		{///
			if (BTB[BTB_I][2] == 2)	// Branch not taken 기록
			{
				latch[5].Branch = 0;
				count_num.Branch_predict_O++;
				return;
			}
			else if (BTB[BTB_I][2] == 3)	// Branch taken 기록
			{
				latch[5].Branch = 0;
				count_num.Branch_predict_X++;
				return;
			}
		}
		else if (BTB_ex == 0)	// 기존의 BTB 기록 X
		{
			// Branch
			if ((latch[5].Branch && latch[5].ALU_result) == 1)
			{
				ctrl_dep_Branch = 3;
				BTB_Insert(latch[5].pc, (latch[5].JumpAddr + latch[5].pc + 4), 3);
				count_num.Branch_predict_X++;
			}
			else if ((latch[5].Branch && latch[5].ALU_result) != 1)
			{
				ctrl_dep_Branch = 0;
				BTB_Insert(latch[5].pc, latch[5].pc + 4, 2);
				count_num.Branch_predict_O++;
			}
		}
	}
	return;
}

void Memory_Access()
{
	Latch_copy(&latch[6], &latch[7]);

	latch[7].Address = latch[7].ALU_result;
	latch[7].M_Write_data = latch[7].Read_data2;

	if ((latch[7].MemWrite == 1) || (latch[7].MemRead == 1))
	{
		count_num.MEM_Access_num++;;

		if ((latch[7].MemWrite == 1) && (latch[7].opcode == 0x2b))
		{
			latch[7].Read_data = 0;
			Memory[(latch[7].Address / 4) - 1] = latch[7].M_Write_data;
			return;
		}	// SW
		if ((latch[7].MemRead == 1) && (latch[7].opcode == 0x23))
		{
			latch[7].Read_data = Memory[(latch[7].Address / 4) - 1];
			return;
		}	// LW
	}
	else
	{
		latch[7].Read_data = 0;
	}
}

void Write_Back()
{
	latch[8].R_Write_data = Mux(latch[8].ALU_result, latch[8].Read_data, latch[8].Mem2Regs);
	if (latch[8].RegWrite == 1)	// Not SW, BOE, BNE, Jump, JR
	{
		if (latch[8].opcode == 0x3)
		{
			Regs[31] = latch[8].pc + 8;
		}	// JAL 조건
		else
		{
			Regs[latch[8].Write_register] = latch[8].R_Write_data;
		}	// Not SW, BOE, BNE, Jump, JR인 나머지
	}
}

void Get_Instruction()
{
	FILE* fp = NULL;
	fp = fopen("simple.bin", "rb");
	if (fp == NULL)
	{
		perror("file open error");
	}

	int i = 0;
	int var = 0;

	while (1)
	{
		int ret = fread(&var, sizeof(var), 1, fp);
		if (ret == 0) break;

		unsigned int b1, b2, b3, b4;
		b1 = var & 0x000000ff;
		b2 = var & 0xff00;
		b3 = var & 0x00ff0000;
		b4 = (var >> 24) & 0xff;
		int res = (b1 << 24) | (b2 << 8) | (b3 >> 8) | b4;

		int instruction = res;
		Memory[i] = instruction;
		i++;
	}
	fclose(fp);
	return;
}

void print_Latch(Latch* latch, int type)
{
	switch (type)
	{
	case 1:	// inst
		printf("pc: %u\n", latch->pc);
		printf("instruction: %d\n", latch->instruction);
		printf("opcode: %d\n", latch->opcode);
		printf("rs: %d\n", latch->rs);
		printf("rt: %d\n", latch->rt);
		printf("rd: %d\n", latch->rd);
		printf("immediate: %d\n", latch->immediate);
		printf("funct: %d\n", latch->funct);
		printf("shamt: %d\n", latch->shamt);

		printf("address: %d\n", latch->address);
		printf("SignExtImm: %d\n", latch->SignExtImm);
		printf("ZeroExtImm: %d\n", latch->ZeroExtImm);
		printf("BranchAddr: %d\n", latch->BranchAddr);
		printf("JumpAddr: %d\n", latch->JumpAddr);

	case 2: // Ctrl signal
		printf("RegDst: %d\n", latch->RegDst);
		printf("Jump: %d\n", latch->Jump);
		printf("Branch: %d\n", latch->Branch);
		printf("MemRead: %d\n", latch->MemRead);
		printf("Mem2Regs: %d\n", latch->Mem2Regs);
		printf("ALUop: %d\n", latch->ALUop);
		printf("MemWrite: %d\n", latch->MemWrite);
		printf("ALUSrc: %d\n", latch->ALUSrc);
		printf("RegWrite: %d\n", latch->RegWrite);

	case 3: // ALU
		printf("ALU_ctrl_num: %d\n", latch->ALU_ctrl_num);
		printf("ALU_result: %d\n", latch->ALU_result);

	case 4: // Register
		printf("Read_register1: %d\n", latch->Read_register1);
		printf("Read_register2: %d\n", latch->Read_register2);
		printf("Write_register: %d\n", latch->Write_register);
		printf("R_Write_data: %d\n", latch->R_Write_data);
		printf("Read_data1: %d\n", latch->Read_data1);
		printf("Read_data2: %d\n", latch->Read_data2);

	case 5: // Memory
		printf("Address: %d\n", latch->Address);
		printf("M_Write_data: %d\n", latch->M_Write_data);
		printf("Read_data: %d\n", latch->Read_data);
	default:
		break;
	}
}

void Print_result()
{
	int count_inst = count_num.R_type_num + count_num.I_type_num + count_num.J_type_num + count_num.Nop_num;
	printf("Num of Executed Inst: %d\n[R_inst: %d, I_inst: %d, J_inst:%d, Nop: %d]\n",
		count_inst, count_num.R_type_num, count_num.I_type_num, count_num.J_type_num, count_num.Nop_num);
	printf("Memory Access inst: %d\n", count_num.MEM_Access_num);
	printf("Num of Jump: %d\n", count_num.Jump_num);
	printf("Num of Branch: %d\nNum of Branch taken: %d\n", count_num.Branch_num, count_num.Branch_taken);
	printf("Num of Branch Predict: success: %d, fail: %d\n", count_num.Branch_predict_O, count_num.Branch_predict_X);
}

int main(int argc, char* argv[])
{
	Get_Instruction();

	while (PC != 0xffffffff)
	{
		Fetch();
		Decode();
		Execution();
		Memory_Access();
		Write_Back();

		for (int i = 1; i < 5; i++)
		{
			Latch_copy(&latch[(2 * i) - 1], &latch[2 * i]);
		}

		count_num.cycle_num++;
	}
	Print_result();
}


