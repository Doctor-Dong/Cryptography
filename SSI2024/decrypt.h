#include "encrypt.h"

typedef struct enc{
    element_t w;
}W;

void decrypt(W** w_ptr,PP* pp_ptr,SK* sk_ptr,element_t* y,CT* ct_ptr,pairing_t pairing,char* Sv_others,int* index_Sv_others,int length_Sv_others){
    *w_ptr=(W *)malloc(sizeof(W));
    element_init_GT((*w_ptr)->w,pairing);

    //W
    element_t upper;
    element_init_GT(upper,pairing);
    pairing_apply(upper,sk_ptr->K2,ct_ptr->C2,pairing);
    element_t tp1;
    element_init_GT(tp1,pairing);
    for(int i=1;i<=(pp_ptr->n);i++){
        element_pow_zn(tp1,(ct_ptr->Cxi)[i],y[i]);
        element_mul(upper,upper,tp1);
    }
    element_clear(tp1);     
    //upper completed

    element_t lower;
    element_init_GT(lower,pairing);
    element_t tp2;
    element_init_G2(tp2,pairing);
    element_set(tp2,sk_ptr->K1);
    element_t tp3;
    element_init_G2(tp3,pairing);
    element_t hashvalue;
    element_init_Zr(hashvalue,pairing);
    int j;
    for(int i=0;i<length_Sv_others;i++){
        for(j=0;j<sk_ptr->Ki_length;j++){
            if((sk_ptr->Ki_index)[j]==index_Sv_others[i]){
                break;
            }
        }
        // j is the index for related Ki
        element_from_hash(hashvalue,&Sv_others[i],sizeof(Sv_others[i]));
        element_pow_zn(tp3,(sk_ptr->Ki)[j],hashvalue);
        element_mul(tp2,tp2,tp3);
    }
    pairing_apply(lower,ct_ptr->C1,tp2,pairing);
    element_clear(tp2);
    element_clear(tp3);
    element_clear(hashvalue);
    //lower completed

    element_div((*w_ptr)->w,upper,lower);
    //W completed

    element_clear(upper);
    element_clear(lower);
}