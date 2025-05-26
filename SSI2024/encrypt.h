#include "keygen.h"

typedef struct ct{
    element_t C1;
    element_t C2;
    element_t* Cxi;
}CT;

void encrypt(CT** ct_ptr,PP* pp_ptr,char* Sv,element_t* x,int length_Sv,int* index_Sv,pairing_t pairing){
    *ct_ptr=(CT *)malloc(sizeof(CT));

    element_t s;
    element_init_Zr(s,pairing);
    do{
        element_random(s);
    }while(element_is0(s));     //s

    //C1
    element_init_G1((*ct_ptr)->C1,pairing);
    element_t hashvalue;
    element_init_Zr(hashvalue,pairing);
    element_from_hash(hashvalue,&Sv[0],sizeof(Sv[0]));
    element_mul(hashvalue,hashvalue,s);
    element_pow_zn((*ct_ptr)->C1,pp_ptr->g,hashvalue);
    element_t temp;
    element_init_G1(temp,pairing);
    element_pow_zn(temp,pp_ptr->g1,s);
    element_mul((*ct_ptr)->C1,(*ct_ptr)->C1,temp);
    element_clear(temp);
    //C1 completed
    
    //C2
    element_init_G2((*ct_ptr)->C2,pairing);
    element_set((*ct_ptr)->C2,pp_ptr->g3);
    element_t tp1;
    element_init_G2(tp1,pairing);
    for(int i=1;i<length_Sv;i++){
        element_from_hash(hashvalue,&Sv[i],sizeof(Sv[i]));
        element_pow_zn(tp1,(pp_ptr->ui)[index_Sv[i]],hashvalue);
        element_mul((*ct_ptr)->C2,(*ct_ptr)->C2,tp1);
    }
    element_pow_zn((*ct_ptr)->C2,(*ct_ptr)->C2,s);
    element_clear(tp1);
    element_clear(hashvalue);
    //C2 completed

    //Cxi
    (*ct_ptr)->Cxi=(element_t *)malloc(((pp_ptr->n)+1)*sizeof(element_t));   //skip index 0
    element_t tp2;
    element_t tp3;
    element_init_GT(tp2,pairing);
    element_init_GT(tp3,pairing);
    for(int i=1;i<=(pp_ptr->n);i++){
        element_init_GT(((*ct_ptr)->Cxi)[i],pairing);
        pairing_apply(tp2,pp_ptr->g,pp_ptr->g2,pairing);
        element_pow_zn(tp2,tp2,x[i]);
        pairing_apply(tp3,pp_ptr->g1,(pp_ptr->g2_betai)[i],pairing);
        element_pow_zn(tp3,tp3,s);
        element_mul(((*ct_ptr)->Cxi)[i],tp2,tp3);
    }
    element_clear(tp2);
    element_clear(tp3);
    //Cxi completed

    element_clear(s);
}