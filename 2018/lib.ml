let stream_fold f stream init =
    let result = ref init in
    Stream.iter
      (fun x -> result := f x !result)
      stream;
    !result

(* returns first element that matches predicate else raises Not_found *)
let rec stream_find fn stream =
  try
    let value = Stream.next stream in
    match fn value with
    | true  -> value
    | false -> stream_find fn stream
  with Stream.Failure -> raise Not_found

let make_stream fn input_file =
  let file = (open_in input_file) in
  (Stream.from
     (fun _ ->
      try Some (fn file)
      with End_of_file -> None))

let of_ints chan = chan |> input_line |> int_of_string

let identity x = x

let list_of_stream stream =
  let result = ref [] in
  Stream.iter (fun value -> result := value :: !result) stream;
  List.rev !result

let array_of_stream stream =
  let result = ref [||] in
  Stream.iter (fun value -> result := Array.append !result [|value|]) stream;
  !result

let regex_matches_to_list regex input =
  let _ = Str.search_forward (Str.regexp regex) input 0 in
  let list  = ref [] in
  let index = ref 0 in
  try
    while true do
      (incr index);
      list := (Str.matched_group !index input) :: !list;
    done;
    List.rev !list
  with Not_found | Invalid_argument _ -> List.rev !list
