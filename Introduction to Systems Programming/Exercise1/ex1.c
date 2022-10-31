#include <stdio.h>
#define HA_range 20
void histogramArray(int [], int);
void printMaxPartAscending(int[], int);
//int checkSaddleElement(int[][10], int, int);
void rotateString(char[], int, int);
int countWords(char[][8], int, int, char[]);
int countWords_rows(char [][8], int , int , char []);
int countWords_cols(char[][8], int, int, char[]);
int countWords_diag(char[][8], int, int, char[]);
void decipher(char[]);
void rotateMatrix90(int[][3], int, int);
void FindSimilarWords(char[]);
void IsFamiliar(char[], int, int, int, int);

//void main() {
	//int array[10] = { 0,0,0,0,2,3,0,0,0,0 };
	//int matrix[8][10] = { {1,2,0,3,9,4,5,3,6,7},{2,4,3,5,9,0,2,3,4,1},{5,3,4,7,6,1,9,0,4,2},{6,9,7,8,6,7,7,4,5,6},{8,3,1,9,2,0,6,2,8,2},{2,7,3,0,3,6,3,1,5,3},{8,2,5,9,7,7,8,3,7,3},{1,7,4,8,5,8,5,0,0,6} };
	//histogramArray(array, 10);
	//printMaxPartAscending(array, 10);
	// checkSaddleElement(matrix, 8, 10);
	//char t[] = "Btwlzx Dqqes Eq|pj4 Tjhvqujs Iqoqjy bpg Eqfxtx Xcwwtt5";
	//rotateString(t, 3, 2);
	//int mat[3][8] = { {'t','o','m',3,9,4,5,'t'},{'o','o',3,'t','o','m',2,'o'},{'m',4,'m',5,9,0,2,'m'} };
	//countWords_rows(mat, 3, 8, "tom");
	//countWords_cols(mat, 3, 8, "tom");
	//countWords_diag(mat, 3, 8, "tom");
	//countWords(mat, 3, 8, "tom");
	//decipher(t);
	//char tifzoret[][8] = { {'a','w', 'o', 'r' ,'d', 'b' ,'c' ,'w'} ,{ 'd', 'o' ,'o' ,'e', 'f' ,'h', 'i' ,'o'} ,{ 'j' ,'r', 'k' ,'r', 'w' ,'l' ,'w' ,'r'}, { 'm', 'd', 'n' ,'o', 'd', 'o', 'o', 'd'
		// }, {'p' ,'q', 'r', 's', 'w', 'o', 'r' ,'d'}, {
		 //'t', 'u', 'v' ,'w', 'x', 'y', 'd', 'd'} };
	//char word[] = "dd";
	//printf("%d words", countWords(tifzoret, 6, 8, word));
	//int matrix[4][3] = { {1,2,3},{4,5,6},{7,8,9},{11,12,13} };
	//rotateMatrix90(matrix, 3, 3);
	//char str[] = "Nanny have you any cheap peach ?";
	//FindSimilarWords(str);

int main(void) {
	while (1) {
		printf("Please choose assignment from 1 to 7\n");
		int ass;
		scanf_s("%d", &ass);
		switch (ass) {
		case 1:
		{
			int arr[20] = { 3,6,4,1,1,2,2,4,7,6,2,2,5,5,6,8,9,4,2,1 };
			histogramArray(arr, 12);
			break;
		}
		case 2:
		{
			int arr[20] = { -2,8,23,-160,-100,-74,8,85,-47,-10,-0,12,-1,-2,-3,12,10,11,3,-12 };
			printMaxPartAscending(arr, 10);
			break;
		}
		//case 3:
		{
			//int ans = -1;
			//int arr[5][10] = { {1,2,3,4,5,7,3,5,2,3},{7,4,2,1,3,8,4,6,7,9},{9,5,6,7,8,1,7,0,3,7}
			//,{3,0,2,6,4,5,8,3,0,8},{1,3,8,9,2,1,4,2,7,5} };
			//printf("Start!\n");
			//ans = checkSaddleElement(arr, 5, 5);
			//printf("%d\n", ans);
			//break;
		}
		case 4:
		{
			int i, j;
			int arr[4][3] = { {1,2,3},{4,5,6},{7,8,9},{10,11,12} };
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 3; j++) {
					printf("%d", arr[i][j]);
				}
				printf("\n");
			}
			rotateMatrix90(arr, 3, 3);
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 3; j++) {
					printf("%d", arr[i][j]);
				}
				printf("\n");
			}
			break;
		}
		case 5:
		{
			char str[7] = "rotatel";
			rotateString(str, 9, 1);
			printf("%s\n", str);

			rotateString(str, 9, 2);
			printf("%s\n", str);
			break;
		}
		case 6:
		{
			char arr[4][8] = { {'c','t','c','m','s','c','a','r'},
			{'a','a','a','t','f','g','h','l'},{'g','a','r','j','e','x','f','b'},{'c','a','r','h','a','v','j','k'} };
			printf("%d\n", countWords(arr, 4, 8, "car"));
			break;
		}
		case 7:
		{
			char str[55] = "Btwlzx Dqqes Eq|pj4 Tjhvqujs Iqoqjy bpg Eqfxtx Xcwwtt5";
			char str2[4] = "bcde";
			decipher(str);
			decipher(str2);
			printf("\n");
			break;
		}
		case 8:
		{
			FindSimilarWords("Nanny have you any cheap peach ?");
			break;
		}
		case 0:
			return 1;
		}
	}
	return 1;
}



