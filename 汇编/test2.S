X		EQU		11			; 定义X的值为11
Y		EQU		8			; 定义Y的值为8

BIT23	EQU		(1<<23)		; 定义BIT23的值为0x00800000
			
			AREA	Example2,CODE,READONLY	; 声明代码段Example2
			ENTRY				; 标识程序入口
			CODE32				; 声明32位ARM指令
			
START		; 使用MOV、ADD指令实现：R8 = R3 = X + Y
			MOV		R0,#X		; R0 <= X，X的值必须是8位图数据
			MOV		R1,#Y		; R1 <= Y，Y的值必须是8位图数据
			ADD		R3,R0,R1	; 即是R3 = X + Y
			MOV		R8,R3		; R8 <= R3
			
			
			; 使用MVN、SUB指令实现：R5 = 0x5FFFFFF8 - R8 * 8
			MVN		R0,#0xA0000007		; 0xA0000007的反码为0x5FFFFFF8
			SUB     R5,R0,R8,LSL #3		; R8左移3位，结果即是 R8 * 8
		
			
			; 使用CMP指令判断(5*Y/2)>(2*X)吗？若大于则R5 = R5&0xFFFF0000，否则R5 = R5|0x000000FF
			MOV		R0,#Y
			ADD		R0,R0,R0,LSL #2		; 计算R0 = Y + 4*Y = 5*Y
			MOV		R0,R0,LSR #1		; 计算R0 = 5*Y/2
			MOV		R1,#X
			MOV		R1,R1,LSL #1		; 计算R1 = 2*X
			CMP		R0,R1				; 比较R0和R1，即(5*Y/2)和(2*X)进行比较
			LDRHI   R2,=0xFFFF0000		; 若(5*Y/2)>(2*X)，则R2 <= 0xFFFF0000
			ANDHI	R5,R5,R2			; 若(5*Y/2)>(2*X)，则R5 = R5&R2
			ORRLS	R5,R5,#0x000000FF	; 若(5*Y/2)≤(2*X)，则R5 = R5|0x000000FF
			
			; 使用TST指令测试R5的bit23是否为1，若是则将bit6位清零(使用BIC指令)
			TST  	R5,#BIT23
			BICNE	R5,R5,#0x00000040			
			
			B		START
			
			END	