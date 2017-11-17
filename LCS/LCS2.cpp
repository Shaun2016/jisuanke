#include <iostream>
#include <string.h>
#include <stack>
using namespace std;
int** getArr(int,int);
void LCS(int**,int**,string,string,int,int);
void TraceBack(int,int,int**,int**,string);
struct node {
    int x;
    int y;
    int lcsLen;
    node(){}
    node(int x,int y,int z):x(x),y(y),lcsLen(z){}
    node(int x,int y,int** c):x(x),y(y),lcsLen(c[x][y]){}
    bool operator ==(const node &n) {
        if(this->x == n.x && this->y == n.y)
            return true;
        return false;
    }
};
void printStack(stack<node>,string);
void popPrint(stack<node>&,int);
node findJump(int,int,int**,int**,int);
void searchMatrix(node,node,int**,int**,stack<node>&);
int main()
{
    string s1,s2;
    cout<<"Input the first Sequence:";
    cin>>s1;
    cout<<"Input the second Sequence:";
    cin>>s2;
    int len1 = s1.length()+1;
    int len2 = s2.length()+1;
    int** c = getArr(len1,len2);
    int** b = getArr(len1,len2);
    s1 = " " + s1;
    s2 = " " + s2;
    LCS(c,b,s1,s2,len1,len2);
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
    if(c[len1-1][len2-1] == 0) {
        cout<<"0 0"<<endl;
        return 0;
    }
    TraceBack(len1,len2,c,b,s1);
    return 0;
}
int** getArr(int M,int N) {
    int** arr = new int*[M];
    for(int i=0;i<M;i++) {
        arr[i] = new int[N];
        memset(arr[i],0,sizeof(int)*N);
    }
    return arr;
}
void LCS(int** c,int** b,string s1,string s2,int m,int n) {
    for(int i=1;i<m;i++) {
        for(int j=1;j<n;j++) {
            if(s1[i] == s2[j]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 1;
            } else if(c[i-1][j] > c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = 2;
            } else if(c[i-1][j] < c[i][j-1]) {
                c[i][j] = c[i][j-1];
                b[i][j] = 3;
            } else {
                c[i][j] = c[i-1][j];
                b[i][j] = 4;
            }
        }
    }
}
void TraceBack(int m,int n,int** c,int** b,string str) {
    stack<node> store, print;
    node virtualNode = node(m,n,c[m-1][n-1]+1);	//由虚拟结点引发循环
    store.push(virtualNode);
    node storeTop;
    int topLen,number=0;
    while(!store.empty()) {
        storeTop = store.top();
        store.pop();
        print.push(storeTop);
        // 判断弹出的元素是否是边界元素
        if(storeTop.x == 1 || storeTop.y == 1 || print.size() == c[m-1][n-1]+1) {
            number++;
            printStack(print,str);		// 是边界将print栈除虚拟结点的所有点打印
            cout<<endl;					// 数量太多不要输出这两行了
            if(!store.empty()) {
                topLen = store.top().lcsLen;	
                popPrint(print,topLen);		// 弹出小于等于store栈顶长度的print栈中的点
            }
        } else {	
          	//不是边界说明还有下一跳
            int x = storeTop.x;
            int y = storeTop.y;
            node e1 = findJump(x-1,y-1,b,c,0);
            node e2 = findJump(x-1,y-1,b,c,1);
            if(e1 == e2) {
                store.push(e1);
            } else {
                searchMatrix(e1,e2,c,b,store);
            }
        }
    }
    cout<<number<<endl;
}
// 输出print栈中所有元素
void printStack(stack<node> s,string str) {
    while(!s.empty()) {
        cout<<str[s.top().x];
        s.pop();
    }
}
// 弹出print栈中长度小于等于n的元素，相当于回到前一跳的状态
void popPrint(stack<node> &s,int n) {
  	// 由于进入print栈点的lcsLen值是单调减，直接出栈直到lcsLen大于n，最底下的虚拟结点不能出栈
    while(s.top().lcsLen <= n && s.size() != 1) {
        s.pop();
    }
}
// 寻找所有下一跳，type为真：遇到跳跃点向左走，type为假：遇到跳跃点向上走，直到找到第一个跳跃点
node findJump(int x,int y,int** b,int** c,int type) {
    if(type) {
        while(b[x][y] != 1){	// 这里一定有下一个跳跃点，不用担心会找不到而越界
        if(b[x][y] == 2)
            x--;
        else
            y--;
        }
    } else {
        while(b[x][y] != 1){
        if(b[x][y] == 3)
            y--;
        else
            x--;
        }
    }
    return node(x,y,c);
}
// 如果findJump找到的两个下一跳跃点不是一个点，则以这两个点为矩形搜索里面的跳跃点，视为下一跳跃点加入store栈中
void searchMatrix(node e1,node e2,int** c,int** b,stack<node>& store) {
    int x1 = e1.x;
    int y1 = e1.y;
    int x2 = e2.x;
    int y2 = e2.y;
    int temp;
    if(x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    for(int i=x2;i>=x1;i--) {
        for(int j=y2;j>=y1;j--) {
            if(b[i][j] == 1)
                store.push(node(i,j,c));
        }
    }
}

/*
Input:
ABCDCDABCDABCDCDABCDABCDCDABCDABCDCDABCD
BADCDCBADCBADCDCBADCBADCDCBADCBADCDCBADC
*/