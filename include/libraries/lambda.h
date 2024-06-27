#ifndef AC0CB76B_E98E_46C3_9476_4C399EAB344A
#define AC0CB76B_E98E_46C3_9476_4C399EAB344A

/*
// Example with lambda  (needs --std=gnu99)
#include <stdio.h>


float thelist[]={ 3.141, 6.02, 42.0, 0.7 };

#define lambda(lambda$_ret, lambda$_args, lambda$_body)\
  ({\
    lambda$_ret lambda$__anon$ lambda$_args\
      lambda$_body\
    &lambda$__anon$;\
  })


float average_apply(float (*fn)(float inp))
{
    int i,n=sizeof(thelist)/sizeof(thelist[0]);
    float avg=0.0;
    for (i=0;i<n;i++)
    {
        avg+=fn(thelist[i]);
        printf("Running sum=%f\n", avg);
    }

    return avg/n;
}

int main(int argc, char *argv[])
{
    printf("%f\n",average_apply(lambda(float,(float x),{ return 2*x; })));
    printf("%f\n",average_apply(lambda(float,(float x),{ return x/3.0; })));

    return 0;
};*/

#define lambda(lambda$_ret, lambda$_args, lambda$_body)                        \
  ({ lambda$_ret lambda$__anon$ lambda$_args lambda$_body &lambda$__anon$; })

/*                                                                             \
#define lambda(return_type, function_body)                                     \
  ({ return_type __fn__ function_body __fn__; })                               \
*/
#endif /* AC0CB76B_E98E_46C3_9476_4C399EAB344A */
