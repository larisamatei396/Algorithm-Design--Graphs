build:
	g++ minlexbfs.cpp -o minlexbfs
	g++ disjcnt.cpp -o disjcnt
	g++ revedges.cpp -o revedges
run-p1:
	./minlexbfs
run-p2:
	./disjcnt
run-p4:
	./revedges
clean:
	rm -f minlexbfs disjcnt revedges
