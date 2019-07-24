all: OBJ_FILE TEST

OBJ_FILE: 
	g++ -g -O0 -I. -I./include -std=c++0x -fno-builtin-memcmp -pthread -DOS_LINUX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -DSNAPPY -DNDEBUG -fPIC -c ./*.cc -I ../


TEST: 
	g++ -o arena_test arena_test.o arena.o env_posix.o env.o status.o testutil.o testharness.o ../port/port_posix.o ../port/port_posix_sse.o -lpthread
	g++ -o coding_test coding_test.o coding.o env_posix.o env.o status.o testutil.o testharness.o ../port/port_posix.o ../port/port_posix_sse.o -lpthread
	g++ -o crc32c_test crc32c_test.o crc32c.o env_posix.o env.o status.o testutil.o testharness.o ../port/port_posix.o ../port/port_posix_sse.o -lpthread
	g++ -o env_test env_test.o env.o env_posix.o status.o testutil.o testharness.o ../port/port_posix.o ../port/port_posix_sse.o -lpthread
	g++ -o env_posix_test env_posix_test.o env_posix.o env.o status.o testutil.o testharness.o ../port/port_posix.o ../port/port_posix_sse.o -lpthread
	g++ -o filter_policy_test filter_policy_test.o filter_policy.o testharness.o env.o coding.o hash.o status.o env_posix.o ../port/port_posix_sse.o ../port/port_posix.o -lpthread 

clean:
	rm -f ./*.o ./*.d ./*exe ./*stackdump arena_test coding_test crc32c_test env_test env_posix_test filter_policy_test
