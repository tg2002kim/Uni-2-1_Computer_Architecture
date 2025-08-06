//#pragma warning(disable:4996)
//
//#include <stdio.h>
//#include <errno.h>
//// #define DEBUG_LOAD
//
//// MIPS single cycle uarch emulator
//
//// Memory is a global data structure
//int memory[0x400000];
//unsigned int pc = 0;
//int Regs[32] = { 0, };
//
//int main(int argc, char* argv[])
//{
//
//	// test for checking binary input/output
//	FILE* fp = NULL;
//	fp = fopen("simple2.bin", "rb");
//	if (fp == NULL)
//	{
//		perror("file open error");
//	}
// 
//	int i = 0;
//	int var = 0;
//	int AluResult = 0;
//	int R_inst = 0;
//	int I_inst = 0;
//	int J_inst = 0;
//	int M_count = 0;
//	int B_count = 0;
//
//	// Initialization: Loading program
//	while (1)
//	{
//		int ret = fread(&var, sizeof(var), 1, fp);
//		if (ret == 0) break;
//
//#ifdef DEBUG_LOAD
//		printf("fread binary: 0x%x\n", var);
//#endif
//		unsigned int b1, b2, b3, b4;
//		b1 = var & 0x000000ff;
//		b2 = var & 0xff00;
//		b3 = var & 0x00ff0000;
//		b4 = (var >> 24) & 0xff;
//		int res = (b1 << 24) | (b2 << 8) | (b3 >> 8) | b4;
//
//#ifdef DEBUG_LOAD
//		printf("reordered data: 0x%x\n", res);
//#endif
//		int instruction = res;
//
//		memory[i] = instruction;
//		i++;
//	}
//	fclose(fp);
//
//	// Initialization: Registers
//	for (int k = 0; k < 32; k++)
//		Regs[k] = 0;
//	Regs[29] = 0x1000000;
//	Regs[31] = 0xffffffff;
//
//	while (1)
//	{
//		// terminal condition check
//		if (pc >= 0x1000000) break; //
//
//		// Instruction Fetch
//		int inst = memory[pc / 4];
//
//		// Instruction Decode
//		int opcode = (inst >> 26) & 0x3f;
//		int rs = (inst >> 21) & 0x1f;
//		int rt = (inst >> 16) & 0x1f;
//		int rd = (inst >> 11) & 0x1f;
//		int shamt = (inst >> 6) & 0x1f;
//		int func = inst & 0x3f;
//		int imm_ = inst & 0xffff;
//		int addr = inst & 0x3fffffff;
//		int s_imm = (imm_ & 0x8000) ? (imm_ | 0xffff0000) : (imm_);
//
//		// Excution loop
//		switch (opcode)
//		{
//		case 0x8: // Add Imm
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x AddI :  R[%d]: 0x%x + 0x%x -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rd, AluResult);
//			Regs[rd] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x9: // Add Imm. Unsigned
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x AddIU :  R[%d]: 0x%x + 0x%x -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rd, AluResult);
//			Regs[rd] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x23: // Load Word
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x LW:  M[ R[%d]:0x%x + 0x%x] -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = memory[AluResult / 4];
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x2b: // Store Word
//			AluResult = Regs[rs] + s_imm;
//			memory[AluResult / 4] = Regs[rt];
//			printf("@ 0x%x SW:  R[%d]: 0x%x -> M[ R[%d]:0x%x + S_imm 0x%x ]\n",
//				pc, rt, Regs[rt], rs, Regs[rs], s_imm);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x0: // op = 0 & func
//			switch (func)
//			{
//			case 0x20: // Add
//				AluResult = Regs[rs] + Regs[rt];
//				printf("@ 0x%x Add:  R[%d]: 0x%x + R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rt, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x21: // Add Unsigned
//				AluResult = Regs[rs] + Regs[rt];
//				printf("@ 0x%x AddU:  R[%d]: 0x%x + R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rt, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x08: // Jump Register
//				AluResult = Regs[rs] * 4;
//				printf("@ 0x%x JR:  R[%d]: 0x%x -> PC: 0x%x]\n",
//					pc, rs, Regs[rs], AluResult);
//				pc = AluResult;
//				R_inst = R_inst + 1;
//				break;
//
//			default:
//				printf("@ 0x%x Nop\n", pc);
//				pc = pc + 4;
//				break;
//			}
//			break;
//
//		default:
//			break;
//
//		}
//
//	}
//
//	// Output result & stats
//	printf("\n\nFinal return value Regs[2]: 0x%x\n", Regs[2]);
//	printf("Num of Executed Inst : R_inst: %d, I_inst: %d, J_inst:%d\n",
//		R_inst, I_inst, J_inst);
//	printf("Memory Access inst: %d\n", M_count);
//	printf("Num of taken Branch: %d\n", B_count);
//
//	return 0;
//}
