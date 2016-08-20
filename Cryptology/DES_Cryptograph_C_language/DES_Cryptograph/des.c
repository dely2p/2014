#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define XOR(a, b) (a || b) && !(a && b)

void Chartobinary(char *in_c, int *in_b);
void InitialPermutation(int *input_b, int *input_chgb);
void RoundKeyGenerator(int *key);
void ParityDrop(int *key_b, int *key_parity);
void ShiftLeft(int *preshiftkey, int *proshiftkey, int rnum);
void CompressionPBox(int *profstkey, int *proseckey, int rnum);
void Round_Func(int *in_chgb, int roundnum, int boolnum);
void Des_Func(int *input_chgb,int roundnum);
void SBoxes(int *temp);
void Exp_PBox(int *R);
void Str_PBox(int *s_outb);
void Final_permutation(int *in_b, int *output_b);
void Binarytochar(int *output_b, char *output_c);
int XOR_Func(int in1, int in2);

char test;
char input_c[9],key_c[9];
int input_b[64], input_chgb[64];
int key_b[64], key_parity[56];
int fstkey[28],seckey[28];
int profstkey[28],proseckey[28];
int procomp[48];
int L[32], R[32];
int exp_R[48];
int roundkey[16][48];
int s_outb[32];
int des_fout[32];
int pro16r[64];
int output_b[64];
char output_c[8];
char decryp_key[9];
int decryproundkey[16][48];
int boolnum = 0;


//초기치환 P-박스 
char InitialPermutationTable[64] = { 58, 50, 42, 34, 26, 18, 10, 2,   
									60, 52, 44, 36, 28, 20,12, 4,
									62, 54 ,46 ,38 ,30 ,22 ,14 ,6, 
									64, 56, 48, 40, 32, 24, 16, 8, 
									57, 49, 41, 33, 25, 17, 9, 1,
									59, 51, 43, 35, 27, 19, 11, 3,
									61, 53, 45, 37, 29, 21, 13, 5,
									63, 55, 47, 39, 31, 23, 15, 7 };

//최종치환 P-박스
char FinalPermutationTable[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 
									39, 7, 47, 15, 55, 23, 63, 31, 
									38, 6, 46, 14, 54, 22, 62, 30, 
									37, 5, 45, 13, 53, 21, 61, 29,
									36, 4, 44, 12, 52, 20, 60, 28,
									35, 3, 43, 11, 51, 19, 59, 27,
									34, 2, 42, 10, 50, 18, 58, 26,
									33, 1, 41, 9, 49, 17, 57, 25 };

//확장 P-박스 
char ExpansionPermutationTable[48] = { 32, 1, 2, 3, 4, 5,   
									4, 5, 6, 7, 8, 9, 
									8, 9, 10, 11, 12, 13, 
									12, 13, 14, 15, 16, 17,
									16, 17, 18, 19, 20, 21,
									20, 21, 22, 23, 24, 25,
									24, 25, 26, 27, 28, 29,
									28, 29, 30, 31, 32, 1 };


//단순 P-박스(단순치환)
char StraightPermutationTable[32] = { 16,  7, 20, 21, 29, 12, 28, 17, 
									1, 15, 23, 26, 5, 18, 31, 10, 
									2,  8, 24, 14, 32, 27, 3, 9, 
									19, 13, 30, 6, 22, 11, 4, 25 };

