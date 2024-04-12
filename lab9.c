#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType *pRecord;
	struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
	int res = 0;

	for (int i = 0; i < x; i++){
		res = (res + (res * 51) + i) % 2651959;
	}

	return res;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct HashType *cur = NULL;

	printf("Records:\n");

	for (i=0;i<hashSz;++i)
	{
		if (pHashArray[i].pRecord == NULL){
			continue;
		}
		
		printf("\tindex %d", i);
		
		cur = pHashArray + i;
		
		while (cur != NULL){
			printf(" -> %d, %c, %d", cur->pRecord->id, cur->pRecord->name, cur->pRecord->order);
			cur = cur->next;
		}

		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	int index = 0;
	struct HashType *prev = NULL;
	struct HashType *cur = NULL;
	
	struct HashType *ht = (struct HashType *) malloc(sizeof(struct HashType) * recordSz);

	for (int i = 0; i < recordSz; i++){
		ht[i].pRecord = NULL;
		ht[i].next = NULL;
	}
	
	for (int i = 0; i < recordSz; i++){
		index = hash(pRecords[i].id) % recordSz;
		if (ht[index].pRecord == NULL){
			ht[index].pRecord = pRecords + i;
			ht[index].next = NULL;
		}
		else{
			cur = ht + index;
			while (cur != NULL){
				prev = cur;
				cur = cur->next;
			}
			prev->next = (struct HashType *) malloc(sizeof(struct HashType));
			prev->next->pRecord = pRecords + i;
			prev->next->next = NULL;
		}
	}

	displayRecordsInHash(ht, recordSz);

	for (int i = 0; i < recordSz; i++){
		int j = 0;

		if (ht[i].pRecord == NULL){
			continue;
		}

		cur = ht + i;

		while (cur != NULL){
			prev = cur;
			cur = cur->next;
			if (j > 0){
				free(prev);
			}
			j++;
		}
	}

	free(pRecords);

	free(ht);
}