//#include <stdio.h>
//#include <stdlib.h>
//
//int memory[0x40000] = {0, };
//
//typedef struct{
//	int reg[32];
//	unsigned int pc;
//}REG;
//
//typedef struct{
//	int address;
//	int opcode;
//	int rs;
//	int rt;
//	int rd;
//	int immediate;
//	int funct;
//	int shamt;
//}d_instruction;
//
//typedef struct{
//	int RegDst;
//	int Jump;
//	int Branch;
//	int MemRead;
//	int MemtoReg;
//	int ALUOp;
//	int MemWrite;
//	int ALUSrc;
//	int RegWrite;
//}ctrl_signal;
//
//typedef struct{
//	int alu_ctrl_num;
//}ALU_ctrl;
//
//typedef struct{
//	int Read_register1;
//	int Read_register2;
//	int Write_register;
//	int Write_data;
//	int Read_data1;
//	int Read_data2;
//}Registers;
//
//typedef struct{
//	int Address;
//	int Write_data;
//	int Read_data;
//}Data_MEM;
//
//typedef struct{
//	int ALU_result;
//}ALU_val;
//
//typedef struct{
//	int cycle_num;
//	int R_type_num;
//	int I_type_num;
//	int J_type_num;
//	int nop_num;
//	int MEM_Access_num;
//	int branch_num;
//	int branch_satisfied_num;
//}print_num;
//
//void print_num_set(print_num* p_n);
//void reg_set(REG* r);
//void ctrl_sig_set(ctrl_signal* c_s);
//int MUX(int input1, int input2, int sig);
//int store2memory();
//void Reg_val(int Read_reg1, int Read_reg2, int Write_reg, REG* r, Registers* R_v);
//void ctrl(d_instruction* d_i, ctrl_signal* c_s, print_num* p_n);
//void ALU_ctrl_sig(ctrl_signal* c_s, int opcode, int funct, ALU_ctrl* A_c);
//void ALU(int input1, int input2, int ALU_ctrl, ALU_val* A_v, print_num* p_n);
/////////////////////////////////////////////////////////////////
//void single_cycle(REG* r);
//unsigned int inst_fetch(REG* r);
//void inst_decode(int instruction, d_instruction* d_i, REG* r, ctrl_signal* c_s, print_num* p_n, ALU_ctrl* A_c, Registers* R_v);
//void execution(d_instruction* d_i, ctrl_signal* c_s, Registers* R_v, ALU_ctrl* A_c, ALU_val* A_v, print_num* p_n);
//void MEM_Access(Data_MEM* D_M, ALU_val* A_v, Registers* R_v, d_instruction* d_i, ctrl_signal* c_s, print_num* p_n);
//void Write_Back(REG* r, Data_MEM* D_M, d_instruction* d_i, ctrl_signal* c_s, Registers* R_v, ALU_val* A_v);
////////////////////////////////////////////////////////////////////
//void print_cycle_end(REG* r, print_num p_n);
//void print_cycle(REG* r, d_instruction* d_i);
//
//void main(){
//	REG r;
//	reg_set(&r);
//	
//	if(store2memory());
//	else return ;
//	single_cycle(&r);
//	
//	return ;
//}
//
//void print_num_set(print_num* p_n){
//	p_n->cycle_num = 1;
//	p_n->R_type_num = 0;
//	p_n->I_type_num = 0;
//	p_n->J_type_num = 0;
//	p_n->nop_num;
//	p_n->MEM_Access_num = 0;
//	p_n->branch_num = 0;
//	p_n->branch_satisfied_num = 0;
//}
//
//void reg_set(REG* r){
//	for(int n=0; n<32; n++){
//		r->reg[n] = 0;
//	}
//	r->reg[29] = 0x100000;
//	r->reg[31] = 0xffffffff;
//	r->pc = 0x0;
//	return ;
//}
//
//void ctrl_sig_set(ctrl_signal* c_s){
//	c_s->RegDst = 0;
//	c_s->Jump = 0;
//	c_s->Branch = 0;
//	c_s->MemRead = 0;
//	c_s->MemtoReg = 0;
//	c_s->ALUOp = 0;
//	c_s->MemWrite = 0;
//	c_s->ALUSrc = 0;
//	c_s->RegWrite = 0;
//}
//
//int MUX(int input1, int input2, int sig){
//	if(sig) return input2;
//	else return input1;
//}
//
//int store2memory(){
//	FILE* fp = NULL;
//	unsigned int data = 0;
//	int m = 0;
//	char* file_name;
//	file_name = (char*)malloc(sizeof(char)*20);
//
//	scanf("%s", file_name);
//	
//	fp = fopen("simple.bin", "rb");
//	if(fp == NULL){
//		printf("ERROR opening file!\n");
//		return 0;
//	}
//	while(fread(&data, sizeof(int), 1, fp)){
//	memory[m] = (((data & 0x000000ff) << 24) & 0xff000000) | (((data & 0x0000ff00) << 8) &0x00ff0000) | 
//	(((data & 0x00ff0000) >> 8) & 0xff00) | (((data & 0xff000000) >> 24) & 0xff);
//	m++;
//	}
//	fclose(fp);
//	return 1;
//}
//
//void Reg_val(int Read_reg1, int Read_reg2, int Write_reg, REG* r, Registers* R_v){
//	R_v->Read_register1 = Read_reg1;
//	R_v->Read_register2 = Read_reg2;
//	R_v->Write_register = Write_reg;
//	R_v->Read_data1 = r->reg[R_v->Read_register1];
//	R_v->Read_data2 = r->reg[R_v->Read_register2];
//	return ;
//}
//
//void ctrl(d_instruction* d_i, ctrl_signal* c_s, print_num* p_n){
//	ctrl_sig_set(c_s);
//	
//	if(d_i->opcode == 0){
//		c_s->RegDst = 1;
//		c_s->ALUOp = 1;
//		if((d_i->rs == 0x0) && (d_i->rd == 0x0) && (d_i->rt == 0x0) && (d_i->shamt == 0x0) && (d_i->funct == 0)) 
//			p_n->nop_num++;
//		else{
//			p_n->R_type_num++;
//		}
//	}
//	else{
//		if((d_i->opcode == 0x2) || (d_i->opcode == 0x3)){
//			p_n->J_type_num++;
//		}
//		else{
//			p_n->I_type_num++;
//		}
//	}
//	
//	if((d_i->opcode != 0) && (d_i->opcode != 0x4) && (d_i->opcode != 0x5)) 
//		c_s->ALUSrc = 1;
//	else ;
//	
//	if((d_i->opcode == 0x23) || (d_i->opcode == 0x24) || (d_i->opcode == 0x25) || (d_i->opcode == 0x30)) 
//		c_s->MemtoReg = 1;
//	else ;
//
//	if((d_i->opcode != 0x2b) && (d_i->opcode != 0x4) && (d_i->opcode != 0x5) && (d_i->opcode != 0x2) && (!((d_i->opcode == 0x0) && (d_i->funct == 0x8))) && (d_i->opcode != 0x28) && (d_i->opcode != 0x38) && (d_i->opcode != 0x29)) 
//		c_s->RegWrite = 1;
//	else ;
//
//	if((d_i->opcode == 0x23) || (d_i->opcode == 0x24) || (d_i->opcode == 0x25) || (d_i->opcode == 0x30)) 
//		c_s->MemRead = 1;
//	else ;
//	
//	if((d_i->opcode == 0x2b) || (d_i->opcode == 0x28) || (d_i->opcode == 0x38) || (d_i->opcode == 0x29)) 
//		c_s->MemWrite = 1;
//	else ;
//
//	if((d_i->opcode == 0x4) || (d_i->opcode == 0x5) || (d_i->opcode == 0x20)){
//		c_s->Branch = 1;
//		p_n->branch_num++;
//	}
//	else ;
//
//	if(((d_i->opcode == 0x0) && (d_i->funct == 0x8)) || (d_i->opcode == 0x2) || (d_i->opcode == 0x3)) 
//		c_s->Jump = 1;
//	else ;
//}
//
//void ALU_ctrl_sig(ctrl_signal* c_s, int opcode, int funct, ALU_ctrl* A_c){
//	if(c_s->ALUOp){
//		switch(funct){
//			case 0x20:
//			case 0x21:
//				A_c->alu_ctrl_num = 1;
//				return ;
//			case 0x24:
//				A_c->alu_ctrl_num = 2;
//				return ;
//			case 0x25:
//				A_c->alu_ctrl_num = 3;
//				return ;
//			case 0x27:
//				A_c->alu_ctrl_num = 10;
//				return ;
//			case 0x2a:
//			case 0x2b:
//				A_c->alu_ctrl_num = 4;
//				return ;
//			case 0x00:
//				A_c->alu_ctrl_num = 5;
//				return ;
//			case 0x02:
//			case 0x03:
//				A_c->alu_ctrl_num = 6;
//				return ;
//			case 0x22:
//			case 0x23:
//				A_c->alu_ctrl_num = 7;
//				return ;
//			case 0x8:
//			case 0x9:
//				A_c->alu_ctrl_num = 12;
//				return ;
//			default:
//				return ;
//		}
//	}
//	else{
//		switch(opcode){
//			case 0x8:
//			case 0x9:
//			case 0x24:
//			case 0x25:
//			case 0x30:
//			case 0x23:
//			case 0x28:
//			case 0x38:
//			case 0x29:
//			case 0x2b:
//				A_c->alu_ctrl_num = 1;
//				return ;
//			case 0xc:
//				A_c->alu_ctrl_num = 2;
//				return ;
//			case 0x4:
//			case 0x20:
//				A_c->alu_ctrl_num = 8;
//				return ;
//			case 0x5:
//				A_c->alu_ctrl_num = 9;
//				return ;
//			case 0xd:
//				A_c->alu_ctrl_num = 3;
//				return;
//			case 0xa:
//			case 0xb:
//				A_c->alu_ctrl_num = 4;
//				return ;
//			case 0xf:
//				A_c->alu_ctrl_num = 11;
//				return ;
//			default:
//				return ;
//		}
//	}
//}
//
//void ALU(int input1, int input2, int ALU_ctrl, ALU_val* A_v, print_num* p_n){
//	switch(ALU_ctrl){
//		case 0:
//			A_v->ALU_result = 0;
//			return ;
//		case 1:
//			A_v->ALU_result = input1 + input2;
//			return ;
//		case 2:
//			A_v->ALU_result = input1 & input2;
//			return ;
//		case 3:
//			A_v->ALU_result = input1 | input2;
//			return ;
//		case 4:
//			if(input1<input2){
//				A_v->ALU_result = 1;
//			}
//			else{
//				A_v->ALU_result = 0;
//			}
//			return ;
//		case 5:
//			A_v->ALU_result = input1 << input2;
//			return ;
//		case 6:
//			A_v->ALU_result = input1 >> input2;
//			return ;
//		case 7:
//			A_v->ALU_result = input1 - input2;
//			return ;
//		case 8:
//			if(input1 == input2){
//				A_v->ALU_result = 1;
//				p_n->branch_satisfied_num++;
//			}
//			else A_v->ALU_result = 0;
//			return ;
//		case 9:
//			if(input1 != input2){
//				A_v->ALU_result = 1;
//				p_n->branch_satisfied_num++;
//			}
//			else A_v->ALU_result = 0;
//			return ;
//		case 10:
//			A_v->ALU_result = ~(input1 | input2);
//			return ;
//		case 11:
//			A_v->ALU_result = (input2 << 16) & 0xffff0000;
//			return ;
//		case 12:
//			A_v->ALU_result = input1;
//		default:
//			return ;
//	}
//}
//
//void single_cycle(REG* r){
//	print_num p_n;
//	print_num_set(&p_n);
//	unsigned int instruction;
//	while(r->pc != 0xffffffff){
//		d_instruction d_i;
//		ctrl_signal c_s;
//		ALU_ctrl A_c;
//		Registers R_v;
//		Data_MEM D_M;
//		ALU_val A_v;
//		//printf("--------------------cycle %d--------------------\n", p_n.cycle_num);
//		//printf("pc = 0x%x\n", r->pc);
//		instruction = inst_fetch(r);
//		p_n.cycle_num++;
//		inst_decode(instruction, &d_i, r, &c_s, &p_n, &A_c, &R_v);
//		p_n.cycle_num++;
//		execution(&d_i, &c_s, &R_v, &A_c, &A_v, &p_n);
//		p_n.cycle_num++;
//		MEM_Access(&D_M, &A_v, &R_v, &d_i, &c_s, &p_n);
//		p_n.cycle_num++;
//		Write_Back(r, &D_M, &d_i, &c_s, &R_v, &A_v);
//		p_n.cycle_num++;
//		//print_cycle(r, &d_i);
//	}
//	print_cycle_end(r, p_n);
//}
//
//unsigned int inst_fetch(REG* r){
//	unsigned int instruction;
//	instruction = memory[r->pc/4];
//	r->pc = r->pc + 4;
//	return instruction;
//}
//
//void inst_decode(int instruction, d_instruction* d_i, REG* r, ctrl_signal* c_s, print_num* p_n, ALU_ctrl* A_c, Registers* R_v){
//	d_i->address = ((instruction & 0x03ffffff) << 2) | ((r->pc+4) & 0xf0000000);
//	d_i->opcode = (instruction & 0xfc000000) >> 26;
//	d_i->rs = (instruction & 0x03e00000) >> 21;
//	d_i->rt = (instruction & 0x001f0000) >> 16;
//	d_i->rd = (instruction & 0x0000f800) >> 11;
//	d_i->immediate = instruction & 0xffff;
//	d_i->funct = instruction & 0x3f;
//	d_i->shamt = (instruction & 0x7c0) >> 6;
//	ctrl(d_i, c_s, p_n);
//	ALU_ctrl_sig(c_s, d_i->opcode, d_i->funct, A_c);
//	if((d_i->opcode == 0xc) || (d_i->opcode == 0xd) || (d_i->opcode == 0xf));
//	else{
//		if((d_i->immediate) & 0x8000)
//		{
//		d_i->immediate = d_i->immediate | 0xffff0000;
//		}
//	}
//	if(d_i->opcode == 0x3){
//		Reg_val(d_i->rs, d_i->rt, 31, r, R_v);
//	}
//	else{
//		Reg_val(d_i->rs, d_i->rt, MUX(d_i->rt, d_i->rd, c_s->RegDst), r, R_v);
//	}
//}
//
//void execution(d_instruction* d_i, ctrl_signal* c_s, Registers* R_v, ALU_ctrl* A_c, ALU_val* A_v, print_num* p_n){
//	if((d_i->opcode == 0xc) || (d_i->opcode == 0xd)){
//		ALU(R_v->Read_data1, MUX(R_v->Read_data2, d_i->immediate | 0x00000000, c_s->ALUSrc), A_c->alu_ctrl_num, A_v, p_n);
//		return ;
//	}
//	else if((d_i->opcode == 0x0) && ((d_i->funct == 0x0) || (d_i->funct == 0x2) || (d_i->funct == 0x3))){
//		ALU(R_v->Read_data2, d_i->shamt, A_c->alu_ctrl_num, A_v, p_n);
//		return ;
//	}
//	else{
//		ALU(R_v->Read_data1, MUX(R_v->Read_data2, d_i->immediate, c_s->ALUSrc), A_c->alu_ctrl_num, A_v, p_n);
//		return ;
//	}
//}
//
//void MEM_Access(Data_MEM* D_M, ALU_val* A_v, Registers* R_v, d_instruction* d_i, ctrl_signal* c_s, print_num* p_n){
//	D_M->Address = A_v->ALU_result;
//	D_M->Write_data = R_v->Read_data2;
//	if(c_s->MemWrite || c_s->MemRead){
//		p_n->MEM_Access_num++;
//		if(c_s->MemWrite){
//			D_M->Read_data = 0;
//			switch(d_i->opcode){
//				case 0x28:
//					memory[(D_M->Address/4)-1] = D_M->Write_data & 0x000000ff;
//					return ;
//				case 0x29:
//					memory[(D_M->Address/4)-1] = D_M->Write_data & 0x0000ffff;
//					return ;
//				case 0x2b:
//				case 0x38:
//					memory[(D_M->Address/4)-1] = D_M->Write_data;
//					return ;
//			}
//		}
//		else ;
//		if(c_s->MemRead){
//			switch(d_i->opcode){
//				case 0x24:
//					D_M->Read_data = memory[(D_M->Address/4)-1] & 0x000000ff;
//					return ;
//				case 0x25:
//					D_M->Read_data = memory[(D_M->Address/4)-1] & 0x0000ffff;
//					return ;
//				case 0x23:
//				case 0x30:
//					D_M->Read_data = memory[(D_M->Address/4)-1];
//					return ;
//			}
//		}
//		else ;
//	}
//	else{
//		D_M->Read_data = 0;
//		return ;
//	}
//}
//
//void Write_Back(REG* r, Data_MEM* D_M, d_instruction* d_i, ctrl_signal* c_s, Registers* R_v, ALU_val* A_v){
//	R_v->Write_data = MUX(A_v->ALU_result, D_M->Read_data,  c_s->MemtoReg);
//	if(c_s->RegWrite){
//		if(d_i->opcode == 0x3){
//			r->reg[31] = r->pc+4;
//		}
//		else if((d_i->opcode == 0x0) && (d_i->funct == 0x9)){
//			r->reg[R_v->Write_register] = r->pc;
//		}
//		else{
//			r->reg[R_v->Write_register] = R_v->Write_data;
//		}
//	}
//	if((d_i->opcode == 0x0) && ((d_i->funct == 0x8) || (d_i->funct == 0x9))){
//		r->pc = R_v->Read_data1;
//	}
//	else{
//		r->pc = MUX(MUX(r->pc, (d_i->immediate << 2)+(r->pc), (c_s->Branch & A_v->ALU_result)), d_i->address, c_s->Jump);
//	}
//}
//
//void print_cycle_end(REG* r, print_num p_n){
//	printf("--------------------end--------------------\n");
//	printf("executed instruction number: %d\n", p_n.cycle_num - 1);
//	printf("R-type: %d\n", p_n.R_type_num);
//	printf("I-type: %d\n", p_n.I_type_num);
//	printf("J-type: %d\n", p_n.J_type_num);
//	printf("nop: %d\n", p_n.nop_num);
//	printf("Memory Access number: %d\n", p_n.MEM_Access_num);
//	printf("branch number: %d\n", p_n.branch_num);
//	printf("branch satisfied: %d\n", p_n.branch_satisfied_num);
//	printf("result = %d\n", r->reg[2]);
//}
//
//void print_cycle(REG* r, d_instruction* d_i){
//	printf("sp = 0x%x\n", r->reg[29]);
//	printf("s8 = 0x%x\n", r->reg[30]);
//	printf("opcode = 0x%x\n", d_i->opcode);
//	printf("rs = %d\n", d_i->rs);
//	printf("rt = %d\n", d_i->rt);
//	printf("rd = %d\n", d_i->rd);
//	printf("immediate = %d\n", d_i->immediate);
//	printf("funct = 0x%x\n", d_i->funct);
//	printf("shamt = %d\n", d_i->shamt);
//	printf("address = 0x%x\n", d_i->address);
//	printf("ra = 0x%x\n", r->reg[31]);
//	printf("v0 = %d\n", r->reg[2]);
//	printf("v1 = %d\n", r->reg[3]);
//}
