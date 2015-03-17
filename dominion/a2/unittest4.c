#include <stdio.h>


int* kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
		  int k8, int k9, int k10) {
  int* k = malloc(10 * sizeof(int));
  k[0] = k1;
  k[1] = k2;
  k[2] = k3;
  k[3] = k4;
  k[4] = k5;
  k[5] = k6;
  k[6] = k7;
  k[7] = k8;
  k[8] = k9;
  k[9] = k10;
  return k;
}

int test_kingdom_cards(void)
{
    int k0 = 0;
    int k1 = 1;
    int k2 = 2;
    int k3 = 3;
    int k4 = 4;
    int k5 = 5;
    int k6 = 6;
    int k7 = 7;
    int k8 = 8;
    int k9 = 9;
    
    int *output = kingdomCards(k0, k1, k2, k3, k4, k5, k6, k7, k8, k9);
    if(!output)
        return 0;

    if(output[0] != k0)
        return 0;
    if(output[1] != k1)
        return 0;
    if(output[2] != k2)
        return 0;
    if(output[3] != k3)
        return 0;
    if(output[4] != k4)
        return 0;
    if(output[5] != k5)
        return 0;
    if(output[6] != k6)
        return 0;
    if(output[7] != k7)
        return 0;
    if(output[8] != k8)
        return 0;
    if(output[9] != k9)
        return 0;
    
    free(output);

    return 1;
}

int main(int argc, char **argv)
{
    if(!test_kingdom_cards())
    {
        printf("error...\n");
        return -1;
    }

    printf("success...\n");
    return 0;
}
