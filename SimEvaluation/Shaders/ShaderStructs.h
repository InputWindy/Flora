#ifndef SHADER_STRUCT_H
#define SHADER_STRUCT_H

struct FNCCContext
{
	vec4 Result;

	vec4 T_Mul_I_Sum;
	vec4 T_2_Sum;
	vec4 I_2_Sum;

	int Lock0;
	int Lock1;
	int Lock2;
};

#endif
