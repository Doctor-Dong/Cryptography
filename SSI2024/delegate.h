#include "decrypt.h"

void delegate(PP* pp_ptr,SK** sk_ptr_new,SK* sk_ptr_old,char Ij,int index_j,pairing_t pairing,char* ID,int* index_ID,int length_ID,int* index_L_I,int length_L_I){
    *sk_ptr_new=(SK *)malloc(sizeof(SK));
    element_t r;
    element_init_Zr(r,pairing);
    element_random(r);

    //K1
    element_init_G2((*sk_ptr_new)->K1,pairing);
    element_t hashvalue;
    element_init_Zr(hashvalue,pairing);
    element_from_hash(hashvalue,&Ij,sizeof(Ij));
    int i;
    for(i=0;i<sk_ptr_old->Ki_length;i++){
        if((sk_ptr_old->Ki_index)[i]==index_j){
            break;
        }
    }
    element_pow_zn((*sk_ptr_new)->K1,(sk_ptr_old->Ki)[i],hashvalue);
    element_mul((*sk_ptr_new)->K1,(*sk_ptr_new)->K1,sk_ptr_old->K1);
    element_t temp;
    element_init_G2(temp,pairing);
    element_set(temp,pp_ptr->g3);
    element_t tp1;
    element_init_G2(tp1,pairing);
    for(i=1;i<length_ID;i++){    //index 0 is I1
        element_from_hash(hashvalue,&ID[i],sizeof(ID[i]));
        element_pow_zn(tp1,(pp_ptr->ui)[index_ID[i]],hashvalue);
        element_mul(temp,temp,tp1);
    }
    element_pow_zn(temp,temp,r);
    element_mul((*sk_ptr_new)->K1,(*sk_ptr_new)->K1,temp);
    element_clear(hashvalue);
    element_clear(temp);
    element_clear(tp1);
    //K1 completed

    //K2
    element_init_G1((*sk_ptr_new)->K2,pairing);
    element_set((*sk_ptr_new)->K2,sk_ptr_old->K2);
    element_t hashvalue2;
    element_init_Zr(hashvalue2,pairing);
    element_from_hash(hashvalue2,&ID[0],sizeof(ID[0]));
    element_t tp2;
    element_init_G1(tp2,pairing);
    element_pow_zn(tp2,pp_ptr->g,hashvalue2);
    element_mul(tp2,tp2,pp_ptr->g1);
    element_pow_zn(tp2,tp2,r);
    element_mul((*sk_ptr_new)->K2,(*sk_ptr_new)->K2,tp2);
    element_clear(hashvalue2);
    element_clear(tp2);
    //K2 completed

    //Ki
    (*sk_ptr_new)->Ki_length=length_L_I;
    (*sk_ptr_new)->Ki=(element_t *)malloc(length_L_I*sizeof(element_t));
    (*sk_ptr_new)->Ki_index=(int *)malloc(length_L_I*sizeof(int));
    int j;
    for(i=0;i<length_L_I;i++){
        element_init_G2(((*sk_ptr_new)->Ki)[i],pairing);
        for(j=0;j<(sk_ptr_old->Ki_length);j++){
            if((sk_ptr_old->Ki_index)[j]==index_L_I[i]){
                break;
            }
        }
        ((*sk_ptr_new)->Ki_index)[i]=index_L_I[i];
        element_pow_zn(((*sk_ptr_new)->Ki)[i],(pp_ptr->ui)[index_L_I[i]],r);
        element_mul(((*sk_ptr_new)->Ki)[i],((*sk_ptr_new)->Ki)[i],(sk_ptr_old->Ki)[j]);
    }
    //Ki completed
}