/*******************************************************************************
 * File: test_AM_insert5.c
 * Purpose: Test AM insert. This test creates enough data blocks to split the
 * index blocks, creating three layers of index blocks.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "../src/accessmethod/AM.h"
#include "../src/block/BL.h"
#include "../src/indexblock/IB.h"
#include "../src/filedesc/FD.h"
#include "../src/datablock/DB.h"
#include "../src/defn.h"
#include "../src/index/BT.h"

int main(void)
{
	srand(13153562);

	char file_name[32] = "database_insert5.db";
	int file_desc_AM;

	AM_Init();

	printf(">>>>>>>>>>>>>>>>>>>\n");
	printf("Creating file %s.\n", file_name);
	if (AM_CreateIndex(file_name, STRING, 124, STRING, 16) != AME_OK) {
		fprintf(stderr, "Error when creating file %s.\n", file_name);
		return 1;
	}
	printf("Created file %s.\n", file_name);
	printf(">>>>>>>>>>>>>>>>>>>\n");

	printf(">>>>>>>>>>>>>>>>>>>\n");
	printf("Opening file %s.\n", file_name);
	if ((file_desc_AM = AM_OpenIndex(file_name)) < 0) {
		fprintf(stderr, "Error when opening file %s.\n", file_name);
		return 1;
	}
	printf("Opened file %s.\n", file_name);
	printf(">>>>>>>>>>>>>>>>>>>\n");

	char key[124];
	char value[16] = "Macaron";

	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	void* key_ptr = &key;
	void* value_ptr = value;

	printf(">>>>>>>>>>>>>>>>>>>\n");
	printf("Inserting records..\n");
	size_t i;
	for (i = 0; i < 20; ++i) {
		key[0] = alphabet[rand() % 26];
		key[1] = alphabet[rand() % 26];
		key[2] = alphabet[rand() % 26];
		key[3] = '\0';
		printf("Inserting record %zu:\n", i);
		printf("Key: %s.\n", key);
		printf("Value: %s.\n", value);
		if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
			fprintf(stderr, "Failed to insert record.\n");
			exit(1);
		}
	}
	printf(">>>>>>>>>>>>>>>>>>>\n");

	key[0] = 'A';
	key[1] = 'A';
	key[2] = 'A';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	key[0] = 'A';
	key[1] = 'A';
	key[2] = 'B';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	key[0] = 'S';
	key[1] = 'N';
	key[2] = 'A';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	key[0] = 'S';
	key[1] = 'N';
	key[2] = 'B';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	key[0] = 'S';
	key[1] = 'F';
	key[2] = 'A';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	key[0] = 'S';
	key[1] = 'F';
	key[2] = 'B';
	key[3] = '\0';
	printf("Inserting record %zu:\n", i++);
	printf("Key: %s.\n", key);
	printf("Value: %s.\n", value);
	if (AM_InsertEntry(file_desc_AM, key_ptr, value_ptr) != AME_OK) {
		fprintf(stderr, "Failed to insert record.\n");
		exit(1);
	}

	printf(">>>>>>>>>>>>>>>>>>>\n");
	printf("Getting the index root block id to print it.\n");
	BF_Block* block;       // Used to get and print blocks.
	size_t index_root_id;  // The data block's id.

	CALL_FD(FD_Get_IndexRoot(file_desc_AM, &index_root_id));
	printf("Index root id: %ld.\n", index_root_id);

	size_t n_pointers;
	CALL_IB(IB_Get_MaxCountPointers(file_desc_AM, &n_pointers));
	printf("n_pointers: %ld.\n", n_pointers);

	CALL_BL(BL_LoadBlock(file_desc_AM, 1, &block));
	printf("Printing sublist.\n");
	CALL_DB(DB_Print_Sublist(file_desc_AM, block));
	CALL_BF(BF_UnpinBlock(block));
	BF_Block_Destroy(&block);

	int block_id;
	key[0] = 'B';
	key[1] = 'W';
	key[2] = 'P';
	key[3] = '\0';
	CALL_BT(BT_Get_SubtreeLeaf(file_desc_AM, index_root_id, key, &block_id));
	printf("%s exists in %d block.\n", key, block_id);

	key[0] = 'A';
	key[1] = 'A';
	key[2] = 'A';
	key[3] = '\0';
	CALL_BT(BT_Get_SubtreeLeaf(file_desc_AM, index_root_id, key, &block_id));
	printf("%s exists in %d block.\n", key, block_id);

	key[0] = 'S';
	key[1] = 'E';
	key[2] = 'Z';
	key[3] = '\0';
	CALL_BT(BT_Get_SubtreeLeaf(file_desc_AM, index_root_id, key, &block_id));
	printf("%s exists in %d block.\n", key, block_id);

	key[0] = 'L';
	key[1] = 'B';
	key[2] = 'C';
	key[3] = '\0';
	CALL_BT(BT_Get_SubtreeLeaf(file_desc_AM, index_root_id, key, &block_id));
	printf("%s exists in %d block.\n", key, block_id);

	key[0] = 'L';
	key[1] = 'B';
	key[2] = 'D';
	key[3] = '\0';
	CALL_BT(BT_Get_SubtreeLeaf(file_desc_AM, index_root_id, key, &block_id));
	printf("%s exists in %d block.\n", key, block_id);

	CALL_BL(BL_LoadBlock(file_desc_AM, 1, &block));
	printf("Printing the data block 1.\n");
	CALL_DB(DB_Print(file_desc_AM, block));
	CALL_BF(BF_UnpinBlock(block));
	BF_Block_Destroy(&block);

	CALL_BL(BL_LoadBlock(file_desc_AM, 2, &block));
	printf("Printing the data block 2.\n");
	CALL_DB(DB_Print(file_desc_AM, block));
	CALL_BF(BF_UnpinBlock(block));
	BF_Block_Destroy(&block);

	CALL_BL(BL_LoadBlock(file_desc_AM, 4, &block));
	printf("Printing the data block 4.\n");
	CALL_DB(DB_Print(file_desc_AM, block));
	CALL_BF(BF_UnpinBlock(block));
	BF_Block_Destroy(&block);

	CALL_BL(BL_LoadBlock(file_desc_AM, 3, &block));
	printf("Printing the index block 3.\n");
	CALL_IB(IB_Print(file_desc_AM, block));
	CALL_BF(BF_UnpinBlock(block));
	BF_Block_Destroy(&block);

	int file_desc_BF;
	CALL_FD(FD_Get_FileDesc(file_desc_AM, &file_desc_BF));
	int n_blocks;
	CALL_BF(BF_GetBlockCounter(file_desc_BF, &n_blocks));
	printf("n_blocks: %d.\n", n_blocks);

	printf(">>>>>>>>>>>>>>>>>>>\n");
	printf("Closing file %s.\n", file_name);
	if (AM_CloseIndex(file_desc_AM) != AME_OK) {
		fprintf(stderr, "Error when closing file %s.\n", file_name);
		return 1;
	}
	printf("Closed file %s.\n", file_name);
	printf(">>>>>>>>>>>>>>>>>>>\n");

	AM_Close();

	return 0;
}
