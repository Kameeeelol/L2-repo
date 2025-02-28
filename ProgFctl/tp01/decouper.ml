let rec decouper (e : 'a) (l : 'a list) : (list1 : *a , list2 : *a) = 
        match l with 
        |[] -> []
        |x::s -> if e>x then (x::decouper s, decouper s) else (decouper s, x::decouper s) 