void histogramArray(int Array[], int size) {		
	int cnt_array[HA_range+1] = { 0 };						//Counting the show times of each number between 0-20.
	for (int i = 0; i < size; i++) {
		cnt_array[Array[i]]++;
	}
	printf("Value             Histogram\n");		//Print the headers of the Histogram
	for (int i = 0; i < HA_range+1; i++) {
		if (cnt_array[i]) {							//cnt_array[i] is the number of times of the number i.
			printf("%d                 ", i);
			while (cnt_array[i]) {
				printf("*");
				cnt_array[i]--;
				if (cnt_array[i] == 0) printf("\n");
			}
		}
	}
}	
void printMaxPartAscending(int array[], int size) {
	int start_index = 0,indexmax = 0;				//indexmax is the index of the stard element of the serious.
	int len = 0, maxlen = 0;						//maxlen is the lentgh of the serious.
	for (int i = 1; i < size; i++) {
		if (array[i-1] < array[i]) len++;			//chacking if the serious is upping.
		else {
			len = 0;
			start_index = i;
		}
		if (len > maxlen) {
			maxlen = len;
			indexmax = start_index;
		}
	}
	if (maxlen) {
		for (int j = 0; j < maxlen+1; j++) {
			printf("%d, ", array[indexmax + j]);		//print the serious.
		}
	}
	else printf("No such segment!");
}
void rotateString(char str[], int amount, int direction) {
	if (direction == 1) {								//shift left
		char first;
		int j;
		for (int i = 0; i < amount; i++) {				//number of shifts
			first = str[0];
			j = 1;
			while (str[j] != '\0') {
				str[j - 1] += (str[j] - str[j - 1]);	//shifting one by one.
				j++;
			}
			str[j - 1] = first;
		}
		printf("%s", str);
	}
	else {												//shift right
		int len = 0;
		int j;
		while (str[len] != '\0') len++;
		for (int i = 0; i < amount; i++) {
			char last = str[len - 1];
			j = len-1;
			while (j>0) {
				str[j] += (str[j-1] - str[j]);
				j--;
			}
			str[0] = last;
		}
		printf("%s", str);
	}
	
}
int countWords_rows(char mat[][8], int rows, int cols, char word[]) {
	int word_cnt = 0;										//count the number of times that the word appear in the rows of the matrix.
	char current_char = word[0];							//current char in the word.
	int word_index = 0;										//the index of the char.
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (mat[i][j] == current_char) {
				word_index++;
				current_char = word[word_index];
					if (current_char == '\0') {
						word_index = 0;
						current_char = word[0];
						word_cnt++;
					}
			}
			else {
				word_index = 0;
				current_char = word[0];
			}
		}

	}
	//printf("%d", word_cnt);
	return(word_cnt);
}
int countWords_cols(char mat[][8], int rows, int cols, char word[]) {
	int word_cnt = 0;											//same as countWord_rows function.
	char current_char = word[0];
	int word_index = 0;
	for (int j = 0; j < cols; j++) {
		for (int i = 0; i < rows; i++) {
			if (mat[i][j] == current_char) {
				word_index++;
				current_char = word[word_index];
				if (current_char == '\0') {
					word_index = 0;
					current_char = word[0];
					word_cnt++;
				}
			}
			else {
				word_index = 0;
				current_char = word[0];
			}
		}

	}
	//printf("%d", word_cnt);
	return(word_cnt);
}
int countWords_diag(char mat[][8], int rows, int cols, char word[]) {
	int word_cnt = 0;
	char current_char = word[0];
	int word_index = 0;
	int min;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (mat[i][j] == word[0]) {
				current_char = word[0];
				word_index = 0;
				min = (rows - i);
				if ((rows-i)>(cols-j)) min=(cols-j);
				for (int k = 0; k < min; k++) {						//running on the diagonal.
					if (mat[i + k][j + k] == current_char) {
						word_index++;
						current_char = word[word_index];
						if (current_char == '\0') {
							word_index = 0;
							current_char = word[0];
							word_cnt++;
							break;
						}
					}
					else {
						word_index = 0;
						current_char = word[0];
					}
				}
			}
			
			
		}

	}
	//printf("%d", word_cnt);
	return(word_cnt);
}
int countWords(char mat[][8], int rows, int cols, char word[]) {
	int sum_rows = 0;
	int sum_cols = 0;
	int sum_diag = 0;
	sum_rows = countWords_rows(mat, rows, cols, word);
	sum_cols = countWords_cols(mat, rows, cols, word);
	sum_diag = countWords_diag(mat, rows, cols, word);
	return(sum_rows + sum_cols + sum_diag);					//print the sum of all rows, cols,diags.
}
void decipher(char str[]) {
	int idx = 0;							//index of the char in the string.
	int cnt = 0;							//the number we need to subtract.
	char tmp;
	while (str[idx] != '\0') {
		if (str[idx] == 32) {
			printf("%c",' ');
			idx++;
			cnt = 0;
			continue;
		}
		else {
			tmp = (str[idx] - (cnt + 1));
			printf("%c", tmp);
			str[idx] = tmp;
			idx++;
			cnt++;
		}
	}
}
void rotateMatrix90(int mat[][3], int rows, int cols) {
	int tmp;
	tmp = mat[0][0];
	mat[0][0] = mat[2][0];
	mat[2][0] = mat[2][2];
	mat[2][2] = mat[0][2];
	mat[0][2] = tmp;
	tmp = mat[0][1];
	mat[0][1] = mat[1][0];
	mat[1][0] = mat[2][1];
	mat[2][1] = mat[1][2];
	mat[1][2] = tmp;
}
void FindSimilarWords(char str[]) {
	int str_idx , start1 = 0, start2 = 0, end1 = 0, end2, len = 0;
	while (str[len] != '\0')
		len++;
	while (start1 < len)
	{
		for (int i = start1; i < len; i++)
		{
			if (str[i] == ' ')
			{
				end1 = i - 1;
				start2 = i + 1;
				break;
			}
		}
		str_idx = start2;
		while (str_idx < len+1)
		{
			if ((str[str_idx] == ' ') || (str[str_idx] == '\0'))
			{
				end2 = str_idx - 1;
				IsFamiliar(str, start1, end1, start2, end2);
				start2 = str_idx + 1;
			}
			str_idx++;
		}
		while ((str[start1] != ' ') || (str[start1] == '\0'))
			start1++;
		start1++;
	}
}
void IsFamiliar(char str[], int start1, int end1, int start2, int end2)
{
	int letter_found = 0, stop = 0;
	char lowercase_letter1, lowercase_letter2;
	for (int i = start1; i <= end1; i++)
	{
		if ((str[i] > 64) && (str[i] < 91)) {
			lowercase_letter1 = str[i] + 32;
		}
		else {
			lowercase_letter1 = str[i];
		}
		for (int j = start2; j <= end2; j++)
		{
			if ((str[j] > 64) && (str[j] < 91)) {
				lowercase_letter2 = str[j] + 32;
			}
			else {
				lowercase_letter2 = str[j];
			}
			if (lowercase_letter1 == lowercase_letter2) {
				letter_found = 1;
				break;
			}
		}
		if (letter_found == 0)
		{
			stop = 1;
			break;
		}
		letter_found = 0;
	}
	if (stop == 0)
	{
		for (int i = start2; i <= end2; i++)
		{
			if ((str[i] > 64) && (str[i] < 91)) {
				lowercase_letter1 = str[i] + 32;
			}
			else {
				lowercase_letter1 = str[i];
			}
			for (int j = start1; j <= end1; j++)
			{
				if ((str[j] > 64) && (str[j] < 91)) {
					lowercase_letter2 = str[j] + 32;
				}
				else {
					lowercase_letter2 = str[j];
				}
				if (lowercase_letter1 == lowercase_letter2) {
					letter_found = 1;
					break;
				}
			}
			if (letter_found == 0)
			{
				stop = 1;
				break;
			}
			letter_found = 0;
		}
	}
	if (stop == 0) {
		for (int i = start1; i <= end1; i++) printf("%c", str[i]);
		printf(" ");
		for (int i = start2; i <= end2; i++) printf("%c", str[i]);
		printf("\n");

	}
}
