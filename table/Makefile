all: OBJ_FILE TEST

OBJ_FILE: 
	g++ -g -O0 -I. -I./include -std=c++0x -fno-builtin-memcmp -pthread -DOS_LINUX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -DSNAPPY -DNDEBUG -fPIC -c ./*.cc -I ../ -I../include


TEST: 
	g++ -o filter_block_test filter_block_test.o filter_block.o block.o block_builder.o ../src/iterator.o ../util/filter_policy.o ../util/testharness.o ../util/env.o ../util/coding.o ../util/hash.o ../util/logging.o ../util/status.o ../util/env_posix.o ../port/port_posix_sse.o ../port/port_posix.o -lpthread 


clean:
	rm -f ./*.o ./*.d ./*exe ./*stackdump filter_block_test

