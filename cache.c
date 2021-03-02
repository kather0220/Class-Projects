#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cache_impl.h"

typedef enum { false, true } bool; // boolean 타입을 쓰기 위한 enum 선언

/* get the global variables in other files(main.c) */
extern int num_cache_hits; // # of cache hits so far
extern int num_cache_misses; // # of cache misses so far

extern int num_bytes; // # of accessed bytes
extern int num_access_cycles; // # of clock cycles

extern int global_timestamp; // # of data access trials as timestamp

extern int value_returned;

char print_access[20];

/* new variables used in this file */
cache_entry_t cache_array[CACHE_SET_SIZE][DEFAULT_CACHE_ASSOC];
// data in cache with size (# of sets X # of associations
// 4X1 for direct, 2X2 for 2-way, 1X4 for fully
int memory_array[DEFAULT_MEMORY_SIZE_WORD];
// data in memory with size of 128 words

int set, tag;

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
void init_memory_content() {
	unsigned char sample_upward[16] = { 0x001, 0x012, 0x023, 0x034, 0x045, 0x056, 0x067, 0x078, 0x089, 0x09a, 0x0ab, 0x0bc, 0x0cd, 0x0de, 0x0ef };
	unsigned char sample_downward[16] = { 0x0fe, 0x0ed, 0x0dc, 0x0cb, 0x0ba, 0x0a9, 0x098, 0x087, 0x076, 0x065, 0x054, 0x043, 0x032, 0x021, 0x010 };
	int index, i = 0, j = 1, gap = 1;

	for (index = 0; index < DEFAULT_MEMORY_SIZE_WORD; index++) {
		memory_array[index] = (sample_upward[i] << 24) | (sample_upward[j] << 16) | (sample_downward[i] << 8) | (sample_downward[j]);
		if (++i >= 16)
			i = 0; // cycle
		if (++j >= 16)
			j = 0; // cycle

		if (i == 0 && j == i + gap) // difference of i and j == gap
			j = i + (++gap); // increases 1 gap and new j for each cycle

		printf("mem[%d] = %#x\n", index, memory_array[index]);
	}
}

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
void init_cache_content() {
	int i, j;

	for (i = 0; i < CACHE_SET_SIZE; i++) {
		for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
			cache_entry_t* pEntry = &cache_array[i][j];
			pEntry->valid = 0;
			pEntry->tag = -1;
			pEntry->timestamp = 0;
		}
	}
}

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
/* This function is a utility function to print all the cache entries. It will be useful for your debugging */
void print_cache_entries() {
	int i, j, k;

	for (i = 0; i < CACHE_SET_SIZE; i++) {
		printf("[Set %d] ", i);
		for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
			cache_entry_t* pEntry = &cache_array[i][j];
			printf("V: %d Tag: %#x Time: %d Data: ", pEntry->valid, pEntry->tag, pEntry->timestamp);
			for (k = 0; k < DEFAULT_CACHE_BLOCK_SIZE_BYTE; k++) {
				printf("%#x(%d) ", pEntry->data[k], k);
			}
			printf("\t");
		}
		printf("\n");
	}
}

/* This funcion is to return the data in cache */
int check_cache_data_hit(void* addr, char type) { // byte address와 type을 매개변수로 받는 함수

	// add this cache access cycle to global access cycle
	// check all entries in a set
	// if there is no data in cache, data is missed and return -1 
	// return -1 for missing

	int i; // iterator i
	int block_addr = (int)addr / DEFAULT_CACHE_BLOCK_SIZE_BYTE; // block address = bytes address / block size(=8)
	tag = block_addr / CACHE_SET_SIZE; // tag = block address / set size
	set = block_addr % CACHE_SET_SIZE; // set = block address % set size
	bool isDifferentTag = true; // tag가 다르면 true, 같으면 false

	num_access_cycles+=CACHE_ACCESS_CYCLE; // access cycles 더하라는데 왜하는건지 몰겟음

	for (i = 0; i < DEFAULT_CACHE_ASSOC; i++) { // set의 모든 entry를 확인하는 for문
		cache_entry_t* temp = &cache_array[set][i];
		if (temp->valid == 0) return -1; // valid가 0이면 데이터가 없는 거니까 MISS
		else { // valid가 1인 경우
			if (tag != temp->tag) isDifferentTag = true; // different tag인 경우 isDifferentTage = true
			else {
				isDifferentTag = false; // same tag인 경우 isDifferentTag = false
				temp->timestamp = global_timestamp++;
				break;
			}
		}
	}

	/* HIT이면 0, MISS면 -1 return */
	if (isDifferentTag == true) return -1; // different tag인 경우 MISS -> -1 return
	else return 0; //same tag인 경우 HIT -> 0 return
}

/* This function is to find the entry index in set for copying to cache */
int find_entry_index_in_set(int cache_index) {
	int entry_index = 0;

	/* Check if there exists any empty cache space by checking 'valid' */
	/* If the set has only 1 entry, return index 0 */
	/* Otherwise, searchfind the le over all entries to the least recently used entry by checking 'timestamp' */
	/* return the cache index for copying from memory */

	int i; // iterator
	int minTime = 0; // entry의 timestamp 중 가장 작은 값

	for (i = 0; i < DEFAULT_CACHE_ASSOC; i++) {
		if (DEFAULT_CACHE_ASSOC == 1) {
			entry_index = 0; // entry가 1개인 경우 entry index = 0
			break;
		}

		cache_entry_t* temp = &cache_array[set][i];
		if (temp->valid == 0) { // vaild가 0, 즉 해당 entry에 data가 없는 경우
			entry_index = i; // entry index는 해당 entry의 index
			break;
		}
		else { // vaild가 1인 경우 time stamp가 가장 작은 entry 찾기
			if (minTime > temp->timestamp) { // timestamp가 minTime보다 작으면
				minTime = temp->timestamp; // minTime 업데이트
				entry_index = i; // entry index는 해당 entry의 index
			}
		}
	}

	return entry_index;
}

