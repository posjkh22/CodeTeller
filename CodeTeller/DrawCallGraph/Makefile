
output 			= 	draw_callgraph
output_bitcode		=	testcase.bc
target_code		=	testcase5-6.c

run:
	./$(output) $(output_bitcode)

#dot -Tpng graph.dot > output.png

all: clean foo
	clang++ -std=c++11 main.cpp -o $(output) \
	`llvm-config --cxxflags --libs --ldflags --system-libs`

foo:
	clang -g -S -emit-llvm -o $(output_bitcode) -c $(target_code)

clean:
	rm -f $(output) *.ll *.bc *.dot *.png
