obj = LL1.o

LL1 : $(obj)
	g++ $(obj) -o LL1 
	./LL1
	
LL1.o: LL1.cpp
	g++ -c LL1.cpp -g

.PHONY: clean
clean :
	rm -rf $(obj)

