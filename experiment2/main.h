#include<bits/stdc++.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<error.h>
#include<wait.h>
#include<unistd.h>
using namespace std;

bool P(string a);
void V(string a);
void consum(int n);
void product(int n);
void Show();
void show_queue(queue<int> temp);
void show_queue(queue<string> temp);
void exeend();
bool mutex_proc();
bool produce_proc();
bool consum_proc();


