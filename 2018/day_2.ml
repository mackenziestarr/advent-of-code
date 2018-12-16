open Lib;;
exception Found of int

let input_file = "./input/day_2_input.txt"

type id_matches = {
  mutable two   : int;
  mutable three : int;
}

(* @todo: add problem description *)
let part_one() =

  let get_matches input =
    let ht = Hashtbl.create 26 in
    String.iter
      (fun char ->
       let value = try Hashtbl.find ht char with Not_found -> 0 in
       Hashtbl.replace ht char (value + 1)) input;

    let matches = {two = 0; three = 0} in
    let update_matches char freq =
      match freq with
      | 2 -> matches.two <- 1;
      | 3 -> matches.three <-1;
      | _ -> () in
    Hashtbl.iter update_matches ht;
    matches
  in

  let lines = make_stream input_line input_file in
  let two_matches, three_matches =
    Lib.stream_fold
      (fun input (twos, threes) ->
       let matches = get_matches input in
       (twos + matches.two, threes + matches.three))
      lines (0, 0)
  in
  Printf.printf "[1.1] = %d\n" (two_matches * three_matches)

let part_two() =
  let levenshtein s t =
    let m = String.length s in
    let n = String.length t in
    let matrix = Array.make_matrix (m + 1) (n + 1) 0 in
    for i = 0 to m do matrix.(i).(0) <- i done;
    for j = 0 to n do matrix.(0).(j) <- j done;
    for i = 1 to n do
      for j = 1 to m do
        if s.[i-1] = t.[j-1] then
          matrix.(i).(j) <- matrix.(i-1).(j-1)
        else
          matrix.(i).(j) <- min (matrix.(i-1).(j) + 1)
                              (min (matrix.(i).(j-1) + 1)
                                   (matrix.(i-1).(j-1) + 1))
      done;
    done;
    matrix.(m).(n)
  in
  let rec find_similar_ids lines current_index =
    let a =  Array.get lines current_index in
    let next_index = current_index + 1 in
    try
      for i = next_index to (Array.length lines) - 1 do
        let b = Array.get lines i in
        let edit_distance = levenshtein a b in
        if edit_distance == 1 then raise (Found i);
      done;
      find_similar_ids lines next_index
    with Found i -> (Array.get lines current_index, Array.get lines i)
  in
  let lines  = Array.of_list(Lib.list_of_stream(make_stream input_line input_file)) in
  let a, b = find_similar_ids lines 0 in
  let find_common_chars a b =
    let common = ref [] in
    String.iteri
      (fun index char ->
       if char == b.[index] then
         common := char :: !common;
      ) a;
    String.concat "" (List.map (String.make 1) (List.rev !common))
  in
  Printf.printf "[1.2] = %s\n" (find_common_chars a b)

let () =
  print_endline "* Day 2 *";
  part_one ();
  part_two ()
