/*numPass=1, numTotal=6
Verdict:WRONG_ANSWER, Visibility:1, Input:"4", ExpOutput:"1 1 1 1 
2 1 1 
2 2 
3 1 
4 
", Output:"1111
211
22
31
4
"
Verdict:WRONG_ANSWER, Visibility:1, Input:"5", ExpOutput:"1 1 1 1 1 
2 1 1 1 
2 2 1 
3 1 1 
3 2 
4 1 
5 
", Output:"11111
2111
221
311
32
41
5
"
Verdict:WRONG_ANSWER, Visibility:1, Input:"2", ExpOutput:"1 1 
2 
", Output:"11
2
"
Verdict:ACCEPTED, Visibility:1, Input:"1", ExpOutput:"1 
", Output:"1
"
Verdict:WRONG_ANSWER, Visibility:0, Input:"6", ExpOutput:"1 1 1 1 1 1 
2 1 1 1 1 
2 2 1 1 
2 2 2 
3 1 1 1 
3 2 1 
3 3 
4 1 1 
4 2 
5 1 
6 
", Output:"111111
21111
2211
222
3111
321
33
411
42
51
6
"
Verdict:WRONG_ANSWER, Visibility:0, Input:"9", ExpOutput:"1 1 1 1 1 1 1 1 1 
2 1 1 1 1 1 1 1 
2 2 1 1 1 1 1 
2 2 2 1 1 1 
2 2 2 2 1 
3 1 1 1 1 1 1 
3 2 1 1 1 1 
3 2 2 1 1 
3 2 2 2 
3 3 1 1 1 
3 3 2 1 
3 3 3 
4 1 1 1 1 1 
4 2 1 1 1 
4 2 2 1 
4 3 1 1 
4 3 2 
4 4 1 
5 1 1 1 1 
5 2 1 1 
5 2 2 
5 3 1 
5 4 
6 1 1 1 
6 2 1 
6 3 
7 1 1 
7 2 
8 1 
9 
", Output:"111111111
21111111
2211111
222111
22221
3111111
321111
32211
3222
33111
3321
333
411111
42111
4221
4311
432
441
51111
5211
522
531
54
6111
621
63
711
72
81
9
"
*/
#include <stdio.h>


void check_seq(int num,int array[],int sum,int check)
{ if(sum==num)
   {
       for(int i=0;i<check;i++)
       {
           printf("%d",array[i]);
       }
       printf("\n");
       return;
   }
    int fing=1;
    while(fing<=num-sum&&(check==0||fing<=array[check-1]))
    {
        array[check]=fing;
        check_seq(num,array,sum+fing,check+1);
        fing++;
    }
}

void print_seq(int num)
{
    int *array;
    array=(int*)malloc(num*sizeof(int));
    check_seq(num,array,0,0);
    
    
}
     
    

int main(){
    int n;
   scanf("%d",&n);
   print_seq(n);
	return 0;
}