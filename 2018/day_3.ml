open Lib;;
let input_file = "./input/day_3_input.txt"

let parse regex input matches =
  let _ = Str.search_forward (Str.regexp regex) input 0 in
  let list = ref [] in
  for index = 1 to matches do
    list := Str.matched_group index input :: !list
  done;
  List.rev !list

let part_one() =
  let stream = make_stream input_line input_file in
  let regex  =
    "#\\([0-9]+\\) @ \\([0-9]+\\),\\([0-9]+\\): \\([0-9]+\\)x\\([0-9]+\\)"
  in
  let fabric =
    Lib.stream_fold
      (fun line matrix ->
       let input = parse regex line 5 in
       let [id; x; y; width; height] = List.map int_of_string input in
       for i = x to (x + width) - 1 do
         for j = y to (y + height) - 1 do
           matrix.(i).(j) <- matrix.(i).(j) + 1;
         done;
       done;
       matrix
      )
      stream
      (Array.make_matrix 1000 1000 0) in
  let overlapping_claims = ref 0 in
  for i = 0 to 1000 - 1 do
    for j = 0 to 1000 - 1 do
      if fabric.(i).(j) >= 2 then incr overlapping_claims
    done;
  done;
  Printf.printf "[1.1] = %d\n" !overlapping_claims

let () =
  print_endline "* Day 3 *";
  part_one ()
