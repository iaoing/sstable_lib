all: OBJ_FILE SH_LIB TEST

SH_LIB: 
	g++ -shared -o libtable.so \
	helpers/memenv/memenv.o \
	src/iterator.o src/options.o \
	port/port_posix.o port/port_posix_sse.o \
	table/block_builder.o table/filter_block.o table/format.o \
	table/table_builder.o table/two_level_iterator.o \
	table/block.o table/merger.o table/table.o util/arena.o \
	util/coding.o util/env.o util/filter_policy.o util/logging.o \
	util/cache.o util/comparator.o util/env_posix.o util/status.o \
	util/crc32c.o util/hash.o            

OBJ_FILE:
	cd ./helpers/memenv && $(MAKE) && cd -
	cd ./port && $(MAKE) && cd -
	cd ./util && $(MAKE) && cd -
	cd ./src && $(MAKE) && cd -
	cd ./table && $(MAKE) && cd -


TEST:
	g++ -c -O0 -g table_test.cc -I . -I./include -DOS_LINUX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -DSNAPPY
	g++ -o table_test ./table_test.o -L ./ -ltable -lsnappy -lpthread
# 	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`


clean:
	rm -f ./*.o ./*.d ./*exe ./*stackdump *.sst table_test* libtable.so
	cd ./helpers/memenv && $(MAKE) clean && cd -
	cd ./port && $(MAKE) clean && cd -
	cd ./util && $(MAKE) clean && cd -
	cd ./src && $(MAKE) clean && cd -
	cd ./table && $(MAKE) clean && cd -
