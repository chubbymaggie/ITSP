/*numPass=4, numTotal=4
Verdict:ACCEPTED, Visibility:1, Input:"12
Hello World", ExpOutput:"dlroW olleH

", Output:"dlroW olleH
"
Verdict:ACCEPTED, Visibility:1, Input:"14
baL noisruceR", ExpOutput:"Recursion Lab

", Output:"Recursion Lab
"
Verdict:ACCEPTED, Visibility:0, Input:"44
The quick brown fox jumps over the lazy dog", ExpOutput:"god yzal eht revo spmuj xof nworb kciuq ehT

", Output:"god yzal eht revo spmuj xof nworb kciuq ehT
"
Verdict:ACCEPTED, Visibility:0, Input:"65
esuoh sybstaG rof yaw edam dah taht seert eht seert dehsinav stI", ExpOutput:"Its vanished trees the trees that had made way for Gatsbys house

", Output:"Its vanished trees the trees that had made way for Gatsbys house
"
*/
#include <stdio.h>
#include <string.h>
void reverse_string(char str1[],char str2[],int k,int i)
{
    if(k<0)
    {
        str2[i]='\0';
        return;
    }
    str2[i]=str1[k];
    reverse_string(str1,str2,k-1,i+1);
}
int main()
{
    int n;
    scanf("%d",&n);
    char str1[n],str2[n];
    for(int i=0;i<n;i++)
    {
        scanf("%c",&str1[i]);
    }

    reverse_string(str1,str2,n-1,0);
    for(int i=0;i<n;i++)
    {
        printf("%c",str2[i]);
    }

    return 0;
}