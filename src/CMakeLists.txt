include_directories(. ds)

add_executable(ant
	main.cpp
	compiler.h
	debug.h
	expression.h expression.cpp
	facts.cpp
	functions.h functions.cpp
	global.h
	heuristic.h heuristic.cpp
	predicate.h predicate.cpp
	predicate_impl.h predicate_impl.cpp
	rules.h rules.cpp
	show.cpp
	values.h
	variables.h variables.cpp
	wm.h wm.cpp

	# Parser
	parser/cmdline.h
	parser/language.h
	parser/parser.h parser/parser.cpp
	parser/setblock.h parser/setblock.cpp
	parser/tokenizer.h parser/tokenizer.cpp

	# Strutture dati
	ds/array.h ds/array.cpp
	ds/heap.h

	# Algoritmi
	algorithms/common.h algorithms/common.cpp
	algorithms/bfs.h algorithms/bfs.cpp
	algorithms/dfs.h algorithms/dfs.cpp
	algorithms/astar.h algorithms/astar.cpp
	algorithms/hill.h algorithms/hill.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O2 -ftree-loop-linear -ftree-loop-im -Wall")
