CC = g++

INCLUDE_DIR = include
UTIL_DIR = util

svalue_objs = $(UTIL_DIR)/svalue.o $(UTIL_DIR)/util.o
$(svalue_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

heap_objs = $(UTIL_DIR)/heap.o $(UTIL_DIR)/util.o $(svalue_objs)
$(heap_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heap_objs = $(UTIL_DIR)/test_heap.o $(heap_objs)
$(test_heap_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heap: $(test_heap_objs)
	$(CC) $^ -o $@

clean_test_heap:
	-rm -f $(test_heap_objs) test_heap

heart_beat_objs = $(UTIL_DIR)/heart_beat.o $(UTIL_DIR)/util.o
$(heart_beat_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heart_beat_objs = $(UTIL_DIR)/test_heart_beat.o $(heart_beat_objs)
$(test_heart_beat_objs): %.o : %.cpp $(INCLUDE_DIR)/*.h
	$(CC) -c $< -I $(INCLUDE_DIR) -o $@

test_heart_beat: $(test_heart_beat_objs) $(heap_objs)
	$(CC) $^ -o $@

clean_test_heart_beat:
	-rm -r $(test_heart_beat_objs) test_heart_beat

cleanall: clean_test_heap clean_test_heart_beat
	echo "cleanall"
