#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <errno.h>
#include <stdbool.h> 
#include <unistd.h>
#include <windows.h>
#define MAX_FILE_NAME 100
#define MAX_PATH 50

//E�er ch bir delimiter ise true d�nd�r�r.
bool isDelimiter(char ch) 
{ 
    if (ch == ' ' || ch == '<' || ch == '>' || ch == '*' ||  
        ch == '/' || ch == ',' || ch == ';' || ch == '=' || 
		ch == '(' || ch == ')' || ch == ':' || ch == '\n'||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
		ch == '"') 
        return (true); 
    return (false); 
} 
  
//E�er ch '*' ise true d�nd�r�r.
bool isOperator(char ch) 
{ 
    if ( ch == '*' ) 
        return (true); 
    return (false); 
}

//E�er ch '>' ise true d�nd�r�r.
bool isEndOfLine(char ch)
{
	if (ch == '>' )
		return (true);
	return (false);	
}

//E�er ch bir bracket ise true d�nd�r�r.
bool isBracket(char ch)
{
	if (ch == '{'||ch == '}')
		return (true);
	return (false);	 	
}
	
//E�er str ge�erli bir directory ise true d�nd�r�r. 
bool validIdentifier(char* str) 
{ 
	int len = strlen(str);
	int i= 0;
    for(i = 0 ; i < len ; i++){
    	if(isDelimiter(str[i]) == true){
    		return (false);
		}
	}
    return (true); 
}


//E�er str bir keyword ise true d�nd�r�r. 
bool isKeyword(char* str) 
{ 
		
    if (!strcmp(str, "if") || !strcmp(str, "ifnot") || 
        !strcmp(str, "make") || !strcmp(str, "go")) 
        return (true); 
    return (false); 
}

//'<' i�aretinden sonra '/' olup olmad���n�n kontrol�
bool isTrueS(char ch){
	if (ch == '<' )
		return (true);
	return (false);	
}  

//'>' i�aretinden �nce '/' olup olmad���n�n kontrol� 
bool isTrueSl(char ch){
	if(ch == '/'){
		return (true);
	}
	return (false);
}
  
//Verilen belirli indexler aras�nda ki charlar i�in malloc la dinamik haf�za olu�turur
// ve str nin o indexleri aras�nda yer alan charlar� buraya atar.
char* subString(char* str, int left, int right) 
{ 
    int i; 
    char* subStr = (char*)malloc( 
                  sizeof(char) * (right - left + 2)); 
  
    for (i = left; i <= right; i++) 
        subStr[i - left] = str[i]; 
    subStr[right - left + 1] = '\0'; 
    return (subStr); 
} 

