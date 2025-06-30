// used to match the indices since I chose 0-index instead of 1-index in CYK algorithm
// lesson learnt: stick to the design, dont try to be smart in unneccessary parts.

#include <iostream>
using namespace std;
const int N = 10;

int main(){

int arr1[N][N]={};
int arr2[N][N]={};


int count1 = 0, count2=0;
for (int l = 1; l<N;l++){
      for (int s = 0; s<= N-l-1;s++){
        for (int p = 0; p<=l-1;p++){
          count1++;
          arr1[l-p-1][s+p+1]++;
        }}}



for (int l = 2; l<=N;l++){
      for (int s = 1; s<= N-l+1;s++){
        for (int p = 1; p<=l-1;p++){
          count2++;
          arr2[l-p][s+p]++;
        }}}
      
        cout<<count1<<" "<<count2<<endl;

for (int i=0;i<N;i++){
  for (int j=0;j<N;j++){
    cout<<arr1[i][j]<<" ";
  }
  cout<<endl;
}

cout<<"\n\n\n";

for (int i=0;i<N;i++){
  for (int j=0;j<N;j++){
    cout<<arr2[i][j]<<" ";
  }
  cout<<endl;
}

      }


