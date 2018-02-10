	AREA TEST,CODE,READONLY
	ENTRY
	CODE32
	;立即数寻址
	MOV R0,#0x1              ;立即数0x1存入寄存器R0
	ADD R0,R0,#0x4           ;寄存器R0中的内容加上立即数0x4 然后存入寄存器R0
	;寄存器寻址
	MOV R1,#0x2              ;立即数0x2存入寄存器R1
	ADD R2,R1,R0             ;寄存器R0中的内容和寄存器R1的内容相加存入寄存器R2
	ADD R2,R2,R0,LSL #1      ;寄存器R0中的内容逻辑左移1位加上寄存器R2的内容在存入寄存器R2
	;寄存器间接寻址
	MOV R1,#0x10             ;将立即数0x10存入寄存器R1
	STR R2,[R1]              ;将寄存器R2的内容存入以寄存器R1的值为地址的存储器中
	SWP R0,R0,[R1]           ;完成寄存器R1所指向的存储器中的字数据与寄存器R0的内容交换
	;基址变址寻址
	MOV R3,#0x14             ;将立即数0x14存入寄存器R3
	MOV R0,#0x3				 ;将立即数0x3存入寄存器R0
	STR R0,[R3]              ;将寄存器R0的内容存入寄存器R3所指向的存储器
	LDR R4,[R1,#4]           ;将寄存器R1的内容加上4所指向的存储器的字存入寄存器R4
	;多寄存器寻址
	LDMIA R1,{R5,R6}
	;相对寻址
	B NEXT
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
NEXT
	NOP
	NOP
	NOP
	END	
