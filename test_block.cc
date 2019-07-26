#include <iostream>
#include <vector>
#include <string>
#include <cinttypes>
#include <time.h>
#include <sys/time.h>
#include "table/block.h"
#include "table/block_builder.h"
#include "table/format.h"
#include "util/slice.h"
#include "util/env.h"
#include "util/status.h"
#include "leveldb/options.h"
#include "util/coding.h"


#ifndef kBlockTrailerSize
#define kBlockTrailerSize 5
#endif

int num = 100;
int val_size = 100;
uint64_t block_size;
std::string test_fname = std::string("./test.blk");



void check_status(leveldb::Status s){
	if(!s.ok()){
		std::cout << s.ToString() << std::endl;
		exit(0);
	}
}


void test_write(){
	leveldb::Status s;
	leveldb::Env *env = leveldb::Env::Default();

	// new writable file;
	leveldb::WritableFile *file;
	s = env->NewWritableFile(test_fname, &file);
	check_status(s);

	// new block builder;
	leveldb::Options op = leveldb::Options();
	leveldb::BlockBuilder *builder = new leveldb::BlockBuilder(&op);
	builder->Reset();

	srand(0x12345678);
	for (int i = 0; i < num; ++i)
	{
		std::string key = std::to_string(rand());
		std::string value = std::string(val_size, key[0]);
		builder->Add(key, value);
	}
	leveldb::Slice block = builder->Finish();
	block_size = block.size();

	// write block to file;
	s = file->Append(block);
	check_status(s);

	// append crc32c (no compression for block based file)
	// char trailer[kBlockTrailerSize];
 //    trailer[0] = leveldb::kNoCompression;
 //    uint32_t crc = leveldb::crc32c::Value(block.data(), block.size());
 //    crc = leveldb::crc32c::Extend(crc, trailer, 1);  // Extend crc to cover block type
 //    leveldb::EncodeFixed32(trailer+1, leveldb::crc32c::Mask(crc));
    // s = file->Append(leveldb::Slice(trailer, kBlockTrailerSize));
    s = file->Append(leveldb::RawBlockCrc(leveldb::kNoCompression, block));
    check_status(s);

	// close file;
	s = file->Close();
	check_status(s);

	delete builder;
	delete file;
}


void test_read(){
	leveldb::Status s;
	leveldb::Env *env = leveldb::Env::Default();

	// new writable file;
	leveldb::RandomAccessFile *file;
	s = env->NewRandomAccessFile(test_fname, &file);
	check_status(s);

	// create BlockContents;
	leveldb::ReadOptions r_op = leveldb::ReadOptions();
	leveldb::BlockContents contents;
	leveldb::BlockHandle handler;
	handler.set_offset(0);
	handler.set_size(block_size);
	s = leveldb::ReadBlock(file, r_op, handler, &contents);
	check_status(s);

	// read by iterator;
	int count = 0;
	leveldb::Block *block = new leveldb::Block(contents);
	leveldb::Iterator *iter = block->NewIterator(leveldb::Options().comparator);
	iter->SeekToFirst();
	while(iter->Valid()){
		std::string key = iter->key().data();
		std::string value = iter->value().data();
		if(key[0] != value[0]){
			std::cout << "Wrong value; key:[" << key << "]; value: [" << value << "]" << std::endl;
			break;
		}
		++count;
		iter->Next();
	}
	std::cout << "Total valid items: " << count << std::endl;

	delete iter;
	delete block;
	delete file;
}


int main(int argc, char const *argv[])
{
	test_write();
	test_read();
	return 0;
}