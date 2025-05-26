#include <pbc/pbc.h>
#include <stdlib.h>

typedef struct pp{
    element_t g;
    element_t g1;
    element_t g2;
    element_t g3;
    element_t* ui;
    element_t* g2_betai;
    element_t v;
    element_t v_;   //v'
    int d;   //unknown 
    int l;
    int n;
}PP;

typedef struct msk{
    element_t alpha;
    element_t* betai;
}MSK;

typedef struct pp_msk{
    PP* pp_ptr;
    MSK* msk_ptr;
}PP_MSK;

void setup(pairing_t pairing,PP_MSK** pp_msk_ptr,int d,int l,int n){
    *pp_msk_ptr=(PP_MSK *)malloc(sizeof(PP_MSK));
    (*pp_msk_ptr)->pp_ptr=(PP *)malloc(sizeof(PP));
    (*pp_msk_ptr)->msk_ptr=(MSK *)malloc(sizeof(MSK));
    //msk
    element_init_Zr((*pp_msk_ptr)->msk_ptr->alpha,pairing);
    do{
        element_random((*pp_msk_ptr)->msk_ptr->alpha);
    }while(element_is0((*pp_msk_ptr)->msk_ptr->alpha));   //alpha
    (*pp_msk_ptr)->msk_ptr->betai=(element_t *)malloc((n+1)*sizeof(element_t));  //avoid index 0
    for(int i=1;i<=n;i++){
        element_init_Zr(((*pp_msk_ptr)->msk_ptr->betai)[i],pairing);
        do{
            element_random(((*pp_msk_ptr)->msk_ptr->betai)[i]);
        }while(element_is0(((*pp_msk_ptr)->msk_ptr->betai)[i]));    //betai
    }
    //msk completed

    //pp
    element_init_G1((*pp_msk_ptr)->pp_ptr->g,pairing);
    element_random((*pp_msk_ptr)->pp_ptr->g);    //g
    element_init_G1((*pp_msk_ptr)->pp_ptr->g1,pairing);
    element_pow_zn((*pp_msk_ptr)->pp_ptr->g1,(*pp_msk_ptr)->pp_ptr->g,(*pp_msk_ptr)->msk_ptr->alpha);  //g1
    element_init_G2((*pp_msk_ptr)->pp_ptr->g2,pairing);
    element_random((*pp_msk_ptr)->pp_ptr->g2);   //g2
    element_init_G2((*pp_msk_ptr)->pp_ptr->g3,pairing);
    element_random((*pp_msk_ptr)->pp_ptr->g3);   //g3
    (*pp_msk_ptr)->pp_ptr->ui=(element_t *)malloc((l+1)*sizeof(element_t));    //avoid index 0,1
    for(int i=2;i<=l;i++){
        element_init_G2(((*pp_msk_ptr)->pp_ptr->ui)[i],pairing);
        element_random(((*pp_msk_ptr)->pp_ptr->ui)[i]);    //ui
    }
    (*pp_msk_ptr)->pp_ptr->g2_betai=(element_t *)malloc((n+1)*sizeof(element_t));   //avoid index 0
    for(int i=1;i<=n;i++){
        element_init_G2(((*pp_msk_ptr)->pp_ptr->g2_betai)[i],pairing);
        element_pow_zn(((*pp_msk_ptr)->pp_ptr->g2_betai)[i],(*pp_msk_ptr)->pp_ptr->g2,((*pp_msk_ptr)->msk_ptr->betai)[i]);   //g2_betai
    }
    element_init_GT((*pp_msk_ptr)->pp_ptr->v,pairing);
    pairing_apply((*pp_msk_ptr)->pp_ptr->v,(*pp_msk_ptr)->pp_ptr->g1,(*pp_msk_ptr)->pp_ptr->g2,pairing);    //v
    element_init_GT((*pp_msk_ptr)->pp_ptr->v_,pairing);
    pairing_apply((*pp_msk_ptr)->pp_ptr->v_,(*pp_msk_ptr)->pp_ptr->g,(*pp_msk_ptr)->pp_ptr->g2,pairing);    //v'
    (*pp_msk_ptr)->pp_ptr->d=d;
    (*pp_msk_ptr)->pp_ptr->l=l;
    (*pp_msk_ptr)->pp_ptr->n=n;
}