//64비트로 이루어진 8개의 S-박스 
char S[8][4][16] = {        
 
   //S-박스1
   {{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
    { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8}, 
    { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
    {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}  },
  
  //S-박스2
   {{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
    { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5}, 
    { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
    {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}  },

  //S-박스3
   {{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
    {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1}, 
    {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
    { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}  },
  
  
  //S-박스4
   {{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
    {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9}, 
    {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
    { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}  },
  
  //S-박스5
   {{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
    {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6}, 
    { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
    {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}  },
  
  
  //S-박스6
   {{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11}, 
    {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8}, 
    { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
    { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}  },
  
  
  //S-박스7
   {{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
    {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6}, 
    { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
    { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}  },
  
  
  //S-박스8
  {{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
   { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2}, 
   { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
   { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}  }
 };

//패리티 비트 제거 표
char ParityDropTable[64] = { 57, 49, 41, 33, 25, 17, 9, 1,
							58, 50, 42, 34, 26, 18, 10, 2,
							59, 51, 43, 35, 27, 19, 11, 3,
							60, 52, 44, 36, 63, 55, 47, 39,
							31, 23, 15, 7, 62, 54, 46, 38,
							30, 22, 14, 6, 61, 53, 45, 37,
							29, 21, 13, 5, 28, 20, 12, 4 };

//축소 치환 P박스
char CompressionPBoxTable[48] = { 14, 17, 11, 24, 1, 5, 3, 28,
								15, 6, 21, 10, 23, 19, 12, 4,
								26, 8, 16, 7, 27, 20, 13, 2,
								41, 52, 31, 37, 47, 55, 30, 40,
								51, 45, 33, 48, 44, 49, 39, 56,
								34, 53, 46, 42, 50, 36, 29, 32 };

void main(){

	int i,j;
	
	printf("문장 입력: ");						// 평문 입력받기
	scanf("%s",&input_c);

	printf("key 입력: ");						// key 입력받기
	scanf("%s",&key_c);

	Chartobinary(input_c, input_b);				// 입력받은 char값을 binary로 변환
	Chartobinary(key_c, key_b);					// 입력받은 char key값을 binary로 변환

	//for(i=0;i<64;i++){
	//	printf("%d",input_b[i]);
	//	if(i!=0 && (i+1)%8==0)
	//		printf("\n");
	//}
	//printf("\n");

	InitialPermutation(input_b, input_chgb);	// 초기치환
	RoundKeyGenerator(key_b);						// 라운드 키 생성
	
	//for(i=0;i<64;i++){
	//	printf("%d",input_chgb[i]);
	//	if(i!=0 && (i+1)%8==0)
	//		printf("\n");
	//}
	//printf("\n");
	for(i=0;i<16;i++){
		if(i==16)
			boolnum=1;
		Round_Func(input_chgb,i,boolnum);				// 라운드 함수 16번 반복
	}

	for(i=0;i<32;i++){
		pro16r[i] = L[i];
		pro16r[i+32] = R[i]; 
	}
	Final_permutation(pro16r,output_b);			// Final 치환

	Binarytochar(output_b,output_c);			// char로 변환

	printf("암호화 된 문장 : ");
	for(i=0;i<8;i++){
		printf("%c",output_c[i]);
	}
	printf("\n");
	
	printf("key 입력 : ");						//복호화
	gets(decryp_key);

	Chartobinary(output_c, input_b);				// 입력받은 char값을 binary로 변환
	Chartobinary(decryp_key, key_b);					// 입력받은 char key값을 binary로 변환


	InitialPermutation(input_b, input_chgb);	// 초기치환
	RoundKeyGenerator(key_b);						// 라운드 키 생성

	for(i=0;i<16;i++){
		for(j=0;j<48;j++){
			decryproundkey[i][j] = roundkey[15-i][j]; 
		}
	}

	for(i=0;i<16;i++){
		if(i==16)
			boolnum=1;
		Round_Func(input_chgb,15-i,boolnum);				// 라운드 함수 16번 반복
	}
	for(i=0;i<32;i++){
		pro16r[i] = L[i];
		pro16r[i+32] = R[i]; 
	}

	Final_permutation(pro16r,output_b);			// Final 치환

	Binarytochar(output_b,output_c);			// char로 변환

	printf("복호화 된 문장 : ");
	for(i=0;i<8;i++){
		printf("%c",output_c[i]);
	}
	printf("\n");




	
} 

void Chartobinary(char *in_c, int *in_b){		//
	
	int i,j,k,int_val=0,quot=0,rem=0;
	int temp[8];

	for(k=0;k<8;k++){
		int_val = (int)in_c[k]; //char값을 dec로 변환
		
		for(j=7;j>=0;j--){		//dec값을 binary로 변환
			quot = int_val/2;
			rem = int_val%2;
			temp[j] = rem;
			int_val = quot;
		}
		for(i=0;i<8;i++){		//변환한 값을 배열에 저장
			in_b[8*k+i] = temp[i];
		}
	}	
	//Binarytochar(in_b,in_c);
	//printf("binary->char");
	//for(i=0;i<8;i++)
	//	printf("%c",in_c[i]);
}

void InitialPermutation(int *in_b, int *in_chgb){

	int i,temp[64];

	//for(i=0;i<64;i++)
	//	printf("%d",in_b[i]);

	for(i=0;i<64;i++)
		temp[i] = in_b[InitialPermutationTable[i]-1];	
	
	for(i=0;i<64;i++){
		in_chgb[i] = temp[i];
	}
	//printf("\n");
	//for(i=0;i<64;i++)
	//	printf("%d",in_chgb[i]);
	

	//for(i=0;i<64;i++){
	//	printf("%d",input_chgb[i]);
	//	if(i!=0 && (i+1)%8==0)
	//		printf("\n");
	//}
	printf("\n");

}

void RoundKeyGenerator(int *key){
	
	int i,j,rnum;

	ParityDrop(key,key_parity);				// 패리티 제거

	for(i=0;i<56;i++){							// 28bit씩 나누기
		fstkey[i] = key_parity[i];
		seckey[i] = key_parity[i+28];
	}
	for(rnum=1;rnum<=16;rnum++){					// 각 라운드 별 좌측 순환 이동
		ShiftLeft(fstkey,profstkey,rnum);							
		ShiftLeft(seckey,proseckey,rnum);		
		CompressionPBox(profstkey, proseckey, rnum);	// 축소 치환

		for(i=0;i<28;i++){
			fstkey[i] = profstkey[i];
			seckey[i] = proseckey[i];
		}
	}
		//for(i=0;i<16;i++){
		//	for(j=0;j<48;j++){
		//		printf("%d",roundkey[i][j]);
		//	}
		//	printf("\n");
		//}
		//scanf("%c",&test);

}

void ParityDrop(int *key_b, int *key_parity){
	
	int i;

	//for(i=0;i<64;i++)
	//	printf("%d",key_b[i]);
	//printf("\n");

	for(i=0;i<56;i++)
		key_parity[i] = input_b[ParityDropTable[i]-1];

	//for(i=0;i<56;i++)
	//	printf("%d",key_parity[i]);

}

void ShiftLeft(int *preshiftkey, int *proshiftkey, int rnum){
	int i;

	//for(i=0;i<28;i++)
	//	printf("%d",preshiftkey[i]);
	//printf("\n");

	if(rnum==1 || rnum==2 || rnum==9 || rnum==16){
		for(i=0;i<28;i++){
			if(i==27)
				proshiftkey[i] = preshiftkey[0];
			else
				proshiftkey[i] = preshiftkey[i+1];	
		}
	}
	else{
		for(i=0;i<28;i++){
			if(i>=26)
				proshiftkey[i] = preshiftkey[i-27+1];	
			else
				proshiftkey[i] = preshiftkey[i+2];			
		}	
	}

	//for(i=0;i<28;i++)
	//	printf("%d",proshiftkey[i]);
	//printf("\n");
}

void CompressionPBox(int *profstkey, int *proseckey, int rnum){
	int temp[56];
	int i,j;

	for(i=0;i<28;i++){					//나누었던 부분을 먼저 56bit로 합친다.
		temp[i] = profstkey[i];
		temp[i+28] = proseckey[i];
	}

	for(i=0;i<48;i++)					//축소 치환 테이블 이용하여 축소 치환
		procomp[i] = temp[CompressionPBoxTable[i]-1];
	
	i=rnum-1;							//각 round에 따른
	for(j=0;j<48;j++){					//roundkey 배열에 저장
		roundkey[i][j] = procomp[j];	
	}	
	for(i=0;i<48;i++)
		printf("%d",procomp[i]);
	printf("\n");


}

void Round_Func(int *in_chgb, int roundnum, int boolnum){
	int i;
	int swap[32];

	for(i=0;i<32;i++){						//L,R로 32bit씩 나눔
		L[i] = in_chgb[i];
		R[i] = in_chgb[i+32];
	}
	Des_Func(in_chgb, roundnum);			//Des함수

	for(i=0;i<32;i++)
		L[i]=XOR_Func(L[i],des_fout[i]);

	if(boolnum==1)
		goto exit;

	for(i=0;i<32;i++){						//swapper
		swap[i] = L[i];
		L[i] = R[i];
		R[i] = swap[i];
	}
exit:;
}

void Des_Func(int *in_chgb, int roundnum){

	int i,temp[48];
	Exp_PBox(R);						//확장 P박스

	for(i=0;i<48;i++)
		temp[i]=XOR_Func(exp_R[i],roundkey[roundnum][i]);//키 XOR
	
	SBoxes(temp);//8개의 S박스
	
	Str_PBox(s_outb);//단순 P박스
}

void Exp_PBox(int *R){
	int temp[48];
	int i;

	for(i=0;i<48;i++)
		temp[i] = R[ExpansionPermutationTable[i]-1];

	for(i=0;i<48;i++)
		exp_R[i]=temp[i];
}

void SBoxes(int *temp){
	int row,column;
	int i,j,quot=0,rem=0;
	int s_in[8][6],s_outd;
	int temp2[8][4];

	for(i=0;i<8;i++){
		for(j=0;j<6;j++){
			s_in[i][j]=temp[i*6+j];
		}
	}

	for(i=0;i<8;i++){
		row = s_in[i][0]*2+s_in[i][5];
		column= s_in[i][1]*8+s_in[i][2]*4+s_in[i][3]*2+s_in[i][4];

		s_outd=S[i][row][column];

		for(j=3;j>=0;j--){		//dec값을 binary로 변환 후 저장
			quot = s_outd/2;
			rem = s_outd%2;
			temp2[i][j] = rem;
			s_outd = quot;
		}
	}
	for(i=0;i<8;i++)
		for(j=0;j<4;j++)
			s_outb[i*8+4]=temp2[i][j];
}

void Str_PBox(int *s_outb){
	int i;
	for(i=0;i<32;i++)				
		des_fout[i] = s_outb[StraightPermutationTable[i]-1];
}

void Final_permutation(int *in_b, int *output_b){
	
	int temp[64];
	int i;

	for(i=0;i<64;i++)
		temp[i] = in_b[FinalPermutationTable[i]-1];

	for(i=0;i<64;i++)
		output_b[i] = temp[i];

	//for(i=0;i<64;i++){
	//	printf("%d",input_chgb[i]);
	//	if(i!=0 && (i+1)%8==0)
	//		printf("\n");
	//}
	printf("\n");

} 

void Binarytochar(int *output_b,char *output_c){
	int i,j,int_val=0,quot=0,rem=0;
	int temp[64];

	printf("\n");
	for(i=0;i<8;i++){
		temp[i]=output_b[i*8+7]+output_b[i*8+6]*2+output_b[i*8+5]*4+output_b[i*8+4]*8+output_b[i*8+3]*16+output_b[i*8+2]*32+output_b[i*8+1]*64+output_b[i*8+0]*128;
	}

	for(i=0;i<8;i++){
		output_c[i] = (char)temp[i];
	//	printf("%c",output_c[i]);
	}
//	printf("\n");

}
int XOR_Func(int in1, int in2){
	if(in1==in2)
		return 0;
	else
		return 1;
}