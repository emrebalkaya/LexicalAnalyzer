#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

char *variableList[1000]; int vIndex = 0;
int integerList[1000] = { 0 }; int iIndex = 0;

bool isValidSeperator(char ch) {
   if (ch == ' ' || ch == ',' || ch == '.' || ch == '{' || ch == '[' || ch == ']' )
   {
   		return (true);
   }
   return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool isvalidIdentifier(char* str){
	if(isalpha(str[0])){
		int i;
		for(i;i<strlen(str);i++)
   		{
	   		if(!isalnum(str[i]))
	   		{
	   			return (false);
			}
   		}
    	return (true);
	}
	return (false);
}

bool isValidKeyword(char* str) {
   if (!strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") || !strcmp(str, "sub") || !strcmp(str, "out") || !strcmp(str, "loop") || !strcmp(str, "int")
   || !strcmp(str, "times") || !strcmp(str, "newline") || !strcmp(str, "from"))
   {
   		return (true);
   }
   return (false);
}

bool isValidInteger(char* str) {
   int i, len = strlen(str);
   if (len == 0)
   return (false);
   for (i = 0; i < len; i++) {
      if (str[i] != '0' && str[i] != '1' && str[i] != '2'&& str[i] != '3' && str[i] != '4' && str[i] != '5'
      && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
      return (false);
   }
   return (true);
}


char* subString(char* str, int left, int right) {
   int i;
   char* subStr = (char*)malloc( sizeof(char) * (right - left + 2));
   for (i = left; i <= right; i++)
      subStr[i - left] = str[i];
   subStr[right - left + 1] = '\0';
   return (subStr);
}

void detectTokens(char* str, char* filename) {
	FILE *laFile;
	laFile = fopen(filename,"w");
	if(laFile == NULL)
	{
		printf("File could not be found");   
	}
	else
	{
		int left = 0, right = 0;
		int length = strlen(str);
		int closeindex = 0;
		while (right <= length && left <= right) {
			if (isValidSeperator(str[right]) == false)
			right++;
			if (isValidSeperator(str[right]) == true && left == right) {
				if(str[right] == '.')
					fprintf(laFile, "%s" , "EndOfLine\n");	
				else if(str[right] == ',')
					fprintf(laFile, "%s" , "Seperator\n");
				else if(str[right] == '{')
				{
					left = right;
					right++;
					while(str[right] != '}'){
						if(right<strlen(str)-1)
							right++;
						if(right >= strlen(str)-1)
							break;
					}
					if(str[right] != '}'){
						printf("%s" , "CommentError: Comment line has not finished \n");
					}
				}
				else if(str[right] == '[')
				{
					fprintf(laFile, "%s" , "OpenBlock\n");
					left = right;
					right++;
					while(str[right] != ']'){
						if(right<strlen(str)-1)
							right++;
						if(right >= strlen(str)-1)
							break;
					}
					if(str[right] != ']'){
						printf("%s" , "BlockError: There is no CloseBlock \n");
					}
					if(str[right] == ']'){
						closeindex = right;
						
					}
					right = left;
				}
				else if(str[right] == ']'){
					if(right == closeindex){
						fprintf(laFile, "%s" , "CloseBlock\n");
					}
					else{
						printf("%s" , "BlockError: There is no OpenBlock \n");					
					}
				}
				right++;
				left = right;
			}
			else if (isValidSeperator(str[right]) == true && left != right || (right == length && left !=       right)) {
				char* subStr = subString(str, left, right - 1);
				if (isValidKeyword(subStr) == true)
					fprintf(laFile, "Keyword : %s\n", subStr);
				else if (isValidInteger(subStr) == true && strlen(subStr)<=5)
					fprintf(laFile, "Integer : %s\n", subStr);
				else if (isValidInteger(subStr) == true && strlen(subStr)>5)
					printf("Integer Error :  An integer can be as big as 100 decimal digits -> %s\n", subStr);
				else if (isvalidIdentifier(subStr) == true
				&& isValidSeperator(str[right - 1]) == false && strlen(subStr)<=20)
					fprintf(laFile, "Identifier : %s\n", subStr);
				else if (isvalidIdentifier(subStr) == true
				&& isValidSeperator(str[right - 1]) == false && strlen(subStr)>20)
					printf("Identifier Error : Variable names have a maximum length of 20 characters -> %s\n", subStr);
				else if (isvalidIdentifier(subStr) == false
				&& isValidSeperator(str[right - 1]) == false && subStr[0] == '"' && subStr[strlen(subStr)-1] == '"')
					fprintf(laFile, "StringConstant : %s\n", subStr);
				else if (isvalidIdentifier(subStr) == false
				&& isValidSeperator(str[right - 1]) == false)
					printf("Identifier Error :  Unknown character -> %s\n", subStr);
				left = right;
			}
		}
	}
	fclose(laFile);
	return;
}
bool isIntegerVariable(char* str) {
		int i;
        for ( i = 0; i < vIndex; i++) {
                if (!strcmp(str, variableList[i])) {
                        return true;
                }
        }

        return false;
}
void parse(char* code) {
        int lineCount = 0;
        char *line[1000];
        char *splitCode = strtok(code, ".");
        while (splitCode != NULL) {
                line[lineCount++] = splitCode;
                splitCode = strtok(NULL, ".");
        }
		int i;
        for ( i = 0; i < lineCount; i++) {
                int wordCount = 0;
                char *word[1000];
                char *splitLine = strtok(line[i], " \n\r[]\"");
                while (splitLine != NULL) {
                        word[wordCount++] = splitLine;
                        splitLine = strtok(NULL, " \n\r");
                }

                if (wordCount == 0) { return; }

                //Int
                if (!strcmp(word[0], "int")) {
                        printf("'int' is a keyword.\n");

                        //Variable
                        if (word[1] == NULL) {
                                printf("Variable is expected.\n");
                                return ;
                        }
                        else if (isValidKeyword(word[1])) {
                                printf("'%s' is a keyword. Variable is expected.\n", word[1]);
                                return ;
                        }
                        else if (isvalidIdentifier(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);

                                variableList[vIndex++] = word[1];
                                integerList[iIndex++] = 0;
                        }
                        else {
                                printf("'%s' is not a variable. Variable is expected.\n", word[1]);
                                return ;
                        }

                        //End Of Line
                        if (wordCount == 2) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return ;
                        }
                }


                //Move
                else if (!strcmp(word[0], "move")) {
                        printf("'move' is a keyword.\n");

                        //Integer Or Variable
                        int moveInt = 0;
                        if (word[1] == NULL) {
                                printf("Integer or variable are expected.\n");
                                return ;
                        }
                        else if (isValidKeyword(word[1])) {
                                printf("'%s' is a keyword. Integer or variable are expected.\n", word[1]);
                                return ;
                        }
                        else if (isValidInteger(word[1])) {
                                printf("'%s' is an integer.\n", word[1]);

                                moveInt = atoi(word[1]);
                        }
                        else if (isIntegerVariable(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);
								int i;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[1])) {
                                                moveInt = integerList[i];
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a integer or variable. Integer or variable are expected.\n", word[1]);
                                return;
                        }

                        //To
                        if (word[2] == NULL) {
                                printf("Keyword 'to' is expected.\n");
                                return;
                        }
                        else if (!strcmp(word[2], "to")) {
                                printf("'to' is a keyword.\n");
                        }
                        else {
                                printf("Keyword 'to' is expected.\n");
                                return;
                        }

                        //Variable
                        if (word[3] == NULL) {
                                printf("Variable is expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[3])) {
                                printf("'%s' is a keyword. Variable is expected.\n", word[3]);
                                return;
                        }
                        else if (isIntegerVariable(word[3])) {
                                printf("'%s' is a variable.\n", word[3]);
								int i;
                                for (i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[3])) {
                                                integerList[i] = moveInt;
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a variable. Variable is expected.\n", word[3]);
                                return;
                        }

                        //End Of Line
                        if (wordCount == 4) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return;
                        }
                }


                //Add
                else if (!strcmp(word[0], "add")) {
                        printf("'add' is a keyword.\n");

                        //Integer Or Variable
                        int addInt = 0;
                        if (word[1] == NULL) {
                                printf("Integer or variable are expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[1])) {
                                printf("'%s' is a keyword. Integer or variable are expected.\n", word[1]);
                                return;
                        }
                        else if (isValidInteger(word[1])) {
                                printf("'%s' is an integer.\n", word[1]);

                                addInt = atoi(word[1]);
                        }
                        else if (isIntegerVariable(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[1])) {
                                                addInt = integerList[i];
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a integer or variable. Integer or variable are expected.\n", word[1]);
                                return;
                        }

                        //To
                        if (word[2] == NULL) {
                                printf("Keyword 'to' is expected.\n");
                                return;
                        }
                        else if (!strcmp(word[2], "to")) {
                                printf("'to' is a keyword.\n");
                        }
                        else {
                                printf("Keyword 'to' is expected.\n");
                                return;
                        }

                        //Variable
                        if (word[3] == NULL) {
                                printf("Variable is expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[3])) {
                                printf("'%s' is a keyword. Variable is expected.\n", word[3]);
                                return;
                        }
                        else if (isIntegerVariable(word[3])) {
                                printf("'%s' is a variable.\n", word[3]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[3])) {
                                                integerList[i] += addInt;
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a variable. Variable is expected.\n", word[3]);
                                return;
                        }

                        //End Of Line
                        if (wordCount == 4) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return;
                        }
                }


                //Sub
                else if (!strcmp(word[0], "sub")) {
                        printf("'sub' is a keyword.\n");

                        //Integer Or Variable
                        int subInt = 0;
                        if (word[1] == NULL) {
                                printf("Integer or variable are expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[1])) {
                                printf("'%s' is a keyword. Integer or variable are expected.\n", word[1]);
                                return;
                        }
                        else if (isValidInteger(word[1])) {
                                printf("'%s' is an integer.\n", word[1]);

                                subInt = atoi(word[1]);
                        }
                        else if (isIntegerVariable(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[1])) {
                                                subInt = integerList[i];
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a integer or variable. Integer or variable are expected.\n", word[1]);
                                return;
                        }

                        //To
                        if (word[2] == NULL) {
                                printf("Keyword 'from' is expected.\n");
                                return;
                        }
                        else if (!strcmp(word[2], "from")) {
                                printf("'to' is a keyword.\n");
                        }
                        else {
                                printf("Keyword 'from' is expected.\n");
                                return;
                        }

                        //Variable
                        if (word[3] == NULL) {
                                printf("Variable is expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[3])) {
                                printf("'%s' is a keyword. Variable is expected.\n", word[3]);
                                return;
                        }
                        else if (isIntegerVariable(word[3])) {
                                printf("'%s' is a variable.\n", word[3]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[3])) {
                                                integerList[i] -= subInt;
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a variable. Variable is expected.\n", word[3]);
                                return;
                        }

                        //End Of Line
                        if (wordCount == 4) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return;
                        }
                }

                //Out
                else if (!strcmp(word[0], "out")) {
                        printf("'out' is a keyword.\n");

                        //Integer, Variable Or String
                        if (word[1] == NULL) {
                                printf("Integer, variable or string are expected.\n");
                                return;
                        }
                        else if (isValidInteger(word[1])) {
                                printf("'%s' is an integer.\n", word[1]);

                                printf("Integer Value: %s\n", word[1]);
                        }
                        else if (isIntegerVariable(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[1])) {
                                                printf("'%s' variable value: %i\n", word[1], integerList[i]);
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("%s\n", word[1]);
                        }

                        //End Of Line
                        if (wordCount == 2) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return;
                        }
                }

                //Loop
                else if (!strcmp(word[0], "loop")) {
                        printf("'loop' is a keyword.\n");

                        //Integer Or Variable
                        int loopInt = 0;
                        if (word[1] == NULL) {
                                printf("Integer or variable are expected.\n");
                                return;
                        }
                        else if (isValidKeyword(word[1])) {
                                printf("'%s' is a keyword. Integer or variable are expected.\n", word[1]);
                                return;
                        }
                        else if (isValidInteger(word[1])) {
                                printf("'%s' is an integer.\n", word[1]);

                                loopInt = atoi(word[1]);
                        }
                        else if (isIntegerVariable(word[1])) {
                                printf("'%s' is a variable.\n", word[1]);
								int i ;
                                for ( i = 0; i < vIndex; i++) {
                                        if (!strcmp(variableList[i], word[1])) {
                                                loopInt = integerList[i];
                                                break;
                                        }
                                }
                        }
                        else {
                                printf("'%s' is not a integer or variable. Integer or variable are expected.\n", word[1]);
                                return;
                        }

                        //Times
                        if (word[2] == NULL) {
                                printf("Keyword 'times' is expected.\n");
                                return;
                        }
                        else if (!strcmp(word[2], "times")) {
                                printf("'times' is a keyword.\n");
                        }
                        else {
                                printf("Keyword 'times' is expected.\n");
                                return;
                        }

                        //Code
                        int i;
                        for ( i = 0; i < loopInt; i++) {
                                char newCode[100] = "";
                                int i ;
                                for ( i = 3; i < wordCount; i++) {
                                        strcat(newCode, " ");
                                        strcat(newCode, word[i]);
                                }
                                strcat(newCode, ".");
                                parse(newCode);
                        }

                        //End Of Line
                        if (wordCount > 4) {
                                printf("'.' End of Line.\n");
                        }
                        else {
                                printf("End of line is expected.\n");
                                return;
                        }
                }

                else {
                        printf("%s", word[0]);
                }
        }
}

int main(int argc, char *argv[]) {
	char input[50];
	char fileName[50] = "";
	char baFile[30] = "";
	char lxFile[30] = "";
	start: 
	
	printf("\nType any command: ");
	
	gets(input);
	
	char * token = strtok(input, " ");
	if( strcmp(token, "la"))
	{
		printf("\nInvalid command -> %s\n", token );
		goto start;
	}
	int i = 0;
    while( token != NULL ) {
		token = strtok(NULL, " ");
		i++;
		if(i==1){
		 strcat(fileName,token);
		}      	
    }
	strcat(baFile,fileName);
	strcat(baFile,".ba");

	strcat(lxFile,fileName);
	strcat(lxFile,".lx");
	
	FILE *file;
	file = fopen(baFile,"r");
	char str[99999];
	char s[99999];
	char d[99999];
	if(file == NULL)
	{
		printf("File could not be found");   
	}
	else
	{	
		int count = 0;
		while(!feof(file)){
			fscanf(file,"%s",d);
			count++;
		}
		rewind(file);
		int counter = 0;
		while(!feof(file)){
			fscanf(file,"%s",s);
			strcat(s," ");
			strcat(str,s);
			counter++;
			if(counter == (count-1))
			{
				break;
			}
		}
		detectTokens(str, lxFile);
		parse(str);
	}
	
	
	fclose(file);
	
	return 0;
}
