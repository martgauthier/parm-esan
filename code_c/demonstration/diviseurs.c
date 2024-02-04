#include <parm.h>
#include <utils.h>
#include <math.h>

//calculateur de diviseurs pour un nombre saisi sur clavier
void run()
{
    BEGIN();
    int a;
    PUTCHAR('A','=');
    a = READINT();
    PUTCHAR('\n');
    PUTCHAR('l','i','s','t','e',' ','d','e','s',' ','d','i','v','i','s','e','u','r','s',':');
    for(int k=1; k<=(a+1)/2; k++){
        if(MOD(a,k) == 0){
            RES = k;
            PRINTRES_SIGN();
            PUTCHAR(',');
        }
    }

    PUTCHAR('\n','s','t','o','p');

    WAITKEY();

    RESET();
    END();
}