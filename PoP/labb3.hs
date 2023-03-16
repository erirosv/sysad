
--- a) 
largest [] = error "ERROR"
largest [x] = x
largest (x:xs)= if x > largest xs
                 then x
                 else largest xs

---�r listan tom fr�n b�rjan kan den inte ta ut n�got v�rde f�r det finns inget. N�r det finns 
---v�rden i listan kolla den f�rst p� den f�rsta i listan. D� �r den det st�rsta v�rdet hittills. 
---Sen j�mnf�r den med n�sta v�rde. �r n�sta v�rde mindre beh�ller den de nuvarande v�ret. 
---�r v�rdet st�rre s� byter den till de st�rsta v�rdet och sedan g�r samma process tills 
---listan �r genoms�kt. 

--- b) 
palindrome :: String -> Bool
palindrome y | length y <= 1 = True
             | head y == last y = palindrome(init(tail y))
             | head y /= last y = False	 

--Funktionen tar ett ord som �r lika och kollar om den kan v�nda den fram och tillbaka och se om 
--- ordet blir desamma som innan. Annars blir det False. 

--- c) 
takeEqual z [] = []
takeEqual z (x:xs) = if z == x
					 then x : takeEqual z xs
					 else takeEqual z xs

---Funktionen s�ker igenom listan man har gett f�r att se hur m�nga av det v�rdet man la in finns i listan. 
 
--- d)  
takeSelected z y [] = []
takeSelected z y(x:xs) = if z y x
                         then x : takeSelected z y xs
					     else takeSelected z y xs					
						 
-- Funktionen tar ut v�rden om �r st�rre, lika med eller mindre �n de v�rdet man har lagt in. 

-- Assignment 2 
merge [] [] = [] --- Sl�r man ihop tv� tomma listor blir den listan tom
merge (xs) [] = xs --- Sl�r man ihop 2 listor d� det finns variablar i den ena s� kommer den att synas.
merge [] (ys) = ys -- Samma som den andra
merge (x:xs) (y:ys) = if x <= y
                      then x : merge xs (y:ys)
                      else y : merge (x:xs) ys -- Sl�r ihop de b�da listorna.

--- Mergesorten tar tv� listor och s�tter ihop till en. Har en utav listorna variablar i och den anadra 
--- �r tom s� kommer den ena listan synas. 
					  
--- b)
splitList [] = ([], [])
splitList [x] = ([x], [])
splitList (x:y:xys) = (x:xs, y:ys)
           where (xs, ys) = splitList xys
		   
mergeSort :: Ord a => [a] -> [a]
mergeSort [] = []
mergeSort [x] = [x]
mergeSort xs = merge (mergeSort as) (mergeSort bs)
          where (as, bs) = splitList xs

--- H�r tar funktionen en lista och delar upp den i tv� styckan listor d�r den tar varannat tal och stoppar 
--- den i varsin lista. Och sedan sl�r den ihop de b�da listorna till en d� alla tal �r i ordning.


