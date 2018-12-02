let input_file = "./input/day_2_input.txt"

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
  let make_stream channel =
    Stream.from
      (fun _ -> try Some (input_line channel) with End_of_file -> None)
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

let () =
  print_endline "* Day 2 *";
  part_one ();
