#include <cstdio>
#include <iostream>

#define ll long long
using namespace std;

int main() {
//    FILE * fp = fopen("table_settings_name.ini", "r");
//    rewind(fp);
//    char valid;
//    char s[1000];
//    fread(&valid, sizeof(char), 1, fp);
//    printf("%c ", valid);
//    fread(s, sizeof(char), 50, fp);
//    printf("%s\n", s);
//    for(int i = 0; i < 8; i++){
//    	fread(s, sizeof(char), 50, fp);
//    	printf("%s ", s);
//    }
//    fclose(fp);
    FILE *fp = fopen("��", "rb+");
    char s[1000];
    char valid;
    rewind(fp);
    fseek(fp, 809, SEEK_SET);
    fread(&valid, sizeof(char), 1, fp);
    printf("%c", valid);
}
