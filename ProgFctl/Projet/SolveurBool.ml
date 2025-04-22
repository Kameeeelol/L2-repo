type eb = X of int | Vrai | Faux | Et of eb * eb | Ou of eb * eb | Non of eb;;

let eq1 = (Ou (X 1, X 2), Vrai);;
let eq2 = (Ou (Et (X 1, Non (X 3)), Et (X 3, Non (X 1))), X 2);;
let eq3 = (Non (Et (X 1, Et(X 2, X 3))), Vrai);;



let rec collect expr =
    match expr with 
    | X i -> [i]
    | Vrai | Faux -> []
    | Et(x1,x2) | Ou(x1,x2) -> collect x1 @ collect x2
    | Non(x) -> collect x;;

let get_var equ =
    let all_vars = List.flatten (List.map (fun(x1,x2) -> collect x1 @ collect x2) equ) in List.sort_uniq compare all_vars;;

let env vars =
    let rec gen = function 
    | [] -> [[]]
    | e::l -> let rest = gen l in 
        List.concat_map (fun r -> [(e,true)::r ; (e,false)::r]) rest in gen vars;;

let rec eval expr env =
    match expr with
    | X i -> List.assoc i env
    | Vrai -> true
    | Faux -> false
    | Et(x1,x2) -> eval x1 env && eval x2 env 
    | Ou(x1,x2) -> eval x1 env || eval x2 env
    | Non(x) -> not (eval x env);;
    
let satisf env equ =
    List.for_all (fun (x1,x2) -> eval x1 env = eval x2 env) equ;;

let res equ =
    let vars = get_var equ in
    let envs = env vars in 
    List.filter (fun env -> satisf env equ) envs;;

let print_solutions env = 
    let sorted_env = List.sort (fun(e1, _) (e2, _) -> compare e1 e2) env in 
    List.map (fun (e, b) -> (e, if b then "Vrai" else "Faux")) sorted_env;;

let exemple = let equ = [eq1; eq2; eq3] in 
    let solutions = res equ in 
    List.iteri (fun e env -> 
        let formated = print_solutions env in 
        Printf.printf "Solutions %d:\n" (e+1);
        List.iter (fun (var, value) ->
            Printf.printf "X%d = %s\n" var value) formated;
            print_endline"" ) solutions;;

