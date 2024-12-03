open System
open System.IO

type JSON =
    | JObject of (string * JSON) list
    | JArray of JSON list
    | JNumber of int
    | JString of string
    | JBoolean of bool
    | JNull

let rec parseString (cs: char list) (acc: char list) : (string * char list) =
    match cs with
    | [] -> failwith "not found close quote"
    | '"' :: t -> acc |> List.rev |> String.Concat, t
    | h :: t -> parseString t (h :: acc)

let rec parseNumber (cs: char list) (acc: int) : (int * char list) =
    match cs with
    | [] -> acc, []
    | h :: t when Char.IsAsciiDigit(h) -> parseNumber t (acc * 10 + int h - int '0')
    | _ -> acc, cs

let rec skipSpace (cs: char list) : char list =
    match cs with
    | [] -> []
    | h :: t when Char.IsWhiteSpace(h) -> skipSpace t
    | _ -> cs

let rec parseJson' (cs: char list) : (JSON * char list) =
    match cs with
    | [] -> failwith "never reach here2"
    | '{' :: t -> parseObject t []
    | '[' :: t -> parseArray t []
    | '"' :: t ->
        let s, t = parseString t []
        JString s, t
    | '-' :: t ->
        let v, t = parseNumber t 0
        JNumber -v, t
    | h :: _ when Char.IsAsciiDigit(h) ->
        let v, t = parseNumber cs 0
        JNumber v, t
    | 't' :: 'r' :: 'u' :: 'e' :: t -> JBoolean true, t
    | 'f' :: 'a' :: 'l' :: 's' :: 'e' :: t -> JBoolean false, t
    | 'n' :: 'u' :: 'l' :: 'l' :: t -> JNull, t
    | _ -> failwith "error"

and parseObject (cs: char list) (acc: (string * JSON) list) : (JSON * char list) =
    let cs = skipSpace cs

    match cs with
    | '}' :: t -> JObject(acc |> List.rev), t
    | ',' :: t -> parseObject t acc
    | '"' :: t ->
        let t = skipSpace t
        let key, t = parseString t []
        let t = skipSpace t

        match t with
        | ':' :: t ->
            let t = skipSpace t
            let v, t = parseJson' t
            let t = skipSpace t
            parseObject t ((key, v) :: acc)
        | _ -> failwith "missing colon"
    | _ -> failwithf "unknown input in object: %A" cs

and parseArray (cs: char list) (acc: JSON list) : (JSON * char list) =
    let cs = skipSpace cs

    match cs with
    | ']' :: t -> JArray(List.rev acc), t
    | ',' :: t -> parseArray t acc
    | _ ->
        let v, t = parseJson' cs
        parseArray t (v :: acc)

let parseJson (s: string) : JSON =
    let json, t = parseJson' (Seq.toList s)

    if List.isEmpty t then
        json
    else
        failwithf "there are some unparsed characters: %s" (t |> String.Concat)

let rec sumNumbersInJson (v: JSON) : int =
    match v with
    | JArray vs -> vs |> List.fold (fun acc v -> acc + sumNumbersInJson v) 0
    | JObject vs -> vs |> List.map snd |> List.fold (fun acc v -> acc + sumNumbersInJson v) 0
    | JNumber v -> v
    | _ -> 0

let rec sumNumbersInJsonExceptRed (v: JSON) : int =
    match v with
    | JArray vs -> vs |> List.fold (fun acc v -> acc + sumNumbersInJsonExceptRed v) 0
    | JObject vs ->
        let hasRed =
            vs
            |> List.exists (fun (_, v) ->
                match v with
                | JString s when s = "red" -> true
                | _ -> false)

        if hasRed then
            0
        else
            vs
            |> List.map snd
            |> List.fold (fun acc v -> acc + sumNumbersInJsonExceptRed v) 0
    | JNumber v -> v
    | _ -> 0

let problem1 = parseJson >> sumNumbersInJson
let problem2 = parseJson >> sumNumbersInJsonExceptRed

// 6
problem1 "[1,2,3]"
// 6
problem1 "{\"a\":2,\"b\":4}"
// 3
problem1 "[[[3]]]"
// 3
problem1 "{\"a\":{\"b\":4},\"c\":-1}"
// 0
problem1 "{\"a\":[-1,-1]}"
// 0
problem1 "[-1,{\"a\":1}]"

// 6
problem2 "[1,2,3]"
// 4
problem2 "[1,{\"c\":\"red\",\"b\":2},3]"
// 0
problem2 "{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}"
// 6
problem2 "[1,\"red\",5]"

let input = File.ReadLines("../input/day12.txt") |> Seq.head
let ret1 = problem1 input
printfn "ret1 = %d" ret1

let ret2 = problem2 input
printfn "ret2 = %d" ret2
