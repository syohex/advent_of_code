open System

let toString (ns: int list) : string =
    let rec toString' ns (acc: char list) =
        match ns with
        | [] -> String.Concat acc
        | h :: t ->
            let c = char <| (h + int 'a')
            toString' t (c :: acc)

    toString' ns []

let toList (s: string) : int list =
    s |> Seq.fold (fun acc c -> (int c - int 'a') :: acc) []

let increment (ns: int list) : int list =
    let rec increment' ns carry acc =
        match ns with
        | [] ->
            if carry = 0 then
                List.rev acc
            else
                increment' [] 0 (1 :: acc)
        | h :: t ->
            let v = h + carry

            if v >= 26 then
                increment' t 1 (0 :: acc)
            else
                increment' t 0 (v :: acc)

    increment' ns 1 []

let isThreeStraight (ns: int list) : bool =
    let rec isThreeStraight' ns prev1 prev2 =
        match ns with
        | [] -> false
        | h :: t ->
            if h + 1 = prev1 && h + 2 = prev2 && prev1 + 1 = prev2 then
                true
            else
                isThreeStraight' t h prev1

    match ns with
    | []
    | _ :: []
    | _ :: _ :: [] -> false
    | h2 :: h1 :: t -> isThreeStraight' t h1 h2

let invalidChars = [ 'i'; 'o'; 'l' ] |> List.map (fun c -> int c - int 'a')

let hasInvalidChar (ns: int list) : bool =
    ns |> List.exists (fun n -> List.contains n invalidChars)

let hasTwoOverwrapped (ns: int list) : bool =
    let rec hasTwoOverwrapped' ns prev count firstOverwrap =
        match ns with
        | [] -> false
        | h :: t ->
            if h = prev && h <> firstOverwrap then
                if count = 1 then true else hasTwoOverwrapped' t h 1 h
            else
                hasTwoOverwrapped' t h count firstOverwrap

    hasTwoOverwrapped' ns 27 0 27

let problem1 (s: string) : string =
    let rec f ns =
        if isThreeStraight ns && not <| hasInvalidChar ns && hasTwoOverwrapped ns then
            toString ns
        else
            f (increment ns)

    let ns = toList s |> increment
    f ns

// "abcdffaa"
problem1 "abcdefgh"
// "ghjaabcc"
problem1 "ghijklmn"

let input = "hepxcrrq"
let ret1 = problem1 input
let ret2 = problem1 ret1

// hepxxyzz
printfn "problem1 = %s" ret1
// heqaabcc
printfn "problem2 = %s" ret2
