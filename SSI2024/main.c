#include <stdio.h>
#include <string.h>
#include "decrypt.h"

#define TYPEA_PARAMS \
"type a\n" \
"q 87807107996633125224377819847540498158068831994142082" \
"1102865339926647563088022295707862517942266222142315585" \
"8769582317459277713367317481324925129998224791\n" \
"h 12016012264891146079388821366740534204802954401251311" \
"822919615131047207289359704531102844802183906537786776\n" \
"r 730750818665451621361119245571504901405976559617\n" \
"exp2 159\n" \
"exp1 107\n" \
"sign1 1\n" \
"sign0 1\n"


int main() {
    //use the example in the article 
    char I1='1';  
    char I2='0';
    char I3='1';
    char I4='0';
    char I5='1';
    char I6='0';
    char I7='1';
    char I8='0';
    char user5[]={I1,I2,I5};
    char user7[]={I1,I3,I7};
    char user8[]={I1,I2,I4,I8};
    char Sv[]={I1,I2,I3,I4,I5,I7,I8};
    int user8_I_index[]={1,2,4,8};
    int user8_L_I_index[]={3,5,6,7};
    char Sv_others[]={I3,I5,I7};
    int index_Sv_others[]={3,5,7};
    int length_Sv_others=3;
    int user8_length_I_index=4;
    int user8_length_L_I_index=4;
    int length_Sv=7;
    int index_Sv[]={1,2,3,4,5,7,8};
    int d=4;
    int l=8;
    int n=7;

    pairing_t pairing;
    pairing_init_set_buf(pairing,TYPEA_PARAMS,strlen(TYPEA_PARAMS));

    //setup
    PP_MSK* pp_msk_ptr=NULL;
    setup(pairing,&pp_msk_ptr,d,l,n);
    //setup completed

    //keygen
    element_t* y=(element_t *)malloc((n+1)*sizeof(element_t));
    for(int i=1;i<=n;i++){  //skip index 0
        element_init_Zr(y[i],pairing);
        element_random(y[i]);
    }
    SK* sk_ptr=NULL;
    keygen(pp_msk_ptr,&sk_ptr,user8,y,user8_I_index,user8_L_I_index,pairing,user8_length_I_index,user8_length_L_I_index);
    //keygen for user8 completed

    //encrypt
    CT* ct_ptr=NULL;
    element_t* x=(element_t *)malloc((n+1)*sizeof(element_t));
    for(int i=1;i<=n;i++){
        element_init_Zr(x[i],pairing);
        element_random(x[i]);
    }
    encrypt(&ct_ptr,pp_msk_ptr->pp_ptr,Sv,x,length_Sv,index_Sv,pairing);
    //encrypt completed

    //decrypt
    W* w_ptr=NULL;
    decrypt(&w_ptr,pp_msk_ptr->pp_ptr,sk_ptr,y,ct_ptr,pairing,Sv_others,index_Sv_others,length_Sv_others);
    //decrypt completed

    //test wrong or right
    element_t x_y;
    element_init_Zr(x_y,pairing);
    element_set0(x_y);
    element_t temp;
    element_init_Zr(temp,pairing);
    for(int i=1;i<=n;i++){
        element_mul(temp,x[i],y[i]);
        element_add(x_y,x_y,temp);
    }
    element_t tp;
    element_init_GT(tp,pairing);
    pairing_apply(tp,pp_msk_ptr->pp_ptr->g,pp_msk_ptr->pp_ptr->g2,pairing);
    element_pow_zn(tp,tp,x_y);
    if(element_cmp(tp,w_ptr->w)==0){
        printf("bingo!");
    }else{
        printf("no!!!!");
    }
}
