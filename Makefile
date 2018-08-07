CC = g++

INCLUDE_DIR = include
UTIL_DIR = util
TEST_DIR = test

svalue_objs = $(UTIL_DIR)/svalue.o $(UTIL_DIR)/util.o
$(svalue_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

heap_objs = $(UTIL_DIR)/heap.o $(UTIL_DIR)/util.o $(svalue_objs)
$(heap_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heap_objs = $(TEST_DIR)/test_heap.o $(heap_objs) 
$(test_heap_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heap: $(test_heap_objs)
	$(CC) $^ -o $@

clean_test_heap:
	-rm -r $(test_heap_objs) test_heap

hash_table_objs = $(UTIL_DIR)/hash_table.o $(UTIL_DIR)/util.o $(svalue_objs)
$(hash_table_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_hash_table_objs = $(TEST_DIR)/test_hash_table.o $(hash_table_objs)
$(test_hash_table_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_hash_table: $(test_hash_table_objs)
	$(CC) $^ -o $@

clean_test_hash_table:
	-rm -r $(test_hash_table_objs) test_hash_table

heart_beat_objs = $(UTIL_DIR)/heart_beat.o $(UTIL_DIR)/util.o
$(heart_beat_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heart_beat_objs = $(TEST_DIR)/test_heart_beat.o $(heart_beat_objs) 
$(test_heart_beat_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heart_beat: $(test_heart_beat_objs) $(heap_objs) $(hash_table_objs)
	$(CC) $^ -o $@

clean_test_heart_beat:
	-rm -r $(test_heart_beat_objs) test_heart_beat

mytest: $(TEST_DIR)/mytest.cpp
	$(CC) $(TEST_DIR)/mytest.cpp -o mytest

clean_test:
	-rm -r mytest

cleanall: clean_test clean_test_heap clean_test_heart_beat clean_test_hash_table
	echo "cleanall"
