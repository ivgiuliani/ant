(defrule fine_gioco
	(declare (salience 1000))
	
	?ind <- (obiettivo nonraggiunto)	
	
  (cella 1 1 1)
  (cella 1 2 2)
  (cella 1 3 3)
  (cella 2 1 8)
  (cella 2 2 0)
  (cella 2 3 4)
  (cella 3 1 7)
  (cella 3 2 6)
  (cella 3 3 5) 

=>
	
	(retract ?ind)
	(printout t "gioco risolto!" crlf)	
)	 		 	

(defrule sposta_vuoto_dx
	(obiettivo nonraggiunto)
	?c1 <- (cella ?x ?y 0)
	(not (cella ?x 3 0))
	?c2 <- (cella ?x =(+ 1 ?y) ?val)
=>
	(printout t "Sposta vuoto a destra;" crlf)
	(retract ?c1 ?c2)
	(assert (cella ?x ?y ?val))
	(assert (cella ?x (+ ?y 1) 0))
)


(defrule sposta_vuoto_sx
	(obiettivo nonraggiunto)
	?c1 <- (cella ?x ?y 0)
	(not (cella ?x 1 0))
	?c2 <- (cella ?x =(- ?y 1) ?val)
=>
	(printout t "Sposta vuoto a sinistra;" crlf)
	(retract ?c1 ?c2)
	(assert (cella ?x ?y ?val))
	(assert (cella ?x (- ?y 1) 0))
)


(defrule sposta_vuoto_su
	(obiettivo nonraggiunto)
	?c1 <- (cella ?x ?y 0)
	(not (cella 1 ?y 0))
	?c2 <- (cella =(- ?x 1) ?y ?val)
=>
	(printout t "Sposta vuoto in su;" crlf)
	(retract ?c1 ?c2)
	(assert (cella ?x ?y ?val))
	(assert (cella (- ?x 1) ?y 0))
)


(defrule sposta_vuoto_giu
	(obiettivo nonraggiunto)
	?c1 <- (cella ?x ?y 0)
	(not (cella 3 ?y 0))
	?c2 <- (cella =(+ ?x 1) ?y ?val)
=>
	(printout t "Sposta vuoto in giu;" crlf)
	(retract ?c1 ?c2)
	(assert (cella ?x ?y ?val))
	(assert (cella (+ ?x 1) ?y 0))
)

(deffacts configurazioneiniziale
	(cella 1 1 0)
	(cella 1 2 1)
	(cella 1 3 3)
	(cella 2 1 8)
	(cella 2 2 2)
	(cella 2 3 4)
	(cella 3 1 7)
	(cella 3 2 6)
	(cella 3 3 5) 

	(obiettivo nonraggiunto)
)
