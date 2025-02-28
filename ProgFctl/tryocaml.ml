(* Ceci est un éditeur pour OCaml
   Entrez votre programme ici, et envoyez-le au toplevel en utilisant le
   bouton "Évaluer le code" ci-dessous ou [Ctrl-e]. *)

                                               
                                               
type arbre = Vide | Noeud of int * arbre * arbre;;
let mon_arbre = Noeud(10, Noeud(5, Vide, Vide), Noeud(15, Vide, Vide));;

let rec appartient (x : int) (abr : arbre) : bool =
  match abr with 
  |Vide -> false
  |Noeud(n,g,d) -> if n = x then true else appartient x g || appartient x d 

  
let rec arbre_2_liste (abr : arbre) : int list =
  match abr with
  |Vide -> []
  |Noeud(n,g,d) -> arbre_2_liste g @ [n] @ arbre_2_liste d 
                     
  
    
let rec inserer_bas (n : int) (abr : arbre) : arbre =
  match abr with 
  | Vide -> Noeud(n, Vide, Vide)  
  | Noeud(x,g,d) ->
      if n < x then Noeud(x, inserer_bas n g, d)  
      else if n > x then Noeud(x, g, inserer_bas n d)  
      else abr

        
let rec couper (n : int) (abr : arbre) : arbre * arbre =
  match abr with 
  | Vide -> (Vide, Vide)  
  | Noeud(x, g, d) -> 
      if n < x then 
        let (a1, a2) = couper n g in
        (a1, Noeud(x, a2, d))  
      else if n > x then 
        let (a1, a2) = couper n d in
        (Noeud(x, g, a1), a2)  
      else (g, d) 
           
  
let rec inserer_haut (n : int) (abr : arbre) : arbre =
  match abr with 
  |Vide -> Noeud(n, Vide, Vide) 
  |Noeud(x,g,d) -> Noeud(n, fst(couper n abr), snd(couper n abr))
                     
                     
let rec construire_abr l abr =
  match l with
  | [] -> abr
  | e::s -> construire_abr s (inserer_bas e abr)
              
let rec triii (l : int list) : int list =
  arbre_2_liste (construire_abr l Vide)
           
           