/* This function is to return the data in main memory */
int access_memory(void* addr, char type) {
	int entry_index; // set 내에서 데이터가 입력될 entry 인덱스
	int word_address; // word address
	int offset; // word offset
	char data[11] = {0,}; // 16진수 정수를 문자열로 변환하여 저장할 배열 선언 및 초기화
	char target_data[11] = {0,}; // fetch 및 copy할 target data 문자열 선언 및 초기화
	int i; // for Iterator
	int index = 0; // data를 target data로 copy할 때 사용하는 인덱스 변수

	/* get the entry index by invoking find_entry_index_in_set() */
	entry_index = find_entry_index_in_set(addr);

	/* add this main memeory access cycle to global access cycle */
	num_access_cycles+=MEMORY_ACCESS_CYCLE;

	/* Fetch the data from the main memory and copy them to the cache */
	/* void *addr: addr is byte address, whereas your main memory address is word address due to 'int memory_array[]' */

	/* Fetch the data from the main memory */
	word_address = (int)addr / WORD_SIZE_BYTE; // word address = byte address / word size
	offset = (int)addr % WORD_SIZE_BYTE; // offset = byte address / word size

	sprintf(data, "0x%x", memory_array[word_address]); // word_adress에 위치한 데이터를 문자열 변환하여 data에 저장
	
	//printf("data: %s\n", data); //디버깅용 프린트문
	
	index = 9 - 2 * offset; // data를 target data로 복사할 때 사용할 배열 인덱스 변수
	target_data[0] = '0';
	target_data[1] = 'x';
	if (type == 'b') { // type이 byte인 경우 offset에 해당하는 byte만큼 target data로 복사
		target_data[2] = data[index - 1];
		target_data[3] = data[index];
	}
	else if (type == 'h') { // type이 half word인 경우 offset부터 2byte만큼 target data로 복사
		/* offset이 3인 경우 다음 word_address로 넘어가는 문제 다음과 같이 처리 */
		if (offset == 3) {
			target_data[4] = data[index - 1];
			target_data[5] = data[index];

			sprintf(data, "0x%x", memory_array[word_address + 1]); // data에 word_address의 다음 주소값을 가지는 word 값 저장

			target_data[2] = data[8];
			target_data[3] = data[9];
		}
		else {
			for (i = 0; i < 4; i++) {
				target_data[i + 2] = data[index - (3 - i)];
			}
		}
	}
	/* type이 word인 경우 word address에 위치한 word 전체를 target address에 복사 */
	else {
		/* offset이 0인 경우 word_address가 가리키는 데이터값 복사 */
		if(offset == 0)
			strcpy(target_data, data); // data를 target_data로 복사
		/* offset이 0이 아닌 경우 word_address의 다음 값으로 넘어가는 문제 아래와 같이 처리 */
		else {
			if (offset == 1) {
				for (i = 0; i < 6; i++) {
					target_data[i + 4] = data[index - (5 - i)];
				}
				sprintf(data, "0x%x", memory_array[word_address + 1]); // data에 word_address의 다음 주소값을 가지는 word 값 저장
				target_data[2] = data[8];
				target_data[3] = data[9];
			}
			else if (offset == 2) {
				for (i = 0; i < 4; i++) {
					target_data[i + 6] = data[index - (3 - i)];
				}
				sprintf(data, "0x%x", memory_array[word_address + 1]); // data에 word_address의 다음 주소값을 가지는 word 값 저장
				for (i = 0; i < 4; i++) { // 아래와 같은 의미
					target_data[i + 2] = data[i + 6];
				}
				/* 
				target_data[2] = data[6];
				target_data[3] = data[7];
				target_data[4] = data[8];
				target_data[5] = data[9];
				*/
			}
			else {
				for (i = 0; i < 2; i++) {
					target_data[i + 8] = data[index - (1 - i)];
				}
				sprintf(data, "0x%x", memory_array[word_address + 1]); // data에 word_address의 다음 주소값을 가지는 word 값 저장
				for (i = 0; i < 6; i++) { // 아래와 같은 의미
					target_data[i + 2] = data[i + 4];
				}
				/*
				target_data[2] = data[4];
				target_data[3] = data[5];
				target_data[4] = data[6];
				target_data[5] = data[7];
				target_data[6] = data[8];
				target_data[7] = data[9];
				*/
			}
		}
	}

	printf("access data: %s\n", target_data); // 디버깅용 프린트문
	strcpy(print_access, target_data);

	/* MISS인 경우에만 Copying to the cache */
	if(value_returned == -1){ 
		cache_entry_t* temp = &cache_array[set][entry_index];
		strcpy(temp->data, target_data); // target data를 cache에 복사
		temp->valid = 1; // 데이터를 복사했으니까 valid를 1로 업데이트
		temp->tag = tag;
		temp->timestamp = global_timestamp++; // time stamp 업데이트
		//printf("temp의 data: %s\n", temp->data); //디버깅용 프린트문
		//printf("%d %d의 vaild: %d\n", set, entry_index, temp->valid); //디버깅용 프린트문
	}
	/* Return the accessed data with a suitable type(b, h, or w) */
	// return -1 for unknown type

	return 0;
}