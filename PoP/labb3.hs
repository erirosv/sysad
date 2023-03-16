
--- a) 
largest [] = error "ERROR"
largest [x] = x
largest (x:xs)= if x > largest xs
                 then x
                 else largest xs

---Är listan tom från början kan den inte ta ut något värde för det finns inget. När det finns 
---värden i listan kolla den först på den första i listan. Då är den det största värdet hittills. 
---Sen jämnför den med nästa värde. Är nästa värde mindre behåller den de nuvarande väret. 
---Är värdet större så byter den till de största värdet och sedan gör samma process tills 
---listan är genomsökt. 

--- b) 
palindrome :: String -> Bool
palindrome y | length y <= 1 = True
             | head y == last y = palindrome(init(tail y))
             | head y /= last y = False	 

--Funktionen tar ett ord som är lika och kollar om den kan vända den fram och tillbaka och se om 
--- ordet blir desamma som innan. Annars blir det False. 

--- c) 
takeEqual z [] = []
takeEqual z (x:xs) = if z == x
					 then x : takeEqual z xs
					 else takeEqual z xs

---Funktionen söker igenom listan man har gett för att se hur många av det värdet man la in finns i listan. 
 
--- d)  
takeSelected z y [] = []
takeSelected z y(x:xs) = if z y x
                         then x : takeSelected z y xs
					     else takeSelected z y xs					
						 
-- Funktionen tar ut värden om är större, lika med eller mindre än de värdet man har lagt in. 

-- Assignment 2 
merge [] [] = [] --- Slår man ihop två tomma listor blir den listan tom
merge (xs) [] = xs --- Slår man ihop 2 listor då det finns variablar i den ena så kommer den att synas.
merge [] (ys) = ys -- Samma som den andra
merge (x:xs) (y:ys) = if x <= y
                      then x : merge xs (y:ys)
                      else y : merge (x:xs) ys -- Slår ihop de båda listorna.

--- Mergesorten tar två listor och sätter ihop till en. Har en utav listorna variablar i och den anadra 
--- är tom så kommer den ena listan synas. 
					  
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

--- Här tar funktionen en lista och delar upp den i två styckan listor där den tar varannat tal och stoppar 
--- den i varsin lista. Och sedan slår den ihop de båda listorna till en då alla tal är i ordning.


