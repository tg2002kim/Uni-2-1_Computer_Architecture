//#pragma warning(disable:4996)
//
//#include <stdio.h>
//#include <errno.h>
////#define DEBUG_LOAD
//
//// MIPS single cycle uarch emulator
//
//// Memory is a global data structure
//int memory[0x400000];
//unsigned int pc = 0;
//int Regs[32] = { 0, };
//
//struct inst
//{
//	int opcode;
//	int rs;
//	int rd;
//	int rt;
//	int shamt;
//	int func;
//	int imm_;
//	int addr;
//	int s_imm;
//	int z_imm;
//	int b_addr;
//	int j_addr;
//};
//// cache 구현
//typedef struct {
//	int tag;
//	int valid;
//	int s_ch;
//	int dirty;
//	int data[16];
//}cacheline;
//
//cacheline cache[64];
//int oldest;
//int cache_hit;
//int cache_miss;
//
//int Read_Memory(unsigned int Addr)
//{
//	int tmp = Addr - (Addr % 64);
//	int tag = (Addr / 4 & 0xfffffff0) >> 4;
//	int offset = Addr & 0x0000003f;
//
//	for (int i = 0; i < 64; i++) {
//		if ((cache[i].tag == tag) && (cache[i].valid == 1)) {
//			cache[i].s_ch = 1;
//			cache_hit++;
//			return cache[i].data[offset];
//		}
//	}
//
//	cache_miss++;
//	for (int i = 0; i < 64; i++) {
//		if (cache[i].valid == 0) {
//			cache[i].valid = 1;
//			cache[i].tag = tag;
//			for (int n = 0; n < 16; n++) {
//				cache[i].data[n] = memory[tmp / 4];
//				tmp = tmp + 4;
//			}
//			return memory[Addr / 4];
//		}
//	}
//	while (1) {
//		if (cache[oldest].s_ch == 0) {
//			if (cache[oldest].dirty == 1) {
//				tmp = cache[oldest].tag << 6;
//				for (int n = 0; n < 16; n++) {
//					memory[tmp / 4] = cache[oldest].data[n];
//					tmp = tmp + 4;
//				}
//			}
//			cache[oldest].tag = tag;
//			tmp = Addr - (Addr % 64);
//			for (int n = 0; n < 16; n++) {
//				cache[oldest].data[n] = memory[tmp / 4];
//				tmp = tmp + 4;
//			}
//			oldest = oldest + 1;
//			if (oldest == 64) {
//				oldest = 0;
//			}
//			return memory[Addr / 4];
//		}
//		else {
//			cache[oldest].s_ch = 0;
//			oldest = oldest + 1;
//			if (oldest == 64) {
//				oldest = 0;
//			}
//		}
//	}
//}
//
//void Write_Memory(int Addr, int val) 
//{
//	int tmp = Addr - (Addr % 64);
//	int tag = (Addr / 4 & 0xfffffff0) >> 4;
//	int offset = (Addr / 4 & 0xf);
//	//////////////////////////////cache hit
//	for (int i = 0; i < 64; i++) {
//		if ((cache[i].tag == tag) && (cache[i].valid == 1)) {
//			cache[i].s_ch = 1;
//			cache[i].dirty = 1;
//			cache[i].data[offset] = val;
//			cache_hit++;
//			return;
//		}
//	}
//	//////////////////////////////cache cold miss
//	cache_miss++;
//	for (int i = 0; i < 64; i++) {
//		if (cache[i].valid == 0) {
//			cache[i].valid = 1;
//			cache[i].tag = tag;
//			for (int n = 0; n < 16; n++) {
//				cache[i].data[n] = memory[tmp / 4];
//				tmp = tmp + 4;
//			}
//			cache[i].dirty = 1;
//			cache[i].data[offset] = val;
//			return;
//		}
//	}
//	////////////////////////////cache conflict miss
//	while (1) {
//		if (cache[oldest].s_ch == 0) {
//			if (cache[oldest].dirty == 1) {
//				tmp = (cache[oldest].tag << 6);
//				for (int n = 0; n < 16; n++) {
//					memory[tmp / 4] = cache[oldest].data[n];
//					tmp = tmp + 4;
//				}
//			}
//			cache[oldest].tag = tag;
//			tmp = Addr - (Addr % 64);
//			for (int n = 0; n < 16; n++) {
//				cache[oldest].data[n] = memory[tmp / 4];
//				tmp = tmp + 4;
//			}
//			cache[oldest].dirty = 1;
//			cache[oldest].data[offset] = val;
//			oldest = oldest + 1;
//			if (oldest == 64) {
//				oldest = 0;
//			}
//			return;
//		}
//		else {
//			cache[oldest].s_ch = 0;
//			oldest = oldest + 1;
//			if (oldest == 64) {
//				oldest = 0;
//			}
//		}
//	}
//}
//
//void print_inst(struct inst in)
//{
//	printf("pc: 0x%x opcode: 0x%x rs: 0x%x rd: 0x%x rt: 0x%x shamt: 0x%x func: 0x%x imm_: 0x%x s_imm: 0x%x z_imm: 0x%x, b_addr:0x%x, j_addr 0x%x\n",
//		pc, in.opcode, in.rs, in.rt, in.rd, in.shamt, in.func, in.imm_, in.s_imm, in.z_imm, in.b_addr, in.j_addr);
//	printf("R[rs]: 0x%x R[rt]: 0x%x R[rd]: 0x%x\n",
//		Regs[in.rs], Regs[in.rt], Regs[in.rd]);
//}
//
//int main(int argc, char* argv[])
//{
//
//	// test for checking binary input/output
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
//	int AluResult = 0;
//	int R_inst = 0;
//	int I_inst = 0;
//	int J_inst = 0;
//	int Nop = 0;
//	int M_count = 0;
//	int B_count = 0;
//	int B_S_count = 0;
//
//	// Initialization: Loading program
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
//		if (pc == 0xffffffff) break;
//
//		// Instruction Fetch
//		int inst = Read_Memory(pc);
//
//		// Instruction Decode
//		int opcode = (inst >> 26) & 0x3f;
//		int rs = (inst >> 21) & 0x1f;
//		int rt = (inst >> 16) & 0x1f;
//		int rd = (inst >> 11) & 0x1f;
//		int shamt = (inst >> 6) & 0x1f;
//		int func = inst & 0x0000003f;
//		int imm_ = inst & 0x0000ffff;
//		int addr = inst & 0x03ffffff;
//		int s_imm = (imm_ & 0x8000) ? (imm_ | 0xffff0000) : (imm_);
//		int z_imm = imm_ & 0x0000ffff;
//		int b_addr = s_imm * 4;
//		int j_addr = ((pc + 4) & 0xf0000000) | (addr << 2);
//		int test = inst;
//
//		struct inst i_;
//		i_.opcode = opcode;
//		i_.rs = rs;
//		i_.rd = rd;
//		i_.rt = rt;
//		i_.shamt = shamt;
//		i_.func = func;
//		i_.imm_ = imm_;
//		i_.addr = addr;
//		i_.s_imm = s_imm;
//		i_.z_imm = z_imm;
//		i_.b_addr = b_addr;
//		i_.j_addr = j_addr;
//
//
//		// Excution loop
//		switch (opcode)
//		{
//		case 0x8: // Add Imm
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x AddI :  R[%d]: 0x%x + 0x%x -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x9: // Add Imm. Unsigned
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x AddIU:  R[%d]: 0x%x + 0x%x -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0xc: // And imm
//			AluResult = Regs[rs] & z_imm;
//			printf("@ 0x%x ANDI:  R[%d] : 0x%x & 0x%x -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], z_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x4: // Branch On Equal
//			if (Regs[rs] == Regs[rt])
//			{
//				AluResult = pc + 4 + b_addr;
//				B_S_count = B_S_count + 1;
//			}
//			else
//			{
//				AluResult = pc + 4;
//			}
//			printf("@ 0x%x BOE:  if(R[%d]: 0x%x == R[%d]: 0x%x) pc + 4 + b_addr: 0x%x // pc + 4 -> pc: 0x%x\n",
//				pc, rs, Regs[rs], rt, Regs[rt], b_addr, AluResult);
//			pc = AluResult;
//			B_count = B_count + 1;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x5: // Branch On Not Equal
//			if (Regs[rs] != Regs[rt])
//			{
//				AluResult = pc + 4 + b_addr;
//				B_S_count = B_S_count + 1;
//			}
//			else
//			{
//				AluResult = pc + 4;
//			}
//			printf("@ 0x%x BNE:  if(R[%d]: 0x%x != R[%d]: 0x%x) pc + 4 + b_addr: 0x%x // pc + 4 -> pc: 0x%x\n",
//				pc, rs, Regs[rs], rt, Regs[rt], b_addr, AluResult);
//			pc = AluResult;
//			B_count = B_count + 1;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x2: // Jump
//			printf("@ 0x%x Jump:  j_addr: 0x%x -> pc: 0x%x\n",
//				pc, j_addr, j_addr);
//			pc = j_addr;
//			J_inst = J_inst + 1;
//			break;
//
//		case 0x3: // Junp And Link
//			AluResult = pc + 8;
//			printf("@ 0x%x JAL:  pc + 8 -> Regs[31], j_addr: 0x%x -> pc: 0x%x\n",
//				pc, j_addr, AluResult);
//			Regs[31] = pc + 8;
//			pc = j_addr;
//			J_inst = J_inst + 1;
//			break;
//
//		case 0xf: // Load Upper Imm
//			AluResult = (imm_ << 16);
//			printf("@ 0x%x LUI  : imm: 0x%x & 16'b0 -> R[%d]: 0x%x",
//				pc, imm_, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x23: // Load Word
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x LW:  M[ R[%d]:0x%x + 0x%x] -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = Read_Memory(AluResult);
//			pc = pc + 4;
//			M_count = M_count + 1;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0xd: // OR Imm
//			AluResult = Regs[rs] | z_imm;
//			printf("@ 0x%x ORI: R[%d]: 0x%x + z_imm: 0x%x -> R[%d] 0x%x\n",
//				pc, rs, Regs[rs], z_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0xa: // Set Less Than Imm
//			AluResult = (Regs[rs] < s_imm) ? 1 : 0;
//			printf("@ 0x%x SLTI: (R[%d]: 0x%x < s_imm: 0x%x) ? 1 : 0 -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0xb: // Set Less Than Imm. Unsigned
//			AluResult = (Regs[rs] < s_imm) ? 1 : 0;
//			printf("@ 0x%x SLTIU: (R[%d]: 0x%x | s_imm: 0x%x) ? 1 : 0 -> R[%d]: 0x%x\n",
//				pc, rs, Regs[rs], s_imm, rt, AluResult);
//			Regs[rt] = AluResult;
//			pc = pc + 4;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x2b: // Store Word
//			AluResult = Regs[rs] + s_imm;
//			printf("@ 0x%x SW:  R[%d]: 0x%x -> M[ R[%d]:0x%x + s_imm 0x%x ]\n",
//				pc, rt, Regs[rt], rs, Regs[rs], s_imm);
//			Write_Memory(AluResult, Regs[rt]);
//			pc = pc + 4;
//			M_count = M_count + 1;
//			I_inst = I_inst + 1;
//			break;
//
//		case 0x0: // op = 0 & func
//			switch (func)
//			{
//			case 0x20: // Add
//				AluResult = Regs[rs] + Regs[rt];
//				printf("@ 0x%x Add:  R[%d]: 0x%x + R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x21: // Add Unsigned
//				AluResult = Regs[rs] + Regs[rt];
//				printf("@ 0x%x AddU:  R[%d]: 0x%x + R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x24: // And
//				AluResult = Regs[rs] & Regs[rt];
//				printf("@ 0x%x AND:  R[%d]: 0x%x & Regs[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x08: // Jump Register
//				AluResult = Regs[rs];
//				printf("@ 0x%x JR:  R[%d]: 0x%x -> pc: 0x%x\n",
//					pc, rs, Regs[rs], AluResult);
//				pc = AluResult;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x27: // NOR
//				AluResult = !(Regs[rs] | Regs[rt]);
//				printf("@ 0x%x NOR:  ~ (R[%d]: 0x%x | R[%d]: 0x%x) -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x25: // OR
//				AluResult = Regs[rs] | Regs[rt];
//				printf("@ 0x%x OR:  R[%d]: 0x%x | R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x2a: // Set Less Than
//				AluResult = (Regs[rs] < Regs[rt]) ? 1 : 0;
//				printf("@ 0x%x SLT:  (R[%d]: 0x%x | R[%d]: 0x%x) ? 1 : 0 -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x2b: // Set Less Than Unsigned
//				AluResult = (Regs[rs] < Regs[rt]) ? 1 : 0;
//				printf("@ 0x%x SLTU:  (R[%d]: 0x%x | R[%d]: 0x%x) ? 1 : 0 -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x01: // Shift Left Logical
//				AluResult = (Regs[rt] << shamt);
//				printf("@ 0x%x SLL:  R[%d]: 0x%x << shamt: 0x%x -> R[%d]: 0x%x\n",
//					pc, rt, Regs[rt], shamt, rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x02: // Shift Right Logical
//				AluResult = (Regs[rt] >> shamt);
//				printf("@ 0x%x SLL:  R[%d]: 0x%x << shamt: 0x%x -> R[%d]: 0x%x\n",
//					pc, rt, Regs[rt], shamt, rd, AluResult);
//				pc = pc + 4;
//				Regs[rd] = AluResult;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x22: // Subtract
//				AluResult = Regs[rs] - Regs[rt];
//				printf("@ 0x%x Sub:  R[%d]: 0x%x - R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//			case 0x23: // Subtract Unsigned
//				AluResult = Regs[rs] - Regs[rt];
//				printf("@ 0x%x Aub:  R[%d]: 0x%x - R[%d]: 0x%x -> R[%d]: 0x%x\n",
//					pc, rs, Regs[rs], rt, Regs[rt], rd, AluResult);
//				Regs[rd] = AluResult;
//				pc = pc + 4;
//				R_inst = R_inst + 1;
//				break;
//
//				//case 0x9:  // Jump And Link Register
//				//	AluResult = Regs[rs];
//				//	printf("@ 0x%x JALR: Regs[%d]: 0x%x -> Regs[31], jump_target_address: 0x%x -> pc: 0x%x\n",
//				//		pc, rs, Regs[rs], Regs[rs], AluResult);
//				//	Regs[31] = AluResult;
//				//	int JALR = 0x00;	// 이전의 rs값과 funct에 0x8(JR)을 넣어 8자리 명령어를 만든다.
//				//	memory[(pc + 4) / 4] = JALR;
//				//	pc = pc + 4;
//				//	J_inst = J_inst + 1;
//				//	break;
//
//
//			default:
//				printf("@ 0x%x Nop\n", pc);
//				Nop = Nop + 1;
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
//	printf("Num of Executed Inst: %d\n(R_inst: %d, I_inst: %d, J_inst:%d, Nop: %d)\n",
//		R_inst + I_inst + J_inst + Nop, R_inst, I_inst, J_inst, Nop);
//	printf("Memory Access inst: %d\n", M_count);
//	printf("Num of Branch: %d\nNum of Branch taken: %d\n\n", B_count, B_S_count);
//	printf("cache hit / miss num || cache hit rate: %d / %d || %d\n", cache_hit, cache_miss, cache_hit + cache_miss);
//	
//	return 0;
//}
