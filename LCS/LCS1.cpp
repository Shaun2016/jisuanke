#include <iostream>
#include <string.h>
#include <set>
using namespace std;
int num = 0;	// 记录遍历路径数
int** getArr(int,int);		// 动态创建二维数组
void LCS(int**,int**,string,string,int,int);	// 给 c,b 数组赋值
void TraceBack(string,string,int**,int,int);    // 回溯路径
set<string> setOfLCS;    // set保存所有的LCS，set集合中元素不重复
int main() {
    string s1,s2;
    cin>>s1>>s2;
    int len1 = s1.size() + 1;
    int len2 = s2.size() + 1;
    int** c = getArr(len1,len2);
    int** b = getArr(len1,len2);
  	// 使 s1,s2 与 c,b 坐标一致
    s1 = " " + s1;	
    s2 = " " + s2;
  	// 自底向上给 c,b 数组赋值
    LCS(c,b,s1,s2,len1,len2);
  	// 显示 c,b 数组
    for(int i=0;i<len1;i++) {
        for(int j=0;j<len2;j++) {
            cout<<c[i][j]<<" ";
            if(j == len2-1)
                cout<<endl;
        }
    }
    cout<<"===============================\n";
    for(int i=0;i<len1;i++) {
        for(int j=0;j<len2;j++) {
            cout<<b[i][j]<<" ";
            if(j == len2-1)
                cout<<endl;
        }
    }
  	// 从右下角根据 b 数组指向回溯到边界 
    TraceBack(s1,"",b,len1-1,len2-1);
    cout<<"All route number: "<<num<<endl;
    cout<<"Remove duplicate elements: "<<endl;
  	// 遍历set，输出所有LCS
    set<string>::iterator iter=setOfLCS.begin();
    while(iter != setOfLCS.end()) {
        cout<<*iter<<endl;
        iter++;
    }
    cout<<"LCS length: "<<c[len1-1][len2-1]<<endl;
    cout<<"LCS number: "<<setOfLCS.size()<<endl;
    return 0;
}
void LCS(int** c,int** b,string s1,string s2,int m,int n) {
    for(int i=1;i<m;i++) {
        for(int j=1;j<n;j++) {
            if(s1[i] == s2[j]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 1;	// 斜上
            } else if(c[i-1][j] > c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = 2;	// 上
            } else if(c[i-1][j] < c[i][j-1]) {
                c[i][j] = c[i][j-1];
                b[i][j] = 3;	// 左
            } else {
                c[i][j] = c[i-1][j];
                b[i][j] = 4;	// 上 或 左
            }
        }
    }
}
// 返回动态创建的二维数组的指针
int** getArr(int M,int N) {
    int** arr = new int*[M];
    for(int i=0;i<M;i++) {
        arr[i] = new int[N];
        memset(arr[i],0,sizeof(int)*N);
    }
    return arr;
}
void TraceBack(string s1,string temp,int** b,int i,int j) {
    while(i>0 && j>0) {
        if(b[i][j] == 1) {
            temp = s1[i] + temp;
            i--;
            j--;
        } else if(b[i][j] == 2) {
            i--;
        } else if(b[i][j] == 3) {
            j--;
        } else {
          	// 递归回溯
            TraceBack(s1,temp,b,i-1,j);
            TraceBack(s1,temp,b,i,j-1);
            return;
        }
    }
    num ++;
  	// 打印一次遍历得到的字符串，可能是重复的
    cout<<"###"<<temp<<endl;
  	// 交给 set 来移除重复字符串
    setOfLCS.insert(temp);
}
/*
Input:
badcdcba
abcdcdab
*/