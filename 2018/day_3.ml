open Lib;;
let input_file = "./input/day_3_input.txt"

let parse_claim input =
  let regex =
    "#\\([0-9]+\\) @ \\([0-9]+\\),\\([0-9]+\\): \\([0-9]+\\)x\\([0-9]+\\)"
  in
  let _ = Str.search_forward (Str.regexp regex) input 0 in
  let list = ref [] in
  for index = 1 to 5 do
    list := Str.matched_group index input :: !list
  done;
  List.map int_of_string (List.rev !list)

let fabric =
  Lib.stream_fold
    (fun line matrix ->
     let [id; x; y; width; height] = parse_claim line in
     for i = x to (x + width) - 1 do
       for j = y to (y + height) - 1 do
         matrix.(i).(j) <- matrix.(i).(j) + 1;
       done;
     done;
     matrix
    )
    (make_stream input_line input_file)
    (Array.make_matrix 1000 1000 0)


let part_one() =
  let overlapping_claims = ref 0 in
  for i = 0 to 1000 - 1 do
    for j = 0 to 1000 - 1 do
      if fabric.(i).(j) >= 2 then incr overlapping_claims
    done;
  done;
  Printf.printf "[3.1] = %d\n" !overlapping_claims

(* Amidst the chaos, you notice that exactly one claim doesn't overlap
by even a single square inch of fabric with any other claim. If you
can somehow draw attention to it, maybe the Elves will be able to make
Santa's suit after all!  For example, in the claims above, only claim
3 is intact after all claims are made. What is the ID of the only claim
that doesn't overlap? *)

let part_two () =
  let stream = make_stream input_line input_file in
  let claim = Lib.stream_find
                (fun line ->
                 let [id; x; y; width; height] = parse_claim line in
                 let count = ref 0 in
                 for i=x to x + width do
                   for j=y to y + height do
                     count := fabric.(i).(j) + !count;
                   done;
                 done;
                 !count == width * height
                ) stream
  in
  let [id; x; y; width; height] = parse_claim claim in
  Printf.printf "[3.1] = %d\n" id

let () =
  print_endline "* Day 3 *";
  part_one ();
  part_two ()
