#include <bits/stdc++.h>
using namespace std;
#define int long long

void selectionSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i< n-1;i++){
        int minIndex = i;
        for( int j = i + 1; j<n;j++){
            if(arr[j]<minIndex){
                minIndex = arr[j];
            }
        }
        swap(arr[i], arr[minIndex]);
    }

}

signed main(){
    vector<int> arr ={};
    selectionSort(arr);
    cout << "Arreglo Ordenado Por selectiion sort:"<< endl;
    for(int i : arr){
        cout<<i<<" ";
    }
    cout<< endl;

    return 0;
}
