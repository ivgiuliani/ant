set
	start "startStateMedium";
	stop "endState";
	algo "astar";
	limit 10;
	heuristic "manhattan";
endSet

beginHeuristic manhattan
	define(h, 0);

	find(cella, x, y, 1);
	sub(x, 1);
	sub(y, 1);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 2);
	sub(x, 1);
	sub(y, 2);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 3);
	sub(x, 1);
	sub(y, 3);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 4);
	sub(x, 2);
	sub(y, 3);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 5);
	sub(x, 3);
	sub(y, 3);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 6);
	sub(x, 3);
	sub(y, 2);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 7);
	sub(x, 3);
	sub(y, 1);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 8);
	sub(x, 2);
	sub(y, 1);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	find(cella, x, y, 0);
	sub(x, 2);
	sub(y, 2);
	abs(x, x);
	abs(y, y);

	add(x, y);
	add(h, x);
	undefine(cella, x, y);

	return(h);
endHeuristic

beginHeuristic tasselli_mancanti
	define(h, 0);

	different("cella_1_1", 1, 1, 1, res);
	add(h, res);

	different("cella_1_2", 1, 2, 2, res);
	add(h, res);

	different("cella_1_3", 1, 3, 3, res);
	add(h, res);

	different("cella_2_1", 2, 1, 8, res);
	add(h, res);

	different("cella_2_2", 2, 2, 0, res);
	add(h, res);

	different("cella_2_3", 2, 3, 4, res);
	add(h, res);

	different("cella_3_1", 3, 1, 7, res);
	add(h, res);

	different("cella_3_2", 3, 2, 6, res);
	add(h, res);

	different("cella_3_3", 3, 3, 5, res)
	add(h, res);

	return(h);
endHeuristic

beginFact startStateHard
	cella_1_1 = [1, 1, 5];
	cella_1_2 = [1, 2, 4];
	cella_1_3 = [1, 3, 0];
	cella_2_1 = [2, 1, 6];
	cella_2_2 = [2, 2, 1];
	cella_2_3 = [2, 3, 8];
	cella_3_1 = [3, 1, 7];
	cella_3_2 = [3, 2, 3];
	cella_3_3 = [3, 3, 2];
endFact

beginFact startStateEasy
	cella_1_1 = [1, 1, 0];
	cella_1_2 = [1, 2, 1];
	cella_1_3 = [1, 3, 3];
	cella_2_1 = [2, 1, 8];
	cella_2_2 = [2, 2, 2];
	cella_2_3 = [2, 3, 4];
	cella_3_1 = [3, 1, 7];
	cella_3_2 = [3, 2, 6];
	cella_3_3 = [3, 3, 5];
endFact

beginFact startStateMedium
	cella_1_1 = [1, 1, 2];
	cella_1_2 = [1, 2, 8];
	cella_1_3 = [1, 3, 3];
	cella_2_1 = [2, 1, 7];
	cella_2_2 = [2, 2, 1];
	cella_2_3 = [2, 3, 4];
	cella_3_1 = [3, 1, 6];
	cella_3_2 = [3, 2, 0];
	cella_3_3 = [3, 3, 5];
endFact

beginFact endState
	cella_1_1 = [1, 1, 1];
	cella_1_2 = [1, 2, 2];
	cella_1_3 = [1, 3, 3];
	cella_2_1 = [2, 1, 8];
	cella_2_2 = [2, 2, 0];
	cella_2_3 = [2, 3, 4];
	cella_3_1 = [3, 1, 7];
	cella_3_2 = [3, 2, 6];
	cella_3_3 = [3, 3, 5];
endFact

beginRule move_empty_to_righty
	equals(cella_vuota, x, y, 0) and
	within(y, 1, 2)
	>
	define(nuova_y, y);
	add(nuova_y, 1);
	find(cella_orig, x, nuova_y, val_orig);
	edit(cella_orig, x, nuova_y, 0);
	edit(cella_vuota, x, y, val_orig);
endRule

beginRule move_empty_to_left
	equals(cella_vuota, x, y, 0) and
	within(y, 2, 3)
	>
	define(nuova_y, y);
	sub(nuova_y, 1);
	find(cella_orig, x, nuova_y, val_orig);
	edit(cella_orig, x, nuova_y, 0);
	edit(cella_vuota, x, y, val_orig);
endRule

beginRule move_empty_below
	equals(cella_vuota, x, y, 0) and
	within(x, 1, 2)
	>
	define(nuova_x, x);
	add(nuova_x, 1);
	find(cella_orig, nuova_x, y, val_orig);
	edit(cella_orig, nuova_x, y, 0);
	edit(cella_vuota, x, y, val_orig);
endRule

beginRule move_empty_above
	equals(cella_vuota, x, y, 0) and
	within(x, 2, 3)
	>
	define(nuova_x, x);
	sub(nuova_x, 1);
	find(cella_orig, nuova_x, y, val_orig);
	edit(cella_orig, nuova_x, y, 0);
	edit(cella_vuota, x, y, val_orig);
endRule

