exception Found of int

let input_file = "./input/day_2_input.txt"

let make_stream channel =
  Stream.from
    (fun _ -> try Some (input_line channel) with End_of_file -> None)

let part_one() =
  let get_matches input =
    let two_match = ref 0 in
    let three_match = ref 0 in
    let ht = Hashtbl.create 26 in
    String.iter
      (fun char ->
       let value = try Hashtbl.find ht char with Not_found -> 0 in
       Hashtbl.replace ht char (value + 1)) input;
    Hashtbl.iter
      (fun char freq ->
       if freq == 2 then two_match   := 1;
       if freq == 3 then three_match := 1;) ht;
    (!two_match, !three_match)
  in
  let stream_fold f stream init =
    let result = ref init in
    Stream.iter (fun x -> result := f x !result) stream;
    !result
  in
  let lines = (make_stream (open_in input_file)) in
  let two_matches, three_matches =
    stream_fold
      (fun input (a, b) ->
       let c, d = get_matches input in
       (a + c, b + d)) lines (0, 0)
  in
  Printf.printf "[1.1] = %d\n" (two_matches * three_matches)

let part_two() =
  let list_of_stream stream =
    let result = ref [] in
    Stream.iter
      (fun value -> result := value :: !result) stream;
    List.rev !result
  in
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
  let lines  = Array.of_list(list_of_stream(make_stream(open_in input_file))) in
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
