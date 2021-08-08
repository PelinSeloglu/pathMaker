#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <errno.h>
#include <stdbool.h> 
#include <unistd.h>
#include <windows.h>
#define MAX_FILE_NAME 100
#define MAX_PATH 50

//Eðer ch bir delimiter ise true döndürür.
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
  
//Eðer ch '*' ise true döndürür.
bool isOperator(char ch) 
{ 
    if ( ch == '*' ) 
        return (true); 
    return (false); 
}

//Eðer ch '>' ise true döndürür.
bool isEndOfLine(char ch)
{
	if (ch == '>' )
		return (true);
	return (false);	
}

//Eðer ch bir bracket ise true döndürür.
bool isBracket(char ch)
{
	if (ch == '{'||ch == '}')
		return (true);
	return (false);	 	
}
	
//Eðer str geçerli bir directory ise true döndürür. 
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


//Eðer str bir keyword ise true döndürür. 
bool isKeyword(char* str) 
{ 
		
    if (!strcmp(str, "if") || !strcmp(str, "ifnot") || 
        !strcmp(str, "make") || !strcmp(str, "go")) 
        return (true); 
    return (false); 
}

//'<' iþaretinden sonra '/' olup olmadýðýnýn kontrolü
bool isTrueS(char ch){
	if (ch == '<' )
		return (true);
	return (false);	
}  

//'>' iþaretinden önce '/' olup olmadýðýnýn kontrolü 
bool isTrueSl(char ch){
	if(ch == '/'){
		return (true);
	}
	return (false);
}
  
//Verilen belirli indexler arasýnda ki charlar için malloc la dinamik hafýza oluþturur
// ve str nin o indexleri arasýnda yer alan charlarý buraya atar.
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

//main fonsiyondur tokenlar burada belirlenir ve yazýlýr.
int main() {
	
	//Okunacak olan dosya açýlýr.
	FILE *fl;
	size_t n = 0;
	char *str;
	int c;
    char filename[MAX_FILE_NAME];
    printf("Enter filename (for example: text.pmk.txt): ");
	scanf("%s",filename);
    fl = fopen(filename, "r");
    
    //dosya bulunup bulunamama kontrolü
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
    
	
	
	//token iþlemlerinin baþlangýcý
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
    
    //file dosyasý küçük karfe çevriliyor
	for (i = 0; i < len; i++){
    	str[i]=tolower(str[i]);
	}
  
    while (right <= len && left <= right)
	{ 
        if (isDelimiter(str[right]) == false) 
			right++; 	  
		
		//Operatorlerin,Bracketlarýn ve EndOfLine tokenlerinin belirlenip yazdýrýlmasý burada gerçekleþir.
        if (isDelimiter(str[right]) == true && left == right) 
		{ 
      
			// * iþareti ile parent a gitme ve son parentin USER olmasýnýn kontrolü
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
				
					//if komutu için sayaç sýfýrlama ve current directory e dönme iþlemi
					if (is == 1){
						is = 0;
						int k = 0;
						for(k = 0;k<is2;k++){
							chdir("..");
						}
					}
					//ifnot komutu için sayaç sýfýrlama ve current directory e dönme iþlemi
					else if(ins == 1){
						ins = 0;
						int m = 0;
						for(m = 0;m < ins2;m++){
							chdir("..");
						}
					}
					}	
					
				
			}
			//'/' kontrolü1
			else if(isTrueS(str[right]) == true){
				if(str[right+1] == '/'){
					printf("Incorrect slash.\n");
				}
			}
			//'/' kontrolü2
			else if(isTrueSl(str[right]) == true){
				if(str[right+1] == '>'){
					printf("Incorrect slash.\n");
				}
			}
		
			//make ve go komutlarý için ; kotrolü
			else if (isEndOfLine(str[right]) == true ){
			
				//make komutu için sayaç sýfýrlama ve current directory e dönme
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
				//go komutu için sayaç sýfýrlama
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
		 
		//Keyword, directory tokenlerinin belirlenmesi, hata kontrolü yapýlmasý ve komutlarýn gerçekleþtiði yer
		else if (isDelimiter(str[right]) == true && left != right || (right == len && left != right))
		{ 
            //belirli indexler arasýndaki kelimeyi oluþturur.
			char* subStr = subString(str, left, right - 1);
  			
            //Keyword tokeni.burada sayaçlar arttýrýlýr.
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
  			
  			    
  			
  			//directory tokeni oluþturma ve hata kontrolü.
  			//sayaçlara göre komutlarýn devreye sokulmasý
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
  			
  			//directory hata kontrolü.
            else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false){
					
				printf("'%s' is not a valid directory.\n", subStr);
										 
        	}
			left = right;		
        } 
    } 
    	
	return 0;
}
