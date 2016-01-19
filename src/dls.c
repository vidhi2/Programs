/*
 *		Implementation of a distributed linear search
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int arr[100];

int mainID;
int chID;

void search(int num,int i,int j){
	int k,ind1,ind2,status;
	pid_t ch1,ch2;
	if(j-i<5){
		for(k=i;k<=j;k++){
			if(arr[k]==num){
				exit(k);
			}
		}
		exit(-1);
	}
	int mid=(i+j)/2;
	ch1=fork();
	if(ch1){
		ch2=fork();
		if(ch2){
			//parent
			//waitpid
			waitpid(ch1,&status,0);
			ind1=WEXITSTATUS(status);
			waitpid(ch2,&status,0);
			ind2=WEXITSTATUS(status);
			if(ind1!=-1 && ind1!=255){
				if(getpid()==mainID)printf("number is at index %d\n",ind1);
				exit(ind1);
			}
			else if(ind2!=-1 && ind2!=255){
				if(getpid()==mainID)printf("number is at index %d\n",ind2);
				exit(ind2);
			}else{
				if(getpid()==mainID)printf("number not present\n");
			}
		}else{
			//child2

			setpgid(0, mainID+1);
			search(num,mid+1,j);
		}

	}else{
		//child1
		setpgid(0, mainID+1);
		search(num,i,mid);
	}
	exit(-1);
}



void main() {
	int i=0,n,num;
	FILE *fp;
	char path[100];
	printf("Enter path >");
	scanf("%s",path);
	fp = fopen(path, "r");
	printf("Enter number = ");
	scanf("%d",&num);
	while(fscanf(fp, "%d", &arr[i])!=EOF){
		i++;
	}
	n=i;
	mainID=getpid();
	search(num,0,n-1);
	fclose(fp);
	kill(-(mainID+1),SIGTERM);
	exit(-1);
}   
