/*numPass=9, numTotal=9
Verdict:ACCEPTED, Visibility:1, Input:"abcdef", ExpOutput:"bdfhjl", Output:"bdfhjl"
Verdict:ACCEPTED, Visibility:1, Input:"wxyz", ExpOutput:"tvxz", Output:"tvxz"
Verdict:ACCEPTED, Visibility:1, Input:"zyxw", ExpOutput:"zxvt", Output:"zxvt"
Verdict:ACCEPTED, Visibility:1, Input:"t", ExpOutput:"n", Output:"n"
Verdict:ACCEPTED, Visibility:1, Input:"lly", ExpOutput:"xxx", Output:"xxx"
Verdict:ACCEPTED, Visibility:0, Input:"qwerty", ExpOutput:"htjjnx", Output:"htjjnx"
Verdict:ACCEPTED, Visibility:0, Input:"manuallyaddedtestcases", ExpOutput:"zbbpbxxxbhhjhnjlnfbljl", Output:"zbbpbxxxbhhjhnjlnfbljl"
Verdict:ACCEPTED, Visibility:0, Input:"yllyl", ExpOutput:"xxxxx", Output:"xxxxx"
Verdict:ACCEPTED, Visibility:0, Input:"visibility", ExpOutput:"rrlrdrxrnx", Output:"rrlrdrxrnx"
*/
#include <stdio.h>

int main() {
	char str[100],temp;
	scanf("%s",str);
	int p;
	 while(str[p]!='\0')//finding the length of str
    p++;
	int i;
	for(i=0;i<p;i++){
	    temp=2*str[i]-96;
	  if(2*str[i]-96>122){
	    str[i]=2*str[i]-122;
	}
	else
	{
	    str[i]=temp;
	}
	} 
	printf("%s",str);
	
	return 0;
}