//main fonsiyondur tokenlar burada belirlenir ve yaz�l�r.
int main() {
	
	//Okunacak olan dosya a��l�r.
	FILE *fl;
	size_t n = 0;
	char *str;
	int c;
    char filename[MAX_FILE_NAME];
    printf("Enter filename (for example: text.pmk.txt): ");
	scanf("%s",filename);
    fl = fopen(filename, "r");
    
    //dosya bulunup bulunamama kontrol�
	if (fl == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    fseek(fl, 0, SEEK_END);
    long f_size = ftell(fl);
    fseek(fl, 0, SEEK_SET);
    str = malloc(f_size);
		
	while ((c = fgetc(fl)) != EOF) {
		
        str[n++] = (char)c;
        
    }
    str[n] = '\0';
    
	
	
	//token i�lemlerinin ba�lang�c�
	int left = 0, right = 0; 
    int i, len = strlen(str); 
    int ms = 0;
    int gs = 0;
    int is = 0;
    int ins = 0;
    int ms2 = 0;
    int is2 = 0;
    int ins2 = 0;
    char cwd[PATH_MAX];
    
    //file dosyas� k���k karfe �evriliyor
	for (i = 0; i < len; i++){
    	str[i]=tolower(str[i]);
	}
  
    while (right <= len && left <= right)
	{ 
        if (isDelimiter(str[right]) == false) 
			right++; 	  
		
		//Operatorlerin,Bracketlar�n ve EndOfLine tokenlerinin belirlenip yazd�r�lmas� burada ger�ekle�ir.
        if (isDelimiter(str[right]) == true && left == right) 
		{ 
      
			// * i�areti ile parent a gitme ve son parentin USER olmas�n�n kontrol�
			if (isOperator(str[right]) == true) {
				char d[100];
                if(getcwd(d,100) != "USER"){
                	
               		int result = chdir(".."); 
					if(result != 0 ){
						printf("The requested directory could not be accessed.\n");			
					}
					if(str[right+1] == '/'){
						right++;
					}
					if(is == 1 ){
						int result = chdir(".."); 
						
						if(result != 0 ){
							printf("The requested directory could not be accessed.\n");
							while(str[right+1]=='}'){
								right++;
							}			
						}
					
					
					}
					else if(ins == 1){
						int result = chdir("..");
						 
						ins2++;
						if(result == 0 ){
							printf("The requested directory could not be accessed.\n");
							while(str[right+1]=='}'){
								right++;
							}			
						}
					
					}
				}
				else{
					printf("Can not use USER's parent directory.\n");
				}
                
			}
           
			else if (isBracket(str[right]) == true ){
				
				if ( str[right] == '{'){
				
					//if komutu i�in saya� s�f�rlama ve current directory e d�nme i�lemi
					if (is == 1){
						is = 0;
						int k = 0;
						for(k = 0;k<is2;k++){
							chdir("..");
						}
					}
					//ifnot komutu i�in saya� s�f�rlama ve current directory e d�nme i�lemi
					else if(ins == 1){
						ins = 0;
						int m = 0;
						for(m = 0;m < ins2;m++){
							chdir("..");
						}
					}
					}	
					
				
			}
			//'/' kontrol�1
			else if(isTrueS(str[right]) == true){
				if(str[right+1] == '/'){
					printf("Incorrect slash.\n");
				}
			}
			//'/' kontrol�2
			else if(isTrueSl(str[right]) == true){
				if(str[right+1] == '>'){
					printf("Incorrect slash.\n");
				}
			}
		
			//make ve go komutlar� i�in ; kotrol�
			else if (isEndOfLine(str[right]) == true ){
			
				//make komutu i�in saya� s�f�rlama ve current directory e d�nme
				if(ms == 1){
					int l = 0;
					ms = 0;
					if(str[right+1] != ';'){
						printf("Please put ';' at the end of make commands.Your command still occurred.\n");
					}
					for(l = 0; l<ms2;l++){
						chdir("..");
					}
					ms2 = 0;
				}
				//go komutu i�in saya� s�f�rlama
				else if(gs == 1){
					
					if(str[right+1] != ';'){
						printf("Please put ';' at the end of go commands.Your command still occurred.\n");
					}
					gs = 0;
				}
				
				}
			
            right++; 
            left = right;
		}
		 
		//Keyword, directory tokenlerinin belirlenmesi, hata kontrol� yap�lmas� ve komutlar�n ger�ekle�ti�i yer
		else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right))
		{ 
            //belirli indexler aras�ndaki kelimeyi olu�turur.
			char* subStr = subString(str, left, right - 1);
  			
            //Keyword tokeni.burada saya�lar artt�r�l�r.
			if (isKeyword(subStr) == true) {
			
                
				if(strcmp(subStr, "make") == 0){
					ms++;
										
				}
				if(strcmp(subStr, "go") == 0){
					gs++;
										
				}
				if(strcmp(subStr, "if") == 0){
					is++;
				}
				if(strcmp(subStr, "ifnot") == 0){
					ins++;
				}
				
			} 
  			
  			    
  			
  			//directory tokeni olu�turma ve hata kontrol�.
  			//saya�lara g�re komutlar�n devreye sokulmas�
            else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false){
				
                
                if(ms == 1){
                	int dir_result = mkdir(subStr);    
					       	
					if(dir_result != 0 && errno == EEXIST){
						printf("This directory is already exists.\n");
   					}
   					
					int result = chdir(subStr);
					ms2++; 
   					
					if(result != 0 ){
						printf("The requested directory could not be accessed.\n");
					
					} 
				}
				else if(gs == 1){
					
					int result = chdir(subStr); 
					if(result == -1 ){
						printf("The requested directory could not be accessed.\n");
					
					} 
				}
				else if(is == 1){
					int result = chdir(subStr); 
					is2++;
					if(result != 0 ){
						
						while(subStr[right+1]=='}'){
							right++;
						}
					
					}
								
				}
				else if(ins == 1){
					int result = chdir(subStr);
					ins2++; 
					if(result == 0 ){
						
						while(subStr[right+1]=='}'){
							right++;
						}
					
					}
					
				}
							
			}
  			
  			//directory hata kontrol�.
            else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false){
					
				printf("'%s' is not a valid directory.\n", subStr);
										 
        	}
			left = right;		
        } 
    } 
    	
	return 0;
}
