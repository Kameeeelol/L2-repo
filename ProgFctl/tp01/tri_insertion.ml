let rec tri_insertion (l : int list) =
  match l with
  | [] -> []
  | [e] -> [e] 
  | e :: s -> inserer (tri_insertion s) e  

