(deftemplate configurazione "Stato del gioco"                    ; Template per la configurazione dello "stato del gioco"
	(slot agricoltore (type STRING) 
			     (default "sulla-riva-vicina"))
	(slot cavolo      (type STRING)
			     (default "sulla-riva-vicina"))
	(slot pecora      (type STRING) 
			     (default "sulla-riva-vicina"))
	(slot lupo        (type STRING) 
			     (default "sulla-riva-vicina")))

(deftemplate inizia "Configurazione iniziale"                    ; Template per la definizione della configurazione iniziale
	(slot config_iniziale           (type SYMBOL) 
				(default ACPL))) 




(defrule regola1                                                 ; Sequenza di 11 Regole attraverso le quali si passa
   ?indice_conf1 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina")           ; da uno stato del gioco ad un altro.
 	       	    (cavolo       "sulla-riva-vicina")
		    (pecora       "sulla-riva-vicina")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Trasborda la pecora" crlf)
(retract ?indice_conf1)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       		      (cavolo      "sulla-riva-vicina")
		      (pecora      "sulla-riva-lontana")
		      (lupo        "sulla-riva-vicina"))))


(defrule regola2 
   ?indice_conf2 <- 
   (configurazione  (agricoltore  "sulla-riva-lontana") 
 	       	    (cavolo       "sulla-riva-vicina")
	            (pecora       "sulla-riva-lontana")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Ritorna alla riva vicina" crlf)
(retract ?indice_conf2)
(assert(configurazione(agricoltore "sulla-riva-vicina") 
       		      (cavolo      "sulla-riva-vicina")
		      (pecora      "sulla-riva-lontana")
		      (lupo        "sulla-riva-vicina"))))


(defrule regola3 
   ?indice_conf3 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina") 
 	       	    (cavolo       "sulla-riva-vicina")
		    (pecora       "sulla-riva-lontana")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Trasborda il lupo" crlf)
(retract ?indice_conf3)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       		      (cavolo      "sulla-riva-vicina")
		      (pecora      "sulla-riva-lontana")
		      (lupo        "sulla-riva-lontana"))))

(defrule regola4 
   ?indice_conf4 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina") 
 	       	    (cavolo       "sulla-riva-vicina")
		    (pecora       "sulla-riva-lontana")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Trasborda il cavolo" crlf)
(retract ?indice_conf4)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       	              (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-lontana")
		      (lupo        "sulla-riva-vicina"))))


(defrule regola5 
   ?indice_conf5 <- 
   (configurazione  (agricoltore  "sulla-riva-lontana") 
 	       	    (cavolo       "sulla-riva-vicina")
		    (pecora       "sulla-riva-lontana")
		    (lupo         "sulla-riva-lontana"))
  =>
(printout t "Riporta indietro la pecora" crlf)
(retract ?indice_conf5)
(assert(configurazione(agricoltore "sulla-riva-vicina") 
       		      (cavolo      "sulla-riva-vicina")
		      (pecora      "sulla-riva-vicina")
		      (lupo        "sulla-riva-lontana"))))


(defrule regola6 
   ?indice_conf6 <- 
   (configurazione  (agricoltore  "sulla-riva-lontana") 
 	       	    (cavolo       "sulla-riva-lontana")
		    (pecora       "sulla-riva-lontana")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Riporta indietro la pecora" crlf)
(retract ?indice_conf6)
(assert(configurazione(agricoltore "sulla-riva-vicina") 
       		      (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-vicina")
		      (lupo        "sulla-riva-vicina"))))


(defrule regola7 
   ?indice_conf7 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina") 
 	       	    (cavolo       "sulla-riva-vicina")
		    (pecora       "sulla-riva-vicina")
		    (lupo         "sulla-riva-lontana"))
  =>
(printout t "Trasborda il cavolo" crlf)
(retract ?indice_conf7)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       		      (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-vicina")
		      (lupo        "sulla-riva-lontana"))))


(defrule regola8 
   ?indice_conf8 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina") 
 	       	    (cavolo       "sulla-riva-lontana")
	            (pecora       "sulla-riva-vicina")
		    (lupo         "sulla-riva-vicina"))
  =>
