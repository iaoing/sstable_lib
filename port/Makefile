all: OBJ_FILE 

OBJ_FILE: 
	g++ -g -O0 -I. -I./include -std=c++0x -fno-builtin-memcmp -pthread -DOS_LINUX -DLEVELDB_PLATFORM_POSIX -DLEVELDB_ATOMIC_PRESENT -DSNAPPY -DNDEBUG -fPIC -c ./*.cc -I ../


clean:
	rm -f ./*.o ./*.d ./*exe ./*stackdump 
