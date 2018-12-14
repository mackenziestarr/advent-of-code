let stream_fold f stream init =
    let result = ref init in
    Stream.iter
      (fun x -> result := f x !result)
      stream;
    !result

let make_stream input_file =
  let file = (open_in input_file) in
  (Stream.from
     (fun _ ->
      try Some (input_line file)
      with End_of_file -> None))

let list_of_stream stream =
  let result = ref [] in
  Stream.iter (fun value -> result := value :: !result) stream;
  List.rev !result
