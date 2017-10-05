ALL THE FOLLOWING COMMANDS ARE SUPPOSED TO BE RUN AT THE exact/ directory

Compile:

g++ *.cpp -std=c++11 -o vc

Running:

./vc ../graph50/graph50-01.txt 29

where:

	"../graph50/graph50-01.txt" is the file containing the graph structure.

	"29" is the desired number of vertices in the cover

	note: the example graph has cover of 30 vertices, hence, the result of the command output will be false.

more test files can be found at the following folders:

	../graph100/ (containing graphs with 100 vertices)

	../graph50/ (containing graphs with 50 vertices)

	../test_files/ which contains an unary tree with 4 vertices (test.mis), a star graph with 6 vertices (star.mis) and a graph that cannot be solved using greedy algorithms (greedy.mis)
