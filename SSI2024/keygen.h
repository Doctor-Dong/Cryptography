#include "setup.h"

typedef struct sk{
    element_t K1;
    element_t K2;
    element_t* Ki;
    int* Ki_index;
    int Ki_length;
}SK;

void keygen(PP_MSK* pp_msk_ptr,SK** sk_ptr,char* ID,element_t* y,int* I_index,int* L_I_index,pairing_t pairing,int length_I_index,int length_L_I_index){    //two index arrays for convenience
    element_t r;
    element_init_Zr(r,pairing);
    do{
        element_random(r);
    }while(element_is0(r));       //r

    *sk_ptr=(SK *)malloc(sizeof(SK));

    //K1
    element_init_G2((*sk_ptr)->K1,pairing);
    element_t beta_y,tp1;
    element_init_Zr(beta_y,pairing);
    element_init_Zr(tp1,pairing);
    element_set0(beta_y);
    for(int i=1;i<=pp_msk_ptr->pp_ptr->n;i++){
        element_mul(tp1,y[i],(pp_msk_ptr->msk_ptr->betai)[i]);
        element_add(beta_y,beta_y,tp1);
    }
    element_clear(tp1);
    element_mul(beta_y,beta_y,pp_msk_ptr->msk_ptr->alpha);    //alpha * <beta,y>
    element_t hashvalue;
    element_init_Zr(hashvalue,pairing);
    element_from_hash(hashvalue,&ID[0],sizeof(ID[0]));   //H(I1)
    element_add(hashvalue,hashvalue,pp_msk_ptr->msk_ptr->alpha);
    element_div(beta_y,beta_y,hashvalue);    //exponent
    element_pow_zn((*sk_ptr)->K1,pp_msk_ptr->pp_ptr->g2,beta_y);
    element_clear(beta_y);

    element_t temp;
    element_init_G2(temp,pairing);
    element_set(temp,pp_msk_ptr->pp_ptr->g3);
    element_t tp2;
    element_init_G2(tp2,pairing);
    for(int i=1;i<length_I_index;i++){   //skip I1,which index=0
        element_from_hash(hashvalue,&ID[i],sizeof(ID[i]));
        element_pow_zn(tp2,(pp_msk_ptr->pp_ptr->ui)[I_index[i]],hashvalue);
        element_mul(temp,temp,tp2);
    }
    element_clear(tp2);
    element_pow_zn(temp,temp,r);
    element_mul((*sk_ptr)->K1,(*sk_ptr)->K1,temp);
    element_clear(temp);
    element_clear(hashvalue);
    //K1 completed

    //K2
    element_t tp3;
    element_init_Zr(tp3,pairing);
    element_from_hash(tp3,&ID[0],sizeof(ID[0]));   //H(I1)
    element_add(tp3,tp3,pp_msk_ptr->msk_ptr->alpha);
    element_mul(tp3,tp3,r);
    element_init_G1((*sk_ptr)->K2,pairing);
    element_pow_zn((*sk_ptr)->K2,pp_msk_ptr->pp_ptr->g,tp3);
    element_clear(tp3);
    //K2 completed

    //Ki
    (*sk_ptr)->Ki_length=length_L_I_index;
    (*sk_ptr)->Ki_index=(int *)malloc(length_L_I_index*sizeof(int));
    (*sk_ptr)->Ki=(element_t *)malloc(length_L_I_index*sizeof(element_t));
    for(int i=0;i<length_L_I_index;i++){
        ((*sk_ptr)->Ki_index)[i]=L_I_index[i];
        element_init_G2(((*sk_ptr)->Ki)[i],pairing);
        element_pow_zn(((*sk_ptr)->Ki)[i],(pp_msk_ptr->pp_ptr->ui)[L_I_index[i]],r);
    }
    //Ki completed

    element_clear(r);
}