(printout t "Trasborda il lupo" crlf)
(retract ?indice_conf8)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       		      (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-vicina")
		      (lupo        "sulla-riva-lontana"))))


(defrule regola9 
   ?indice_conf9 <- 
   (configurazione  (agricoltore  "sulla-riva-lontana") 
 	       	    (cavolo       "sulla-riva-lontana")
		    (pecora       "sulla-riva-vicina")
		    (lupo         "sulla-riva-lontana"))
  =>
(printout t "Ritorna alla riva vicina" crlf)
(retract ?indice_conf9)
(assert(configurazione(agricoltore "sulla-riva-vicina") 
       		      (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-vicina")
		      (lupo        "sulla-riva-lontana"))))


(defrule regola10 
   ?indice_conf10 <- 
   (configurazione  (agricoltore  "sulla-riva-vicina") 
 	       	    (cavolo       "sulla-riva-lontana")
		    (pecora       "sulla-riva-vicina")
		    (lupo         "sulla-riva-lontana"))
  =>
(printout t "Trasborda la pecora" crlf)
(retract ?indice_conf10)
(assert(configurazione(agricoltore "sulla-riva-lontana") 
       		      (cavolo      "sulla-riva-lontana")
		      (pecora      "sulla-riva-lontana")
		      (lupo        "sulla-riva-lontana"))))

(defrule regola11                                                ; La regola 11 indica che non vi sono piu' regola aplicabili
    ?indice_conf11 <-
   (configurazione (agricoltore "sulla-riva-lontana")
                   (cavolo      "sulla-riva-lontana")
                   (pecora      "sulla-riva-lontana")
                   (lupo        "sulla-riva-lontana"))

=>
(printout t "Fine." crlf crlf))





(defrule chiedi_configurazione_iniziale                          ; Regola con priorità maggiore che permette l'inserimento
	(declare (salience 10000))                               ; in input dei dati che rappresenteranno la configurazione iniziale
 =>                                    

(printout t crlf"**************************" crlf crlf
           "Inserisci la configurazione iniziale:" crlf crlf)
(printout t "L'agricoltore è sulla riva vicina? (S/N): ")
(bind ?agric (read)) 
(if (eq ?agric S) then (bind ?temp_a A) else (bind ?temp_a _))
(printout t "Il cavolo è sulla riva vicina?     (S/N): ")
(bind ?cav (read))
(if (eq ?cav S) then (bind ?temp_c C) else (bind ?temp_c _))
(printout t "La pecora è sulla riva vicina?     (S/N): ")
(bind ?pec (read))
(if (eq ?pec S) then (bind ?temp_p P) else (bind ?temp_p _))
(printout t "Il lupo è sulla riva vicina?       (S/N): ")
(bind ?lup (read))
(if (eq ?lup S) then (bind ?temp_l L) else (bind ?temp_l _))
(bind ?config (sym-cat ?temp_a ?temp_c ?temp_p ?temp_l))
(assert (inizia (config_iniziale ?config)))
(assert (config inesatta))  ;Inesatta
(printout t crlf crlf))

(defrule controlla_input                                         ; Regola per il controllo sull'esattezza o la legalità della configurazione
  (declare (salience 9000))                                      ; inserita nel precedente passo.
  ?indice <- (inizia (config_iniziale ?config))
  (stato ?config) ;CONTROLLARE ATTENTAMENTE
  ?ind <- (config inesatta) ;Riportare valori QUI  INESATTA

  
 =>

  (printout t crlf)

  (retract ?indice ?ind)
  (assert (config esatta)) 
  (if (eq ?config ACPL) 
    then (assert (configurazione))
    
    else (if (eq ?config ____) then 
    (assert (configurazione (agricoltore "sulla-riva-lontana")
			        (cavolo "sulla-riva-lontana") 
			        (pecora "sulla-riva-lontana") 
			        (lupo "sulla-riva-lontana")))) 
	else (if (eq ?config A_P_) then
    (assert (configurazione (agricoltore "sulla-riva-vicina")
                                (cavolo "sulla-riva-lontana")
                                (pecora "sulla-riva-vicina")
                                (lupo   "sulla-riva-lontana"))))
         else (if (eq ?config _C_L) then
    (assert (configurazione (agricoltore "sulla-riva-lontana")
                                (cavolo "sulla-riva-vicina")
                                (pecora "sulla-riva-lontana")
                                (lupo   "sulla-riva-vicina"))))

        else (if (eq ?config A_PL) then
    (assert (configurazione (agricoltore "sulla-riva-vicina")
                                (cavolo "sulla-riva-lontana")
                                (pecora "sulla-riva-vicina")
                                (lupo   "sulla-riva-vicina"))))
       else (if (eq ?config _C__) then
    (assert (configurazione (agricoltore "sulla-riva-lontana")
                                (cavolo "sulla-riva-vicina")
                                (pecora "sulla-riva-lontana")
                                (lupo   "sulla-riva-lontana"))))
         else (if (eq ?config AC_L) then
    (assert (configurazione (agricoltore "sulla-riva-vicina")
                                (cavolo "sulla-riva-vicina")
                                (pecora "sulla-riva-lontana")
                                (lupo   "sulla-riva-vicina"))))
        
        
          else (if (eq ?config ___L) then
    (assert (configurazione (agricoltore "sulla-riva-lontana")
                                (cavolo "sulla-riva-lontana")
                                (pecora "sulla-riva-lontana")
                                (lupo   "sulla-riva-vicina"))))
          else (if (eq ?config ACP_) then
    (assert (configurazione (agricoltore "sulla-riva-vicina")
                                (cavolo "sulla-riva-vicina")
                                (pecora "sulla-riva-vicina")
                                (lupo   "sulla-riva-lontana"))))
          
         else (if (eq ?config __P_) then
    (assert (configurazione (agricoltore "sulla-riva-lontana")
                                (cavolo "sulla-riva-lontana")
                                (pecora "sulla-riva-vicina")
                                (lupo   "sulla-riva-lontana"))))

 
)



  (printout t "Le mosse dell'agricoltore sono: " crlf crlf))









(defrule ritenta "Se la configurazione data in input è errata"          ; Regola che assegna alla variabile indice il valore
  (declare (salience 8500))                                             ; "inesatta", specificando che si è inserito uno stato
  ?indice <- (config inesatta)                                          ; non ammissibile e permette all'utente di riprovare. 
	
 =>
   
   


  (retract ?indice)
  (printout t crlf "La configurazione iniziale data in input risulta illegale!!!" crlf crlf crlf)
  (printout t "Vuoi riprovare? (S/N): ")
  (bind ?risposta (read))
  (if (eq ?risposta S) then (reset) 
	else (printout t crlf crlf"**************" crlf crlf)))


(defrule continua_gioco "L'utente vuole giocare ancora?"                ; Regola a priorità minore che permette all'utente di continuare
  (declare (salience -10000))                                           ; a giocare una volta che è stata data la soluzione.
  ?indice <- (config esatta)
 =>
  (retract ?indice)
  (printout t crlf crlf crlf "Vuoi giocare ancora? (S/N): ")
  (bind ?risposta(read))
  (if (eq ?risposta S) then (reset)
  	else (printout t crlf "******************" crlf crlf)))



(deffacts stati_legali  "Stati validi nel gioco"                                   ; Fatti che rappresentano gli stati legali del gioco.
(stato  ACPL)  ; Tutti i personaggi sono sulla riva 1 
(stato  ____)  ; Tutti i personaggi sono sulla riva 2
(stato  A_P_)  ; Agricoltore e pecora sulla riva 1 - Cavolo e Lupo sulla riva 2 
(stato  _C_L)  ; Cavolo e Lupo sulla riva 1
(stato  A_PL)  ; Agricoltore e pecora e lupo sulla riva 2
(stato  _C__)
(stato  AC_L)
(stato  ___L)
(stato  ACP_)
(stato  __P_))

