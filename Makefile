exe = terrible

CXXFLAGS += --std=c++14

$(exe):
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(exe).cc -o $(exe)

.PHONY: clean

clean:
	rm $(exe) -fv
