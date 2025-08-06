//#pragma warning(disable:4996)
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//int Memory[0x400000];
//int Regs[32] = {0, };
//unsigned int PC = 0;
//unsigned int nPC = 0;
//int Forward[3];
//int Forward_val[3];
//
//int ctrl_dep;
//int ctrl_dep_count;
//int BTB_ex = 0;			// BTB Fetch에 적용 1 
//int BTB_I = 0;
//
//int BTB[0x100000][3];	// [0] = pc / [1] = target / [2] = Branch_ex
//
//typedef struct
//{
//	unsigned int pc;
//	// instruction
//	int instruction;
//	int opcode;
//	int rs;
//	int rt;
//	int rd;
//	int immediate;
//	int funct;
//	int shamt;
//
//	int address;
//	int SignExtImm;
//	int ZeroExtImm;
//	int BranchAddr;
//	int JumpAddr;
//
//	// ctrl_signal
//	int RegDst;
//	int Jump;
//	int Branch;
//	int MemRead;
//	int Mem2Regs;
//	int ALUop;
//	int MemWrite;
//	int ALUSrc;
//	int RegWrite;
//
//	// alu
//	int ALU_ctrl_num;
//	int ALU_result;
//
//	// Register
//	int Read_register1;
//	int Read_register2;
//	int Write_register;
//	int R_Write_data;
//	int Read_data1;
//	int Read_data2;
//
//	// Memory
//	int Address;
//	int M_Write_data;
//	int Read_data;
//}Latch;
//
//typedef struct
//{
//	int cycle_num;
//
//	int R_type_num;
//	int I_type_num;
//	int J_type_num;
//	int Nop_num;
//
//	int MEM_Access_num;
//	int Jump_num;
//	int Branch_num;
//	int Branch_taken;
//	int Branch_predict_O;
//	int Branch_predict_X;
//}Count_num;
//
//// 함수원형 정의
//void Regs_Set(int Regs[]);
//void Latch_Set(Latch* latch);
//void BTB_Set(int i);
//void Count_num_set(Count_num* count_num);
//void Inst_Set(Latch* IF_ID);
//
//void Latch_copy(Latch* input, Latch* output);
//int Mux(int input1, int input2, int signal);
//void Regs_value(int Read_regs1, int Read_regs2, int Write_regs, Latch* latch);
//
//void Count_RIJ_type(Latch* IF_ID, Count_num* count);	// RIJ_type Count
//void Ctrl_signal_Set(Latch* IF_ID, Count_num* count);
//void ALU_Ctrl_signal(Latch* IF_ID, int opcode, int funct, Count_num* count);
//void ALU(int input1, int input2, int ALU_ctrl_num, Latch* IF_ID, Count_num* count);
//
//void Forwarding_unit_in(Latch* Latch, int i);
//int F_mux(int r, int data);
//
//void BTB_Insert(unsigned int pc, int Address, int taken);
//int BTB_detection(Latch* IF_ID);
//
//void Fetch(Latch* IF_ID, Latch* EX_MEM);
//void Decode(Latch* IF_ID, Count_num* count_num);
//void Execution(Latch* ID_EX, Count_num* count_num);
//void Memory_Access(Latch* EX_MEM, Count_num* count_num);
//void Write_Back(Latch* MEM_WB);
//
//void Get_Instruction();
//void print_Latch(Latch* latch, int type);
//void Print_result(Count_num* count_num);
//
//// 함수 정의
//void Regs_Set(int Regs[])
//{
//	for (int n = 0; n < 32; n++)
//		Regs[n] = 0;
//	Regs[29] = 0x100000;
//	Regs[31] = 0xffffffff;
//	return;
//}
//
//void Latch_Set(Latch* latch)
//{
//	latch->pc = 0;
//
//	latch->instruction = 0;
//	latch->opcode = 0;
//	latch->rs = 0;
//	latch->rt = 0;
//	latch->rd = 0;
//	latch->immediate = 0;
//	latch->funct = 0;
//	latch->shamt = 0;
//
//	latch->address = 0;
//	latch->SignExtImm = 0;
//	latch->ZeroExtImm = 0;
//	latch->BranchAddr = 0;
//	latch->JumpAddr = 0;
//
//	latch->RegDst = 0;
//	latch->Jump = 0;
//	latch->Branch = 0;
//	latch->MemRead = 0;
//	latch->Mem2Regs = 0;
//	latch->ALUop = 0;
//	latch->MemWrite = 0;
//	latch->ALUSrc = 0;
//	latch->RegWrite = 0;
//
//	latch->ALU_ctrl_num = 0;
//	latch->ALU_result = 0;
//
//	latch->Read_register1 = 0;
//	latch->Read_register2 = 0;
//	latch->Write_register = 0;
//	latch->R_Write_data = 0;
//	latch->Read_data1 = 0;
//	latch->Read_data2 = 0;
//
//	latch->Address = 0;
//	latch->M_Write_data = 0;
//	latch->Read_data = 0;
//
//}
//
//void BTB_Set(int i)
//{
//	BTB[i][0] = 0;
//	BTB[i][1] = 0;
//	BTB[i][2] = 0;
//}
//
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
//
//void Inst_Set(Latch* IF_ID)
//{
//	IF_ID->opcode = (IF_ID->instruction >> 26) & 0x3f;
//	IF_ID->rs = (IF_ID->instruction >> 21) & 0x1f;
//	IF_ID->rt = (IF_ID->instruction >> 15) & 0x1f;
//	IF_ID->rd = (IF_ID->instruction >> 11) & 0x1f;
//	IF_ID->shamt = (IF_ID->instruction >> 6) & 0x1f;
//	IF_ID->funct = IF_ID->instruction & 0x0000003f;
//	IF_ID->immediate = IF_ID->instruction & 0x0000ffff;
//	IF_ID->address = IF_ID->instruction & 0x03ffffff;
//	IF_ID->SignExtImm = (IF_ID->immediate & 0x8000) ? (IF_ID->immediate | 0xffff0000) : (IF_ID->immediate);
//	IF_ID->ZeroExtImm = IF_ID->SignExtImm & 0x0000ffff;
//	IF_ID->BranchAddr = IF_ID->SignExtImm * 4;
//	IF_ID->JumpAddr = ((IF_ID->pc + 4) & 0xf0000000) | (IF_ID->address << 2);
//}
//
//void Latch_copy(Latch* input, Latch* output)
//{
//	memcpy(output, input, sizeof(Latch));
//}
//
//int Mux(int input1, int input2, int signal)
//{
//	if (signal == 1)
//		return input2;
//	else
//		return input1;
//}
//
//void Regs_value(int Read_regs1, int Read_regs2, int Write_regs, Latch* latch)
//{
//	latch->Read_register1 = Read_regs1;
//	latch->Read_register2 = Read_regs2;
//	latch->Write_register = Write_regs;	// 적을 Regs 주소 저장
//	latch->Read_data1 = Regs[latch->Read_register1];
//	latch->Read_data2 = Regs[latch->Read_register2];
//	return;
//}
//
//void Count_RIJ_type(Latch* IF_ID, Count_num* count)	// RIJ_type Count
//{
//	if (IF_ID->instruction == 0)
//	{
//		count->Nop_num++;	// Nop count
//	}
//	else if (IF_ID->opcode == 0)
//	{
//		count->R_type_num++;	// R-type count
//	}
//	else if ((IF_ID->opcode == 0x2) || (IF_ID->opcode == 0x3))
//	{
//		count->J_type_num++;	// J-type count
//	}
//	else if ((IF_ID->opcode == 0x0) && (IF_ID->funct == 0x8))
//	{
//		count->Jump_num++;	// jump count
//	}
//	else
//	{
//		count->I_type_num++;	// I-type count
//	}
//}
//
//void Ctrl_signal_Set(Latch* IF_ID, Count_num* count)
//{
//	if (IF_ID->opcode == 0x0)
//	{
//		IF_ID->RegDst = 1;
//		IF_ID->ALUop = 1;
//	}	// R-type inst
//	else;
//	if (((IF_ID->opcode == 0x0) && (IF_ID->funct == 0x8)) || (IF_ID->opcode == 0x2) || (IF_ID->opcode == 0x3))
//	{
//		IF_ID->Jump = 1;
//	}	// JR, Jump, JAL
//	else;
//	if ((IF_ID->opcode == 0x4) || (IF_ID->opcode == 0x5))
//	{
//		IF_ID->Branch = 1;
//	}	// BEQ, BNE
//	else;
//	if (IF_ID->opcode == 0x23)
//	{
//		IF_ID->MemRead = 1;
//		IF_ID->Mem2Regs = 1;
//	}	// LW
//	else;
//	if (IF_ID->opcode == 0x2b)
//	{
//		IF_ID->MemWrite = 1;
//	}	// SW
//	else;
//	if ((IF_ID->opcode != 0) && (IF_ID->opcode != 0x4) && (IF_ID->opcode != 0x5))
//	{
//		IF_ID->ALUSrc = 1;
//	}	// Not R_type, BEQ, BNE
//	else;
//	if ((IF_ID->opcode != 0x2b) && (IF_ID->opcode != 0x4) && (IF_ID->opcode != 0x5) && (IF_ID->opcode != 0x2) && !((IF_ID->opcode == 0x0) && (IF_ID->funct == 0x8)))
//	{
//		IF_ID->RegWrite = 1;
//	}	// Not SW, BEQ, BNE, Jump, JR
//	else {
//		IF_ID->RegWrite = 0;
//	}
//}
//
//void ALU_Ctrl_signal(Latch* IF_ID, int opcode, int funct, Count_num* count)
//{
//	if (IF_ID->ALUop == 1)
//	{
//		switch (IF_ID->funct)
//		{
//		case 0x20:	// Add
//		case 0x21:	// AddU
//			IF_ID->ALU_ctrl_num = 1;	// +
//			return;
//		case 0x24:	// And
//			IF_ID->ALU_ctrl_num = 2;	// &
//			return;
//		case 0x8:	// JR
//			IF_ID->ALU_ctrl_num = 3;	// JR
//			count->Jump_num++;
//			return;
//		case 0x27:	// NOR
//			IF_ID->ALU_ctrl_num = 4;	// ~( | )
//			return;
//		case 0x25:	// OR
//			IF_ID->ALU_ctrl_num = 5;	// |
//			return;
//		case 0x2a:	// SLT
//		case 0x2b:	// SLTI
//			IF_ID->ALU_ctrl_num = 6;	// ( < )? 1:0
//			return;
//		case 0x00:	// SLL
//			IF_ID->ALU_ctrl_num = 7;	// <<
//			return;
//		case 0x02:	// SRL
//			IF_ID->ALU_ctrl_num = 8;	// >>
//			return;
//		case 0x22:	// Sub
//		case 0x23:	// SubU
//			IF_ID->ALU_ctrl_num = 9;	// -
//			return;
//		default:
//			return;
//		}
//		// I-type
//	}
//	else
//	{
//		switch(opcode)
//		{
//		case 0x8:	// AddI
//		case 0x9:	// AddIU
//		case 0x23:	// LW
//		case 0x2b:	// SW
//			IF_ID->ALU_ctrl_num = 1;	// +
//			return;
//		case 0xc:	// AndI
//			IF_ID->ALU_ctrl_num = 2;	// &
//			return;
//		case 0x4:	// BEQ
//			IF_ID->ALU_ctrl_num = 10;	// ( == )? 1:0
//			return;
//		case 0x5:	// BNQ
//			IF_ID->ALU_ctrl_num = 11;	// ( != )? 1:0
//			return;
//		case 0xf:	// LUI
//			IF_ID->ALU_ctrl_num = 12;	// LUI
//			return;
//		case 0xd:	// ORI
//			IF_ID->ALU_ctrl_num = 5;	// |
//			return;
//		case 0xa:	// SLTI
//		case 0xb:	// SLTIU
//			IF_ID->ALU_ctrl_num = 6; // ( < )? 1:0
//			return;
//			// J-type
//		case 0x2:	// Jump
//		case 0x3:	// JAR
//			return;
//		default:
//			return;
//		}
//	}
//}
//
//void ALU(int input1, int input2, int ALU_ctrl_num, Latch* IF_ID, Count_num* count)
//{
//	switch (ALU_ctrl_num)
//	{
//	case 0:
//		IF_ID->ALU_result = 0;
//		return;
//	case 1:
//		IF_ID->ALU_result = input1 + input2;
//		return;
//	case 2:
//		IF_ID->ALU_result = input1 & input2;
//		return;
//	case 3:
//		IF_ID->ALU_result = input1;
//		return;
//	case 4:
//		IF_ID->ALU_result = ~(input1 | input2);
//		return;
//	case 5:
//		IF_ID->ALU_result = input1 | input2;
//		return;
//	case 6:
//		if (input1 < input2)
//		{
//			IF_ID->ALU_result = 1;
//		}
//		else
//		{
//			IF_ID->ALU_result = 0;
//		}
//		return;
//	case 7:
//		IF_ID->ALU_result = input1 << input2;
//		return;
//	case 8:
//		IF_ID->ALU_result = input1 >> input2;
//		return;
//	case 9:
//		IF_ID->ALU_result = input1 - input2;
//		return;
//	case 10:
//		count->Branch_num++;
//		if (input1 == input2)
//		{
//			IF_ID->ALU_result = 1;
//			count->Branch_taken++;
//		}
//		else
//		{
//			IF_ID->ALU_result = 0;
//		}
//		return;
//	case 11:
//		count->Branch_num++;
//		if (input1 != input2)
//		{
//			IF_ID->ALU_result = 1;
//			count->Branch_taken++;
//		}
//		else
//		{
//			IF_ID->ALU_result = 0;
//		}
//		return;
//	case 12:
//		IF_ID->ALU_result = (input2 << 16) & 0xffff0000;
//		return;
//	default:
//		return;
//	}
//}
//
//void Forwarding_unit_in(Latch* Latch, int i)
//{
//	if (i == 1) {
//		Forward[2] = Forward[1];
//		Forward[1] = Forward[0];
//		Forward[0] = Latch->Write_register;
//		Forward_val[2] = Forward_val[1];
//		Forward_val[1] = Forward_val[0];
//		Forward_val[0] = Latch->Read_data;
//		return;
//	}
//	else {
//		if (Latch->RegWrite == 1) {
//			if ((Latch->Mem2Regs == 0) && (Latch->Jump == 0)) {
//				Forward[2] = Forward[1];
//				Forward[1] = Forward[0];
//				Forward[0] = Latch->Write_register;
//				Forward_val[2] = Forward_val[1];
//				Forward_val[1] = Forward_val[0];
//				Forward_val[0] = Latch->ALU_result;
//				return;
//			}
//			else if (Latch->Jump) {
//				Forward[2] = Forward[1];
//				Forward[1] = Forward[0];
//				Forward[0] = Latch->Write_register;
//				Forward_val[2] = Forward_val[1];
//				Forward_val[1] = Forward_val[0];
//				Forward_val[0] = Latch->pc + 8;
//				return;
//			}
//			return;
//		}
//		else {
//			Forward[2] = Forward[1];
//			Forward[1] = Forward[0];
//			Forward[0] = 0;
//			Forward_val[2] = Forward_val[1];
//			Forward_val[1] = Forward_val[0];
//			Forward_val[0] = 0;
//			return;
//		}
//	}
//}
//
//int F_mux(int r, int data)
//{
//	for (int i = 0; i < 3; i++) {
//		if (Forward[i] == r) {
//			return Forward_val[i];
//		}
//	}
//	return data;
//}
//
//int BTB_detection(Latch* IF_ID)	// 0일때 무시됨??
//{
//	int i = 1;
//	BTB[1][0] = 1;
//	for (i = 1; i < 0x100000; i++)
//	{
//		if (IF_ID->pc == BTB[i][0])
//		{
//			IF_ID->pc = BTB[i][1];	// BTB address 입력
//			nPC = IF_ID->pc;
//
//			BTB_ex = 1;
//			return i;
//		}
//		else
//		{
//			nPC = IF_ID->pc + 4;
//			BTB_ex = 0;
//			return 0;
//		}
//	}
//	return 0;
//}
//
//void BTB_Insert(unsigned int pc, int Address, int taken)
//{
//	for (int i = 1; i < 0x100000; i++)
//	{
//		if (BTB[i][0] == pc)
//		{
//			return;
//		}
//		else if (BTB[i][0] == 0)
//		{
//			BTB[i][0] = pc;
//			BTB[i][2] = Address;
//			if (taken == 1)
//			{
//				BTB[i][2] = 1;
//			}
//			else
//			{
//				BTB[i][2] = 0;
//			}
//		}
//	}
//}
//
//void Fetch(Latch* IF_ID, Latch* EX_MEM)
//{
//	if (ctrl_dep != 1)
//	{
//		IF_ID->pc = nPC;
//		IF_ID->instruction = Memory[nPC / 4];
//	}
//
//	if ((EX_MEM->opcode == 0x0) && (EX_MEM->funct == 0x08))
//	{
//		nPC = EX_MEM->Read_data1;	// JR은 기록X
//	}
//	else
//	{
//		nPC = Mux(Mux(IF_ID->pc + 4, (EX_MEM->BranchAddr + EX_MEM->pc + 4), (EX_MEM->Branch & EX_MEM->ALU_result)), EX_MEM->JumpAddr, EX_MEM->Jump);
//	}	// Jump, Branch에 따른 ->주소값, else ->pc + 4;
//	
//	BTB_I = BTB_detection(IF_ID);	// 기록이 있으면 이전의 nPC 새로 입력됨
//
//	if (ctrl_dep_count < 1)	// 새로 불러와야 하는 경우
//	{
//		if (ctrl_dep == 1)
//		{
//			if (nPC == 0xffffffff)
//			{
//				Latch_Set(IF_ID);
//				PC = nPC;
//				return;
//			}
//			IF_ID->pc = nPC;
//			IF_ID->instruction = Memory[nPC / 4];
//			nPC += 4;
//		}
//		else if (ctrl_dep == 2)
//		{
//			ctrl_dep_count++;
//		}
//	}
//	else
//	{
//		ctrl_dep_count = 0;
//		ctrl_dep = 0;
//	}
//
//	PC = nPC;
//}
//
//void Decode(Latch* IF_ID, Count_num* count_num)
//{
//	if (ctrl_dep_count < 2)
//	{
//		if (ctrl_dep == 1)		// Jump, Branch taken & 예측 실패
//		{
//			Latch_Set(IF_ID);
//			return;
//		}
//		else if (ctrl_dep == 2)	// Branch not taken & 예측 실패
//		{
//			Latch_Set(IF_ID);
//			return;
//		}
//	}
//	Inst_Set(IF_ID);
//	Count_RIJ_type(IF_ID, count_num);
//	ALU_Ctrl_signal(IF_ID, IF_ID->opcode, IF_ID->funct, count_num);
//	Ctrl_signal_Set(IF_ID, count_num);
//
//	// imm = signExtimm
//	if ((IF_ID->opcode == 0xc) || (IF_ID->opcode == 0xd) || (IF_ID->opcode == 0xf));
//	else
//	{
//		IF_ID->immediate = IF_ID->SignExtImm;
//	}
//	// Reg_value 입력
//	if (IF_ID->opcode == 0x3)
//	{
//		Regs_value(IF_ID->rs, IF_ID->rt, 31, IF_ID);
//	}	// JAL
//	else
//	{
//		Regs_value(IF_ID->rs, IF_ID->rt, Mux(IF_ID->rt, IF_ID->rd, IF_ID->RegDst), IF_ID);
//	}	// else
//}
//
//void Execution(Latch* ID_EX, Count_num* count_num)
//{
//	if (ctrl_dep_count < 2)
//	{
//		if (ctrl_dep == 1)		// Jump, Branch taken & 예측 실패
//		{
//			Latch_Set(ID_EX);
//			Forwarding_unit_in(ID_EX, 0);
//			ctrl_dep = 0;
//			return;
//		}
//		else if (ctrl_dep == 2)	// Branch not taken & 예측 실패
//		{
//			Latch_Set(ID_EX);
//			Forwarding_unit_in(ID_EX, 0);
//			ctrl_dep = 0;
//			return;
//		}
//	}
//
//	// forwarding 해서 불러온 값 F_Mux로 재정의
//	ID_EX->Read_data1 = F_mux(ID_EX->Read_register1, ID_EX->Read_data1);
//	ID_EX->Read_data2 = F_mux(ID_EX->Read_register2, ID_EX->Read_data2);
//	if ((ID_EX->opcode == 0xc) || (ID_EX->opcode == 0xd))
//	{
//		ALU(ID_EX->Read_data1, Mux(ID_EX->Read_data2, ID_EX->ZeroExtImm, ID_EX->ALUSrc), ID_EX->ALU_ctrl_num, ID_EX, count_num);
//		Forwarding_unit_in(ID_EX, 0);
//		return;
//	}	// AndI, ORI = ALU(rs, Z_imm)
//	else if ((ID_EX->opcode == 0x0) && ((ID_EX->funct == 0x0) || (ID_EX->funct = 0x2)))
//	{
//		ALU(ID_EX->Read_data2, ID_EX->shamt, ID_EX->ALU_ctrl_num, ID_EX, count_num);
//		Forwarding_unit_in(ID_EX, 0);
//		return;
//	}	// SLL, SRL = ALU(rt, shamt)
//	else
//	{
//		ALU(ID_EX->Read_data1, Mux(ID_EX->Read_data2, ID_EX->immediate, ID_EX->ALUSrc), ID_EX->ALU_ctrl_num, ID_EX, count_num);
//		Forwarding_unit_in(ID_EX, 0);
//
//		if (BTB_ex == 1)		// 기존의 BTB 기록 O
//		{
//			// Jump
//			if (ID_EX->Jump == 1)
//			{
//				ID_EX->Jump = 0;
//			}
//			// Branch
//			if ((ID_EX->Branch && ID_EX->ALU_result) == 1)
//			{
//				count_num->Branch_predict_O++;
//				ID_EX->Branch = 0;
//			}
//			else if ((ID_EX->Branch && ID_EX->ALU_result) != 1)
//			{
//				count_num->Branch_predict_O++;
//			}
//		}
//		else if (BTB_ex != 1)	// 기존의 BTB 기록 X
//		{
//			// Jump
//			if (ID_EX->Jump == 1)
//			{
//				if (ID_EX->RegDst == 1)
//				{
//					ctrl_dep = 1;
//				}
//				else 
//				{
//					ctrl_dep = 1;
//					BTB_Insert(ID_EX->pc, ID_EX->JumpAddr, BTB[BTB_I][2]);
//				}
//			}
//			// Branch
//			if ((ID_EX->Branch && ID_EX->ALU_result) == 1)
//			{
//				ctrl_dep = 1;
//				BTB_Insert(ID_EX->pc, (ID_EX->JumpAddr + ID_EX->pc + 4), BTB[BTB_I][2]);
//				count_num->Branch_predict_X++;
//			}
//			else if ((ID_EX->Branch && ID_EX->ALU_result) != 1)
//			{
//				ctrl_dep = 2;
//				BTB_Insert(ID_EX->pc, (ID_EX->JumpAddr + ID_EX->pc + 4), BTB[BTB_I][2]);
//				count_num->Branch_predict_X++;
//			}
//		}
//	}
//	return;
//}
//
//void Memory_Access(Latch* EX_MEM, Count_num* count)
//{
//	EX_MEM->Address = EX_MEM->ALU_result;
//	EX_MEM->M_Write_data = EX_MEM->Read_data2;
//
//	if ((EX_MEM->MemWrite == 1) || (EX_MEM->MemRead == 1))
//	{
//		count->MEM_Access_num++;;
//
//		if ((EX_MEM->MemWrite == 1) && (EX_MEM->opcode == 0x2b))
//		{
//			EX_MEM->Read_data = 0;
//			Memory[(EX_MEM->Address / 4) - 1] = EX_MEM->M_Write_data;
//			return;
//		}	// SW
//		if ((EX_MEM->MemRead == 1) && (EX_MEM->opcode == 0x23))
//		{
//			EX_MEM->Read_data = Memory[(EX_MEM->Address / 4) - 1];
//			return;
//		}	// LW
//	}
//	else
//	{
//		EX_MEM->Read_data = 0;
//	}
//}
//
//void Write_Back(Latch* MEM_WB)
//{
//	MEM_WB->R_Write_data = Mux(MEM_WB->ALU_result, MEM_WB->Read_data, MEM_WB->Mem2Regs);
//	if (MEM_WB->RegWrite == 1)	// Not SW, BOE, BNE, Jump, JR
//	{
//		if (MEM_WB->opcode == 0x3)
//		{
//			Regs[31] = MEM_WB->pc + 8;
//		}	// JAL 조건
//		else
//		{
//			Regs[MEM_WB->Write_register] = MEM_WB->M_Write_data;
//		}	// Not SW, BOE, BNE, Jump, JR인 나머지
//	}
//}
//
//void Get_Instruction()
//{
//	FILE* fp = NULL;
//	fp = fopen("simple.bin", "rb");
//	if (fp == NULL)
//	{
//		perror("file open error");
//	}
//
//	int i = 0;
//	int var = 0;
//
//	while (1)
//	{
//		int ret = fread(&var, sizeof(var), 1, fp);
//		if (ret == 0) break;
//
//		unsigned int b1, b2, b3, b4;
//		b1 = var & 0x000000ff;
//		b2 = var & 0xff00;
//		b3 = var & 0x00ff0000;
//		b4 = (var >> 24) & 0xff;
//		int res = (b1 << 24) | (b2 << 8) | (b3 >> 8) | b4;
//
//		int instruction = res;
//		Memory[i] = instruction;
//		i++;
//	}
//	fclose(fp);
//	return;
//}
//
//void print_Latch(Latch* latch, int type)
//{
//	switch (type)
//	{
//	case 1:	// inst
//		printf("pc: %u\n", latch->pc);
//		printf("instruction: %d\n", latch->instruction);
//		printf("opcode: %d\n", latch->opcode);
//		printf("rs: %d\n", latch->rs);
//		printf("rt: %d\n", latch->rt);
//		printf("rd: %d\n", latch->rd);
//		printf("immediate: %d\n", latch->immediate);
//		printf("funct: %d\n", latch->funct);
//		printf("shamt: %d\n", latch->shamt);
//
//		printf("address: %d\n", latch->address);
//		printf("SignExtImm: %d\n", latch->SignExtImm);
//		printf("ZeroExtImm: %d\n", latch->ZeroExtImm);
//		printf("BranchAddr: %d\n", latch->BranchAddr);
//		printf("JumpAddr: %d\n", latch->JumpAddr);
//
//	case 2: // Ctrl signal
//		printf("RegDst: %d\n", latch->RegDst);
//		printf("Jump: %d\n", latch->Jump);
//		printf("Branch: %d\n", latch->Branch);
//		printf("MemRead: %d\n", latch->MemRead);
//		printf("Mem2Regs: %d\n", latch->Mem2Regs);
//		printf("ALUop: %d\n", latch->ALUop);
//		printf("MemWrite: %d\n", latch->MemWrite);
//		printf("ALUSrc: %d\n", latch->ALUSrc);
//		printf("RegWrite: %d\n", latch->RegWrite);
//
//	case 3: // ALU
//		printf("ALU_ctrl_num: %d\n", latch->ALU_ctrl_num);
//		printf("ALU_result: %d\n", latch->ALU_result);
//
//	case 4: // Register
//		printf("Read_register1: %d\n", latch->Read_register1);
//		printf("Read_register2: %d\n", latch->Read_register2);
//		printf("Write_register: %d\n", latch->Write_register);
//		printf("R_Write_data: %d\n", latch->R_Write_data);
//		printf("Read_data1: %d\n", latch->Read_data1);
//		printf("Read_data2: %d\n", latch->Read_data2);
//
//	case 5: // Memory
//		printf("Address: %d\n", latch->Address);
//		printf("M_Write_data: %d\n", latch->M_Write_data);
//		printf("Read_data: %d\n", latch->Read_data);
//	default:
//		break;
//	}
//
//}
//
//void Print_result(Count_num* count_num)
//{
//	int count_inst = count_num->R_type_num + count_num->I_type_num + count_num->J_type_num + count_num->Nop_num;
//	printf("Num of Executed Inst: %d\n[R_inst: %d, I_inst: %d, J_inst:%d, Nop: %d]\n",
//		count_inst, count_num->R_type_num, count_num->I_type_num, count_num->J_type_num, count_num->Nop_num);
//	printf("Memory Access inst: %d\n", count_num->MEM_Access_num);
//	printf("Num of Jump: %d\n", count_num->Jump_num);
//	printf("Num of Branch: %d\nNum of Branch taken: %d\n", count_num->Branch_num, count_num->Branch_taken);
//	printf("Num of Branch Predict: success: %d, fail: %d\n", count_num->Branch_predict_O, count_num->Branch_predict_X);
//}
//
//int main(int argc, char* argv[])
//{
//	Get_Instruction();
//
//	Latch latch[8];
//	Count_num count_num;
//	// BTB btb[0x100000];
//
//	// Initialization
//	Regs_Set(Regs);
//	for (int i = 0; i < 8; i++)
//	{
//		Latch_Set(&latch[i]);
//	}
//	Count_num_set(&count_num);
//	for (int i = 0; i < 0x100000; i++)
//	{
//		BTB_Set(i);
//	}
//
//	// pipeline
//	while (PC != 0xffffffff)
//	{
//		Fetch(&latch[0], &latch[5]);
//		Decode(&latch[1], &count_num);
//		Latch_copy(&latch[0], &latch[1]);
//		Execution(&latch[4], &count_num);
//		Latch_copy(&latch[3], &latch[4]);
//		Memory_Access(&latch[6], &count_num);
//		Latch_copy(&latch[5], &latch[6]);
//		Write_Back(&latch[8]);
//
//		for (int i = 0; i < 9; i++)
//		{
//			print_Latch(&latch[i], 1);
//		}
//		
//
//		// 다음 cycle을 위한 inst 5단계 간 copy
//		for (int i = 1; i < 5; i++)
//		{
//			Latch_copy(&latch[(2 * i) - 1], &latch[2 * i]);
//		}
//		count_num.cycle_num++;
//	}
//	Print_result(&count_num);
//}
//
//
//
//
