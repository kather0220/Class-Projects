/*
 * main.c
 *
 * 20493-02 Computer Architecture
 * Term Project on Implentation of Cache Mechanism
 *
 * Skeleton Code Prepared by Prof. HyungJune Lee
 * Nov 13, 2017
 *
 */

#include <stdio.h>
#include "cache_impl.h"


/* hit ratio = (num_cache_hits / (num_cache_hits + num_cache_misses) */
int num_cache_hits = 0; // # of hits
int num_cache_misses = 0; // # of misses

/* bandwirdth = (num_bytes / num_access_cycles) */
int num_bytes = 0; // # of accessed bytes
int num_access_cycles = 0; // # of clock cycles

int global_timestamp = 0; // # of data access trials

int value_returned = -1; /* accessed data */

extern char print_access[20];

int retrieve_data(void* addr, char data_type) {
	//int value_returned = -1; /* accessed data */

	/* Invoke check_cache_data_hit() */
	value_returned = check_cache_data_hit(addr, data_type);
	// printf("%d\n", value_returned); //디버깅용 프린트문
	
	// HIT일 때도 target data 값을 알기위해 access_memory() 무조건 호출
	access_memory(addr, data_type);
	switch (data_type) {
	case 'w':
		num_bytes += 4;
		break;
	case 'h':
		num_bytes += 2;
		break;
	case 'b':
		num_bytes += 1;
		break;
	}
	if (value_returned == 0) num_cache_hits++;
	else num_cache_misses++;

	/* In case of the cache miss event, access the main memory by invoking access_memory() */



	return value_returned;
}

int main(void) {
	FILE* ifp = NULL, * ofp = NULL;
	unsigned long int access_addr; /* byte address (located at 1st column) in "access_input.txt" */
	char access_type; /* 'b'(byte), 'h'(halfword), or 'w'(word) (located at 2nd column) in "access_input.txt" */
	int accessed_data; /* This is the data that you want to retrieve first from cache, and then from memory */

	init_memory_content();
	init_cache_content();

	
	ifp = fopen("access_input2.txt", "r");
	if (ifp == NULL) {
		printf("Can't open input file\n");
		return -1;
	}
	ofp = fopen("access_output_test.txt", "w");
	if (ofp == NULL) {
		printf("Can't open output file\n");
		fclose(ifp);
		return -1;
	}
	fputs("[Accessed Data]\n", ofp);
	while (EOF != fscanf(ifp, "%ld", &access_addr)&& EOF != fscanf(ifp, "%c", &access_type)) {
		fscanf_s(ifp, "%ld", &access_addr);
		fscanf_s(ifp, "%c", &access_type);
		/*fputs(access_addr, ofp);
		fputs("\t", ofp);
		fputs(access_type, ofp);
		fputs("\t", ofp);
		printf("%ld\n", access_addr);
		printf("%c\n", access_type);
		fputs(retrieve_data(access_addr, access_type),ofp);
		fputs("\n", ofp);
		*/
		retrieve_data(access_addr, access_type);
		fprintf(ofp, "%d	%c	%s\n", access_addr, access_type, print_access);
		

	}
	fputs("--------------------------------------\n", ofp);

	switch (DEFAULT_CACHE_ASSOC) {
	case 1:
		fputs("[Direct mapped cache performance]\n", ofp);
		break;
	case 2:
		fputs("[2-way set associative cache performance]\n", ofp);
		break;
	case 4:
		fputs("[Fully associative cache performance]\n", ofp);
		break;
	}
	
	fprintf(ofp,"Hit ratio = %.2f (%d/%d)\n", (float)num_cache_hits / (num_cache_hits+num_cache_misses), num_cache_hits, num_cache_hits + num_cache_misses);
	fprintf(ofp, "Bandwidth = %.2f (%d/%d)\n", (float)num_bytes / num_access_cycles, num_bytes, num_access_cycles);

	

	/* Fill out here by invoking retrieve_data() */

	/*
	retrieve_data(334, 'b');
	retrieve_data(490, 'h');
	retrieve_data(329, 'w');
	retrieve_data(489, 'h');
	retrieve_data(338, 'h');
	retrieve_data(279, 'b');
	retrieve_data(148, 'w');
	retrieve_data(339, 'b');
	*/
	printf("hits: %d misses: %d\n", num_cache_hits, num_cache_misses); //디버깅용 프린트문
	printf("%d    %d", num_bytes, num_access_cycles);
	printf("%s\n", print_access);
	
	fclose(ifp);
	fclose(ofp);
	

	print_cache_entries();
	return 0;
}