/*Programmers: Kyriakou Georgia AEM: 02901, Kosmatou Andromachi AEM: 02822
 * Date: 10/03/2019
 * Programma gia tin diaxeirisi tou mitroou foititwn tou tmimatos, opou ola ta dedomena vriskontai stin mnimi tou ypologisth(epektasi).
 */
#include<stdio.h>
#include<string.h>
#include<time.h>
#include <stdlib.h>
#include <ctype.h>
#define NAME_SIZE 64
#define FOUND 0
#define NOTFOUND 1

typedef struct listOfLessons{
	unsigned short int code_less;
	struct listOfLessons *nxtLess;
} lessonsT;

typedef struct student {
	long unsigned int aem;
	char name[64];
	short unsigned int NumOfLes;
	lessonsT *rootLess;
	struct student *nxt_stud;
	struct student *prev_stud;
	int sizeBucket;
} studentT;

typedef struct hashStruct{
	int buckets;
	studentT **hashTable;	
	int reg;
}hashT;

typedef struct comparisons{
	studentT* position; 
	long unsigned int comps; //apothikevei tis sygkriseis pou ginontai stin linear_search kai tin voithitiki_find
} compT;

//synartisi pou metatrepei ta onomata apo mikra se kefalaia
char* uppercase(char nameStud[NAME_SIZE]){
	int i;
	
	for (i=0; nameStud[i]!='\0'; i++)
		nameStud[i] = toupper(nameStud[i]);

	return(nameStud);
}
//dosmeni synartisi pou tha xreiastei stin hashFunction
unsigned long hash(char *str){
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}
//synartisi pou vriskei tin thesi pou antistoixei to onoma ston pinaka
int hashFunction(char nameStud[NAME_SIZE], hashT hashStruct){
	int pos;
	long unsigned hash_value = hash(nameStud);
	
	pos = hash_value % hashStruct.buckets;
	return pos;
}
//synartisi pou apodesmevei tin mnimi gia tin apli lista mathimatwn
lessonsT* quitLess(lessonsT *rootLess){
	
	lessonsT *currLess, *prevLess;
	
	while(rootLess->nxtLess != rootLess) {
		for(prevLess = rootLess, currLess = rootLess->nxtLess; currLess->nxtLess != rootLess;prevLess = currLess, currLess = currLess->nxtLess);
		prevLess->nxtLess = currLess->nxtLess;
		free(currLess);
		currLess = NULL;
	}
	return rootLess;
}
//synartisi pou apodesmevei tin mnimi gia tin dipli lista mathitwn
hashT clear(hashT hashStruct){
	studentT *currStud, *rootStud;
	int i;
	
	for(i=0; i<hashStruct.buckets; i++){
		rootStud=hashStruct.hashTable[i];
		
		while((rootStud->nxt_stud!=rootStud) && (rootStud->prev_stud != rootStud)){
			for(currStud = rootStud; currStud->nxt_stud != rootStud;currStud = currStud->nxt_stud);
			quitLess(currStud->rootLess);
			currStud->prev_stud->nxt_stud=rootStud;
			rootStud->prev_stud=currStud->prev_stud;
			free(currStud);
		}
		hashStruct.hashTable[i] = NULL;
	}
	return hashStruct;
}
//synartisi pou apodesmevei tin mnimi gia ton pinaka hash
hashT quit(hashT hashStruct){
	hashStruct = clear(hashStruct);
	int i;
	for(i=hashStruct.buckets-1;i>=0;i--){
		free(hashStruct.hashTable[i]);//apodesmevoume ti mnimi olwn twn root
	}
	free(hashStruct.hashTable);
	
	hashStruct.hashTable = NULL;
	return hashStruct;
}
//synartisi pou arxikopoiei tin apli kykliki lista mathimatwn me termatiko komvo
lessonsT* init_less_list(lessonsT *rootLess){
	rootLess = (lessonsT *)malloc(sizeof(lessonsT));//sentinel
	rootLess->nxtLess=rootLess;
	return(rootLess);
}
//synartisi pou arxikopoiei tin dipli kykliki lista mathitwn me termatiko komvo
studentT* init_stud_list(studentT *rootStud){
	
	rootStud = (studentT *)malloc(sizeof(studentT));//sentinel
	rootStud->sizeBucket = 0;
	rootStud->nxt_stud = rootStud;
	rootStud->prev_stud = rootStud;
	rootStud->rootLess = init_less_list(rootStud->rootLess);
	return rootStud;
	
}
//synartisi pou arxikopoiei ton hash pinaka kai ta stoixeia tou
hashT initHashStruct(hashT hashStruct){
	int i;
	for (i=0; i<hashStruct.buckets; i++)
		hashStruct.hashTable[i] = init_stud_list(hashStruct.hashTable[i]);
	
	hashStruct.reg = 0;
	return(hashStruct);
}
//anazitisi me vasi to aem
int search_aem(hashT hashStruct, long unsigned aemStud){
	studentT *currStud, *rootStud;
	int i;

	for(i=0;i<hashStruct.buckets;i++){
		rootStud=hashStruct.hashTable[i];
		rootStud->aem = aemStud;//sentinel
	
		for (currStud=rootStud->nxt_stud; (currStud->aem != aemStud); currStud=currStud->nxt_stud);
		if(currStud != rootStud)
			return (FOUND);
	}
	return (NOTFOUND);
}
//anazitisi me vasi to onoma
compT search_name(hashT hashStruct, char nameStud[NAME_SIZE], compT compStruct){
	studentT *currStud, *rootStud;
	int pos, counter;
	
	pos = hashFunction(nameStud, hashStruct);
	rootStud=hashStruct.hashTable[pos];
	strcpy(rootStud->name, nameStud);
	
	for(counter = 1, currStud=rootStud->nxt_stud; strcmp(currStud->name, nameStud)<0; counter++, currStud = currStud->nxt_stud);
	
	compStruct.comps = counter;
	
	if ((strcmp(currStud->name, nameStud) == 0)&&(currStud !=rootStud)){
		compStruct.position = currStud;
	}
	else{
		compStruct.position = NULL;
	}
		return compStruct;
}
//synartisi pou eisagei i oxi enan mathiti stin lista
hashT addStud(hashT hashStruct, char nameStud[NAME_SIZE], long unsigned aemStud, short unsigned lesStud, compT compStruct){

	studentT *currStud, *posStud, *rootStud;
	int pos;
	
	if((hashStruct.reg>0)&&(search_aem(hashStruct, aemStud) == FOUND)){
		printf("\nA-NOK %lu\n", aemStud);
		return (hashStruct);
	}
	compStruct = search_name(hashStruct, nameStud, compStruct);
	if((hashStruct.reg>0)&&(compStruct.position != NULL)){
		printf("\nA-NOK %s\n", nameStud);
		return (hashStruct);
	}
	else {
		++hashStruct.reg;
		pos = hashFunction(nameStud, hashStruct);
		rootStud = hashStruct.hashTable[pos];
		if(rootStud->sizeBucket == 0){
			(rootStud->sizeBucket)++;
			rootStud->nxt_stud = (studentT*)malloc(sizeof(studentT));
			strcpy(rootStud ->nxt_stud-> name, nameStud);
			rootStud->nxt_stud->aem = aemStud;
			rootStud->nxt_stud->NumOfLes = lesStud;
			
			rootStud->nxt_stud->rootLess = init_less_list(rootStud->nxt_stud->rootLess);
			
			rootStud->prev_stud = rootStud->nxt_stud;
			rootStud->nxt_stud->prev_stud = rootStud;
			rootStud->prev_stud->nxt_stud = rootStud;
		}
		else{
			rootStud=hashStruct.hashTable[pos];
			++(rootStud->sizeBucket);
			for(posStud = rootStud, currStud = rootStud->nxt_stud; strcmp(currStud->name, nameStud) < 0;posStud = currStud, currStud = currStud ->nxt_stud);
			 
			currStud = (studentT*)malloc(sizeof(studentT));
			strcpy(currStud -> name, nameStud);
			currStud->aem = aemStud;
			currStud->NumOfLes = lesStud;
			
			currStud->rootLess = init_less_list(currStud->rootLess);
			
			currStud->nxt_stud=posStud->nxt_stud;
			currStud->prev_stud=posStud;
			currStud->nxt_stud->prev_stud=currStud;
			currStud->prev_stud->nxt_stud=currStud;
			
		}
	}

	return hashStruct;
}
hashT addPrint(hashT hashStruct, char nameStud[NAME_SIZE], long unsigned aemStud, short unsigned lesStud, compT compStruct){
	studentT *currStud, *posStud, *rootStud;
	int pos;
	
	if((hashStruct.reg>0)&&(search_aem(hashStruct, aemStud) == FOUND)){
		printf("\nA-NOK %lu\n", aemStud);
		return (hashStruct);
	}
	compStruct = search_name(hashStruct, nameStud, compStruct);
	if((hashStruct.reg>0)&&(compStruct.position != NULL)){
		printf("\nA-NOK %s\n", nameStud);
		return (hashStruct);
	}
	else {
		++hashStruct.reg;
		pos = hashFunction(nameStud, hashStruct);
		rootStud = hashStruct.hashTable[pos];
		if(rootStud->sizeBucket == 0){
			(rootStud->sizeBucket)++;
			rootStud->nxt_stud = (studentT*)malloc(sizeof(studentT));
			strcpy(rootStud ->nxt_stud-> name, nameStud);
			rootStud->nxt_stud->aem = aemStud;
			rootStud->nxt_stud->NumOfLes = lesStud;
			
			rootStud->nxt_stud->rootLess = init_less_list(rootStud->nxt_stud->rootLess);
			
			rootStud->prev_stud = rootStud->nxt_stud;
			rootStud->nxt_stud->prev_stud = rootStud;
			rootStud->prev_stud->nxt_stud = rootStud;
		}
		else{
			rootStud=hashStruct.hashTable[pos];
			++(rootStud->sizeBucket);
			for(posStud = rootStud, currStud = rootStud->nxt_stud; strcmp(currStud->name, nameStud) < 0;posStud = currStud, currStud = currStud ->nxt_stud);
			 
			currStud = (studentT*)malloc(sizeof(studentT));
			strcpy(currStud -> name, nameStud);
			currStud->aem = aemStud;
			currStud->NumOfLes = lesStud;
			
			currStud->rootLess = init_less_list(currStud->rootLess);
			
			currStud->nxt_stud=posStud->nxt_stud;
			currStud->prev_stud=posStud;
			currStud->nxt_stud->prev_stud=currStud;
			currStud->prev_stud->nxt_stud=currStud;
			
		}
		printf("\nA-OK\n");
	}

	return hashStruct;
	
}
//synartisi pou diagrafei i oxi enan mathiti apo tin lista
hashT rmvStud(hashT hashStruct, char nameStud[NAME_SIZE], compT compStruct){
	studentT *currStud, *rootStud;
	int pos;
	pos = hashFunction(nameStud, hashStruct);
	rootStud=hashStruct.hashTable[pos];
	
	compStruct = search_name(hashStruct, nameStud, compStruct);
	currStud = compStruct.position;
	if(currStud != NULL){
		quitLess(currStud->rootLess);
		currStud->nxt_stud->prev_stud = currStud->prev_stud;
		currStud->prev_stud->nxt_stud = currStud->nxt_stud;
		free(currStud);
		--(hashStruct.reg);
		--(rootStud->sizeBucket);
		printf("\nR-OK\n");
	}
	else
		printf("\nR-NOK %s\n", nameStud);
	return hashStruct;
}
//synartisi pou vriskei i oxi enan mathiti stin lista
void find(hashT hashStruct, char nameStud[NAME_SIZE], compT compStruct){
	studentT *currStud;
	
	clock_t start_t, end_t;
	double total_t;
	
	start_t=clock();
	
	compStruct = search_name(hashStruct, nameStud, compStruct);
	currStud = compStruct.position;
	
	if(currStud == NULL){
		printf("\nF-NOK %s\n", nameStud);
	}
	else{
		printf("\n#");
		printf("\nF-OK %lu %hu\n", currStud->aem, currStud->NumOfLes);
	}
	
	end_t=clock();
	
	total_t= (double)(end_t-start_t)/CLOCKS_PER_SEC;
	
	fprintf(stderr, "\n%lu,%lf\n", compStruct.comps, total_t);
}
//synartisi pou allazei ton arithmo xrostoumenwn mathimatwn i oxi apo enan mathiti stin lista
hashT modStud(hashT hashStruct, char nameStud[NAME_SIZE], short unsigned int NewLes){
	studentT* currStud, *rootStud;
	int pos;
	
	pos = hashFunction(nameStud, hashStruct);
	rootStud=hashStruct.hashTable[pos];
	
	strcpy(rootStud->name, nameStud);
	
	for(currStud=rootStud->nxt_stud; strcmp(currStud->name, nameStud)<0; currStud = currStud->nxt_stud);
	
	if((currStud != rootStud)&&(strcmp(currStud->name, nameStud)==0)){
		currStud->NumOfLes = NewLes;
		printf("\nM-OK\n");
	}
	else {
		printf("\nM-NOK %s\n", nameStud);
	}
	return hashStruct;
}
//synartisi pou vriskei i oxi ena mathima stin lista
int findLess(unsigned short code_less, lessonsT *rootLess){
	lessonsT *currLess;
	
	rootLess->code_less = code_less;
	for (currLess = rootLess->nxtLess; currLess->code_less < code_less; currLess = currLess->nxtLess);
	
	return((currLess != rootLess)&&(currLess->code_less==code_less));
}
//synartisi pou eisagei i oxi ena mathima stin lista
lessonsT* addLess(unsigned short code_less, lessonsT* rootLess){
	lessonsT *prevLess, *currLess;
	int found;
	
	found = findLess(code_less, rootLess);
	
	if(found == 0) { 
		for(prevLess=rootLess, currLess=rootLess->nxtLess; currLess->code_less < code_less; prevLess=currLess, currLess = currLess->nxtLess);
		
		currLess = (lessonsT *)malloc(sizeof(lessonsT));
		currLess->code_less=code_less;
		currLess->nxtLess = prevLess->nxtLess;
		prevLess->nxtLess = currLess;
		printf("\nG-OK\n");
	}
	else 
		printf("\nG-NOK %hu\n", code_less);
	return rootLess;
}
//synartisi pou diagrafei i oxi ena mathima apo tin lista
lessonsT* rmvLess(unsigned short code_less, lessonsT* rootLess){
	lessonsT *currLess, *prevLess;
	int found;
	
	found = findLess(code_less, rootLess);
	
	if(found != 0) {
		rootLess->code_less = code_less;
		for(prevLess=rootLess, currLess=rootLess->nxtLess; currLess->code_less < code_less; prevLess=currLess, currLess = currLess->nxtLess);
		
		if ((currLess != rootLess)&&(currLess->code_less == code_less)){
			prevLess->nxtLess = currLess->nxtLess;
			free(currLess);
		}
		printf("\nU-OK\n");
	}
	else
		printf("\nU-NOK %hu\n", code_less);
	return rootLess;
}
// synartisi eggrafhs mathiti se mathima
hashT reg(hashT hashStruct, char nameStud[NAME_SIZE], unsigned short code_less, compT compStruct){
	compStruct = search_name(hashStruct, nameStud, compStruct);

	studentT *currStud = compStruct.position;
	
	if (currStud == NULL){
		printf("\nG-NOK %s\n", nameStud);
	}
	else{
		currStud->rootLess = addLess(code_less, currStud->rootLess);
	}
	return hashStruct;
}
// synartisi diagrafhs mathiti apo mathima
hashT unreg(hashT hashStruct, char nameStud[NAME_SIZE], unsigned short code_less, compT compStruct){
	compStruct = search_name(hashStruct, nameStud, compStruct);

	studentT *currStud = compStruct.position;
	
	if (currStud == NULL){
		printf("\nU-NOK %s\n", nameStud);
	}
	else{
		currStud->rootLess = rmvLess(code_less, currStud->rootLess);
	}
	
	return hashStruct;
}
//anazitisi eggrafhs mathiti se mathima
void isreg(hashT hashStruct, char nameStud[NAME_SIZE], unsigned short code_less, compT compStruct){
	int foundLess;
	compStruct = search_name(hashStruct, nameStud, compStruct);
	studentT *currStud = compStruct.position;
	
	if (currStud == NULL)
		printf("\nI-NOK %s\n", nameStud);
	else{
		foundLess=findLess(code_less,currStud->rootLess);
		if(foundLess==0)
			printf("\nNO\n");
		else 
			printf("\nYES\n");
	}
}
// ektyposh listas mathimatwn
void list(hashT hashStruct, char nameStud[NAME_SIZE], compT compStruct){
	lessonsT *currLess;
	compStruct = search_name(hashStruct, nameStud, compStruct);

	studentT *currStud = compStruct.position;
	
	if (currStud == NULL)
		printf("\nL-NOK %s\n", nameStud);
	else{
		printf("\n#\n%s\n", nameStud);
		for (currLess=currStud->rootLess->nxtLess; currLess!=currStud->rootLess; currLess=currLess->nxtLess)
			printf("%hu\n", currLess->code_less);
	}
}
// anazitisi tou megalyterou bucket (megethos)
int find_largest_bucket(hashT hashStruct){
	int temp, i, largestSize;
	studentT* rootStud;
	rootStud= hashStruct.hashTable[0];
	largestSize = rootStud->sizeBucket;
	
	for(i=1;i<hashStruct.buckets;i++){
		rootStud= hashStruct.hashTable[i];
		temp = rootStud->sizeBucket;
		if(temp>largestSize){
			largestSize = temp;
		}
	}
	return largestSize;
}
// ektyposh olwn twn buckets tou hash table
void print(hashT hashStruct){
	int i;
	studentT *currStud, *rootStud;
	double load_factor = (double)(hashStruct.reg) / hashStruct.buckets;
	
	printf("\n#\n");
	printf("%d %d %.2lf %d", hashStruct.buckets, hashStruct.reg, load_factor, find_largest_bucket(hashStruct));
	
	for(i=0;i<hashStruct.buckets;i++){
		rootStud=hashStruct.hashTable[i];
		printf("\n%d %d", i, hashStruct.hashTable[i]->sizeBucket);
		for(currStud=rootStud->nxt_stud; currStud!=rootStud; currStud=currStud->nxt_stud){
			if((hashStruct.hashTable[i]->sizeBucket) != 0)
				printf(" [%lu %s %hu]", currStud->aem, currStud->name, currStud->NumOfLes);
		}
		printf("\n");
	}
		printf("\n");
}
// synartisi anakatakermatismou
hashT rehash(hashT hashStruct, float multiplier, compT compStruct){
	int i;
	studentT *currHash, *rootHash;
	hashT newStruct;
// 	lessonsT *currLess;
	newStruct.buckets=multiplier*hashStruct.buckets;
	newStruct.hashTable = (studentT**)malloc(newStruct.buckets*sizeof(studentT*));

	newStruct = initHashStruct(newStruct);
	
	for(i=0; i<hashStruct.buckets; i++){
		for(rootHash=hashStruct.hashTable[i], currHash=rootHash->nxt_stud;currHash!=rootHash;currHash = currHash->nxt_stud){
			newStruct = addStud(newStruct,currHash->name, currHash->aem, currHash->NumOfLes,compStruct);
		/*	for(currLess=currHash->rootLess->nxtLess; currLess != currHash->rootLess; currLess=currLess->nxtLess)
				currHash->rootLess= addLess(currHash->rootLess->code_less, currHash ->rootLess);*/
		}
	}
	hashStruct = quit(hashStruct);
	return newStruct;
}
int main(int argc, char* argv[]){
	
	studentT *rootStud = NULL; 
	rootStud = init_stud_list(rootStud);
	char choice, nameStud[NAME_SIZE];
	long unsigned int aemStud; 
	short unsigned int lesStud, NewLes, code_less;
	hashT hashStruct;
	compT compStruct;
	compStruct.position=0;
	compStruct.comps = 0;
	
	
	if(argc == 2){
		hashStruct.buckets = atoi(argv[1]);
		hashStruct.hashTable = (studentT**)malloc(hashStruct.buckets*sizeof(studentT*));
	}
	else{
		hashStruct.buckets = 8;
		hashStruct.hashTable = (studentT**)malloc(hashStruct.buckets*sizeof(studentT*));
	}
	hashStruct = initHashStruct(hashStruct);
	
	while(1){
		scanf(" %c", &choice);
		switch (choice) {
			case 'a':{
				scanf(" %lu %s %hu", &aemStud,nameStud, &lesStud);
				strcpy(nameStud, uppercase(nameStud));
				hashStruct = addPrint(hashStruct, nameStud, aemStud, lesStud, compStruct);
				
					if((double)(hashStruct.reg) / hashStruct.buckets>= 4)
						hashStruct = rehash(hashStruct, 2, compStruct);
				
				break;
			}
			case 'r':{
				scanf("%s", nameStud);
				strcpy(nameStud, uppercase(nameStud));
				hashStruct = rmvStud(hashStruct, nameStud, compStruct);
				if((double)(hashStruct.reg) / hashStruct.buckets== 1)
					hashStruct = rehash(hashStruct,(double)(1)/2, compStruct);
				break;
			}
			case 'm':{
				scanf(" %s %hu", nameStud, &NewLes);
				strcpy(nameStud, uppercase(nameStud));
				hashStruct = modStud(hashStruct, nameStud, NewLes);
				break;
			}
			case 'f':{
				scanf("%s", nameStud);
				strcpy(nameStud, uppercase(nameStud));
				find(hashStruct, nameStud, compStruct);
				break;
			}
			case 'u':{
				scanf(" %s %hu", nameStud, &code_less);
				strcpy(nameStud, uppercase(nameStud));
				hashStruct = unreg(hashStruct, nameStud, code_less, compStruct);
				break;
			}
			case 'i':{
				scanf(" %s %hu", nameStud, &code_less);
				strcpy(nameStud, uppercase(nameStud));
				isreg(hashStruct, nameStud, code_less, compStruct);
				break;
			}
			case 'l':{
				scanf(" %s", nameStud);
				strcpy(nameStud, uppercase(nameStud));
				list(hashStruct, nameStud, compStruct);
				break;
			}
			case 'c': {
				hashStruct = clear(hashStruct);
				hashStruct = initHashStruct(hashStruct);
				printf("\nC-OK\n");
				break;
			}
			case 'q':{
				hashStruct = quit(hashStruct);
				return 0;
			}
			case 'p': {
				print(hashStruct);
				break;
			}
			case 'g':{
				scanf(" %s %hu", nameStud, &code_less);
				strcpy(nameStud, uppercase(nameStud));
				hashStruct = reg(hashStruct, nameStud,code_less, compStruct);
				break;
			}
		}
	}
}
