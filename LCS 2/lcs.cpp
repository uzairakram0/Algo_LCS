#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#define S 16

using namespace std;

/*Function Prototypes*/
int LCS(string A, string B, int i, int j);
int LCS_Length(string X, string Y);
void Print_LCS(vector<vector<char> >, string X, int i, int j);
void grouping (string labels[S], string sequences[S]);
void printTable(vector<string> labels, vector<vector<int> > table);
void printVector(vector<string> V);
void buildMatrix(vector<string>G, vector<vector<int> > M, int n);
void readfile(string sequences[S], string labels[S]);
int max(int a, int b);

int main(){
    string sequences[S];
    string labels[S];
    readfile(sequences, labels);
    
    
    for (int i = 0; i < S; i++){
        cout << labels[i] << endl;
        cout << sequences [i] << endl;
    }
    
    cout << "FOR 2B";
    for(int i = 0; i < S-1; i++){
        for (int j = i+1; j < S; j++){
            cout << labels[i] << "\t" << labels[j] << endl;
            int LCS_size = LCS_Length(sequences[i], sequences[j]);
            cout << LCS_size << endl;
        }
    }
    
    grouping (labels, sequences);
    
    return 0;
}

/*BruteForce Algortihm for finding LCS*/
int LCS(string A, string B, int m, int n){
    if(m == 0 || n == 0)
        return 0;
    else if (A[m-1] == B[n-1])
        return 1 + LCS(A, B, m-1, n-1);
    else
        return max(LCS(A, B, m-1, n), LCS(A, B, m, n-1));
}

//Citation: introduction to Algorithm Chapter 15.4 LCS
/*This Algorithm computes an LCS matrix*/
int LCS_Length(string X, string Y){
    int m = X.length();
    int n = Y.length();
    
    int c[m][n];
    char b[m][n];
    
    for (int i = 0; i < m; i++)
        c[i][0] = 0;
    for (int j = 0; j < n; j++)
        c[0][j] = 0;
        
    for (int i = 1; i < m; i++){
        for (int j = 1; j < n; j++){
            if(X[i] == Y[i]){
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 'd';
            } else if(c[i-1][j] >= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j] = 'u';
            } else {
                c[i][j] = c[i][j-1];
                b[i][j] = 'l';
            }
        }
    }
    
    vector<vector<char> > B;
    for (int i = 0; i < m; i++){
        vector<char> tmp;
        for (int j = 1; j < n; j++){
            tmp.push_back(b[i][j]);
        }
        B.push_back(tmp);
    }
    
    //Print_LCS(B, X, m, n);
    
    return c[m-1][n-1];
}

/*Print LCS function*/
void Print_LCS(vector<vector<char> > b, string X, int i, int j){
    if (i==0 || j==0)
        return;
    else if(b[i][j] == 'd'){
       Print_LCS(b, X, i-1, j-1);
       cout << X[i];
    } else if (b[i][j] == 'u')
        Print_LCS(b, X, i-1, j);
    else
        Print_LCS(b, X, i, j-1);
    
}

//Function for grouping
/*The Grouping function finds the maximum match between two sequences and then creates a grouping*/
void grouping (string labels[S], string sequences[S]){
    
    vector<vector<int> > table (16, vector<int>(16, 0));
    
    for(int i = 0; i < S-1; i++)
        for (int j = i+1; j < S; j++)
            table[i][j] = LCS_Length(sequences[i], sequences[j]);
    
    //build vector of labels
    vector<string> L;
    for(int i = 0; i < S; i++){
        L.push_back(labels[i]);
    }
    
    cout << "\n\nMatrix" << endl;
    printTable(L, table);
    cout << endl;
    
    buildMatrix(L, table, S);
}

/*Matrix for building groupings*/
void buildMatrix(vector<string> L, vector<vector<int> > M, int n){
    
    if (n == 1)
        return;
    
    vector<vector<int> > matrix (n-1, vector<int>(n-1, 0));
    cout << "n =" << n << endl;
    string pair;
    vector<string> l;   //labels vector
    
    int max = 0;
    int a;
    int b;
    
    for(int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            if(M[i][j] > max){
                max = M[i][j];
                a = i;
                b = j;
            }
        }
    }
    
    pair = "[" + L[a] + " ," + L[b] + "]";
    l.push_back(pair);
    
    for (int i = 0; i < L.size(); i++){
        if (i != a && i != b)
            l.push_back(L[i]);
    }
    
    
    for(int i = 0; i < M.size(); i++){
        int x, y;
        
        if(M[b][i] != 0)
            x = M[b][i];
        else
            x = M[i][b];
        
        if(M[b][i] != 0)
            y = M[a][i];
        else
            y = M[i][a];
        
        matrix[0][i] = (x+y)/2;
    }
    
    for(int i = 1; i < matrix.size(); i++)
        for (int j = i+1; j < matrix.size(); j++)
                matrix[i][j] = M[i][j];
    
    for (int i = 0; i < l.size(); i++)
        cout << l[i] << " ";
    cout << endl;
    
    //printTable(l, matrix);
    buildMatrix(l, matrix, n-1);
}

//function for reading file into the arrays
void readfile(string sequences[S], string labels[S]){
    ifstream infile;
    infile.open("tulp3_relatives.txt");
    
    string line;
    int n =0;
    
    while(getline(infile, line)){
        if(line[0] == '>'){
            labels[n] = line;
            getline(infile, line);
            sequences[n] = line;
            n++;
        }
    }
    
    infile.close();
}

//auxillary function for getting max of 2 values
int max(int a, int b){
    return (a > b) ? a : b;
}

//The function prints table
void printTable(vector<string> labels, vector<vector<int> > table){
    
    cout << "\t";

    for(int i = 0; i < labels.size(); i++)
        cout << labels[i] << " ";
    cout << endl;
    
    for(int i = 0; i < table.size(); i++){
        cout << labels[i] << "\t\t";
        for (int j = i+1; j < S; j++)
            cout << table[i][j] << "\t";
        cout << endl;
    }
            
}
