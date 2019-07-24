#include <iostream>
#include <vector>
#include <string>
#include <cinttypes>
#include <time.h>
#include <sys/time.h>
#include "table/table.h"
#include "table/table_builder.h"
#include "util/slice.h"
#include "util/env.h"
#include "util/status.h"


int num = 1000;
int val_size = 100;
uint64_t file_size;
std::string test_fname = std::string("./test.sst");



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

	// new builder;
	leveldb::Options op = leveldb::Options();
	leveldb::TableBuilder *builder = new leveldb::TableBuilder(op, file);

	srand(0x12345678);
	for (int i = 0; i < num; ++i)
	{
		std::string key = std::to_string(rand());
		std::string value = std::string(val_size, key[0]);
		builder->Add(key, value);
	}
	builder->Finish();
	file_size = builder->FileSize();

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

	// new table reader;
	leveldb::Options op = leveldb::Options();
	leveldb::Table *table;
	s = leveldb::Table::Open(op, file, file_size, &table);
	check_status(s);

	// read by iterator;
	int count = 0;
	leveldb::ReadOptions r_op = leveldb::ReadOptions();
	leveldb::Iterator *iter = table->NewIterator(r_op);
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
	delete table;
	delete file;
}


int main(int argc, char const *argv[])
{
	test_write();
	test_read();
	return 0;
}