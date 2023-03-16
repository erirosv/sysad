palindrome(L) :- reverse(L,L).

suits([diamonds, spades, hearts, clubs]).
ranks([knave, queen, king, ace]).

cards(X) :- X=c(S,R),
          suits(Xs), member(S,Xs),
		  ranks(Ys), member(R,Ys).

deck(Cs) :- findall(X,cards(X),(Cs)).

shuffle(C1,C2) :- append(X,Y,C1), length(X,8), length(Y,8),
		       random_permutation(X,X1),
		       random_permutation(Y,Y1),
			 polymerization(X1,Y1,C2).

polymerization([],[],[]).
polymerization([X|Xs],[Y|Ys],[X,Y|Zs]):- polymerization(Xs,Ys,Zs).

play(Cs,[]) :- deck(Cs), shuffle(Cs,C1), battle(C1,[]).

battle([],[]) :- write('Fuck yeah').
battle([],A) :- A \= [],write('GameOver').
battle([],Tc) :- length(Tc,N), N=4.
battle(Cs,Y) :- addcard(Cs,Y,Dw,Tc),
		    ranks(Tc,Tc,Cr),
		    suits(Cr,Cr,Sc),
		    battle(Dw,Sc).


addcard(Cs,Y,Dc,Dw) :- takecard(1,Cs,Tc),
		       dropcard(1,Cs,Dc),
		       append(Tc,Y,Dw).

takecard(_,[],[]).
takecard(0,_,[]).
takecard(N,[X|XS],[X|YS]) :- N1 is N-1, takecard(N1,XS,YS).

dropcard(_,[],[]).
dropcard(0,Xs,Xs).
dropcard(N,[_|XS],Y) :- N1 is N-1, dropcard(N1,XS,Y).


ranks(X,_,X) :- length(X,N), N<4.

ranks([X|_],[_,_,_,Y|Ys],Ys) :- X=c(_,V), Y=c(_,V),
	                         ranks(P), member(V,P).

ranks([X|Xs],[_,_,_,Y|_],[X|Xs]) :- X=c(_,V1), Y=c(_,V2), ranks(P),
	                          member(V1,P), member(V2,P), V1\=V2.

suits(X,_,X) :- length(X,N), N<4.

suits([X|_],[_,_,_,Y|Ys],[X,Y|Ys]) :- X=c(F,_), Y=c(F,_),
	                         suits(P), member(F,P).

suits([X|Xs],[_,_,_,Y|_],[X|Xs]) :- X=c(F1,_), Y=c(F2,_), suits(P),
	                          member(F1,P), member(F2,P), F1\=F2.







