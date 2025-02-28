let rec inserer (l : int list) (x : int) =
        match l with 
        |[] -> x::[]
        |e::s -> if ( x > e ) then e::inserer s x else x::l
