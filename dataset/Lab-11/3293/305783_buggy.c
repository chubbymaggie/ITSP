/*numPass=0, numTotal=6
Verdict:WRONG_ANSWER, Visibility:1, Input:"4
0 1 0 1
1 0 1 0
0 1 0 1
1 0 1 0 
2", ExpOutput:"1 2 1 2 ", Output:""
Verdict:WRONG_ANSWER, Visibility:1, Input:"4
0 1 1 1
1 0 1 0
1 1 0 1
1 0 1 0 
3", ExpOutput:"1 2 3 2 ", Output:""
Verdict:WRONG_ANSWER, Visibility:1, Input:"4
0 1 1 1
1 0 1 0
1 1 0 1
1 0 1 0
3", ExpOutput:"1 2 3 2 ", Output:""
Verdict:WRONG_ANSWER, Visibility:0, Input:"2
0 1
1 0
2", ExpOutput:"1 2 ", Output:""
Verdict:WRONG_ANSWER, Visibility:0, Input:"1
0
2", ExpOutput:"1 ", Output:""
Verdict:WRONG_ANSWER, Visibility:0, Input:"4 
0 1 1 1 
1 0 1 1 
1 1 0 1 
1 1 1 0
1000", ExpOutput:"1 2 3 4 ", Output:""
*/
#include <stdio.h>

int main(){
    int n,a,b;
    scanf("%d ",&n);
    int** ar=(int**)malloc(n*sizeof(int*));
    for(a=0;a<n;a++)
     *(ar+a)=(int*)malloc(n*sizeof(int));
    
    for(a=0;a<n;a++)
    {
        for(b=0;b<n;b++)
         scanf("%d ",(*(ar+a)+b));
    }
    /*for(a=0;a<n;a++)
    {
        for(b=0;b<n;b++)
         printf("%d ",*(*(ar+a)+b));
         printf("\n");
    }*/
	return 0;
}