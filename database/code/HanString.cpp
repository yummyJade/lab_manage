#include <iostream>
#include <algorithm>
#include <vector>
#include <string> 
using namespace std;

class Character{
	public:
		char c[2];
		bool isHan;
		Character(char a, bool isHan, char b = '\0'){
			c[0] = a;
			c[1] = b;
			this->isHan = isHan;
		}
		bool operator == (const Character & o){
			if(isHan == o.isHan){
				if(c[0] == o.c[0] && c[1] == o.c[1]){
					return true;
				}
			}
			return false;
		}
		bool operator != (const Character & o){
			if(isHan == o.isHan){
				if(c[0] == o.c[0] && c[1] == o.c[1]){
					return false;
				}
			}
			return true;
		}
		
		bool operator == (const Character & o) const {
			if(isHan == o.isHan){
				if(c[0] == o.c[0] && c[1] == o.c[1]){
					return true;
				}
			}
			return false;
		}
		bool operator != (const Character & o) const {
			if(isHan == o.isHan){
				if(c[0] == o.c[0] && c[1] == o.c[1]){
					return false;
				}
			}
			return true;
		}
};

class HanString{
	private:
		vector<int> next;
	public:
		vector<Character> s;
		bool isHan(string a, int i){
			char c = a[i];
			if((c>>7)&1 == 1) return true;
			else return false;
		}
		
		HanString(const string & a){
			int n = a.length(); 
			s.clear();
			for(int i = 0; i < n; i++){
				if(isHan(a, i)){
					s.push_back(Character(a[i], true, a[i+1]));   
					i++;
				}
				else{
					s.push_back(Character(a[i], false));  
				}
			}
		}
		
		void getNext(){
			next.resize(s.size()+1); 
			next[0] = 0;
			int j = 0;
			int n = s.size();
			for(int i = 1; i < n; i++){
				while(j > 0 && s[i] != s[j]){
					j = next[j-1];
				}
				if(s[i] == s[j]){
					j++;
				}
				next[i] = j;
			}
		}
		
		bool in(const HanString & a){
			int n = a.s.size();
			int j = 0;
			int m = s.size();
			for(int i = 0; i < n; i++){
				//cout << j << endl;
				while(j > 0 && a.s[i] != s[j]){
					j = next[j-1];
				}
				
				if(a.s[i] == s[j]){
					j++;
				}
				if(j == m) return true;
			}
			return false;
		}
	
};


// int main(){
// 	string a, b;
// 	cin >> a;
// 	HanString aa(a);
// //	int n = min(aa.s.size(), bb.s.size());
// //	for(int i = 0; i < n; i++){
// //		if(aa.s[i] == bb.s[i]){
// //			cout << "Y" << endl;
// //		}
// //	}
// 	aa.getNext(); 
// 	while (1) {
// 		cin >> b;
// 		if (aa.in(b)) {
// 			cout << "Y" << endl;
// 		}
// 	}
// }
