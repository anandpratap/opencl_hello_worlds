__kernel void hello_world(void) {
	size_t gid = get_global_id(0);
    printf("Global id: %lu\n", gid);
 }

